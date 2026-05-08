#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Código da Ilha – Edição Free Fire
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

typedef struct //<--Estrutura contendo 3 atributos
{
    char nome[30];
    char tipo[20];
    int qtd;
    int prioridade;
} Item;

int ordenadoPorNome = 0;

Item mochila[20]; // vetor do tipo Item que contêm 20 elementos com 3 campos cada

int totalItens = 0;

// --- Protótipos das funçoes ---
void limparBufferEntrada();
void inserirItem(Item mochila[]);
void removerItem(Item mochila[]);
void listarItens(Item mochila[]);
void ordenarPorNome(Item mochila[]);
void ordenarPorTipo(Item mochila[]);
void ordenarPorPrioridade(Item mochila[]);
int buscarItem(Item mochila[], int totalItens, const char *nome);
void buscarItemPorNome(Item mochila[]);
int buscaBinariaPorNome(Item mochila[]);
void exibirMenu();
void exibirMenuOrdenacao();
void menuOrdenacao(Item mochila[]);
void salvarDados(Item mochila[]);   // salva os dados em arquivo para facilitar testes
void carregarDados(Item mochila[]); // carrega os dados

int main()
{
    int opcao;
    char entrada[20];

#ifdef _WIN32      // Se a macro _WIN32 estiver DEFINIDA (compilando no Windows)
    system("cls"); // limpa a tela no win
#else              // Senão (Linux, Mac, etc.)
    system("clear"); // limpa tela no linux, mac, etc
#endif

    carregarDados(mochila); // carrega os dados
    char statusOrdenacao[30];

    // ====================================
    // --- Laço Principal do menu ---
    // ====================================
    do
    {
        if(!ordenadoPorNome){ // equivale a (ordenadoPorNome == 0)
            strcpy(statusOrdenacao, "NAO ORDENADO");
        } else{
            strcpy(statusOrdenacao, "ORDENADO");
        }
        printf("-----------------------------------------------------\n");
        printf("    PLANO DE FUGA - CÓDIGO DA ILHA (Nível Mestre)\n");
        printf("-----------------------------------------------------\n");
        printf("Itens na mochila: %d/20\n", totalItens);
        printf("Status da Ordenação por Nome: %s\n\n",  statusOrdenacao);
        exibirMenu();

        fgets(entrada, sizeof(entrada), stdin);

        entrada[strcspn(entrada, "\n")] = '\0'; // remove o \n

        if (sscanf(entrada, "%d", &opcao) != 1)
        { // tenta converter para número
            printf("\nEntrada invalida! Digite um numero.\n\n");
            continue;
        }

        // Não aceita opções que não sejam as dadas
        if (opcao != 0 && opcao != 1 && opcao != 2 && opcao != 3 && opcao != 4 && opcao != 5)
        {
            printf("Opcao invalida!\n\n");
            continue;
        }

#ifdef _WIN32 // limpando a tela se for win
        system("cls");
#else
        system("clear"); // limpando tela no Linux
#endif

        switch (opcao)
        {
        case 1:
            inserirItem(mochila);
            break;
        case 2:
            removerItem(mochila);
            break;
        case 3:
            listarItens(mochila);
            break;
        case 4:
            menuOrdenacao(mochila);
            break;
        case 5:
        {
            int pos = buscaBinariaPorNome(mochila);

            if (pos == -2)
            {
                break;
            }
            else if (pos != -1)
            {
                printf("Item encontrado!\n");
                printf("Nome: %s\n", mochila[pos].nome);
            }
            else
            {
                printf("Item nao encontrado!\n");
            }

            break;
        }
        case 0:
            salvarDados(mochila);
            printf("\nSaindo do sistema...\n");
            break;
        }
    } while (opcao != 0);

    return 0;
}
// ---=== Implementação das FUNÇÕES ===---

// Função pra limpar Buffer de entrada
void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
// Função para Adicionar item
void inserirItem(Item mochila[])
{
    if (totalItens >= 20)
    {
        printf("Mochila cheia!\n");
        return;
    }

    Item novo;

    printf("\n--- Adicionar Novo Item ---\n");

    printf("Nome do Item: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // remove o \n(enter)

    printf("Tipo do Item (arma, munição, cura, etc.): ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo.qtd);

    printf("Prioridade: ");
    scanf("%d", &novo.prioridade);

    limparBufferEntrada();

    // adiciona na mochila
    mochila[totalItens] = novo;
    totalItens++;

    printf("Item %s adicionado com sucesso!\n\n", novo.nome);

    ordenadoPorNome = 0;
}

// Função Remover Itens
void removerItem(Item mochila[])
{
    if (totalItens == 0)
    {
        printf("Mochila vazia!\n");
        return;
    }

    char nomeBusca[30];
    printf("Digite o nome do item a ser removido: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int pos = buscarItem(mochila, totalItens, nomeBusca); // reaproveita a buscarItem

    if (pos != -1)
    {
        for (int i = pos; i < totalItens - 1; i++)
        {
            mochila[i] = mochila[i + 1];
        }
        totalItens--;
        printf("Item %s removido com sucesso!\n", nomeBusca);
    }
    else
    {
        printf("Item nao encontrado!\n\n");
    }
    ordenadoPorNome = 0;
}

// Função listar itens
void listarItens(Item mochila[])
{
    printf("\n--- Itens na Mochila %d/20 ---\n", totalItens);
    if (totalItens == 0)
    {
        printf("Mochila vazia, adicione itens!\n");
        return;
    }

    printf("-----------------------------------------------------\n");
    printf(" NOME                 | TIPO             | QUANTIDADE | PRIORIDADE\n");
    printf("-----------------------------------------------------\n");

    for (int i = 0; i < totalItens; i++)
    {
        printf("%d. %-18s | %-16s | %-10d | %-10d\n", i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].qtd, mochila[i].prioridade);
    }
    printf("-------------------------------------------------\n");
}

// Função Exibir Menu
void exibirMenu()
{

    printf("1 - Adicionar Item.\n");
    printf("2 - Remover Item\n");
    printf("3 - Listar Itens na Mochila\n");
    printf("4 - Organizar Mochila (Ordenar Itens)\n");
    printf("5 - Busca Binaria por Item-Chave (por nome)\n");
    printf("0 - ATIVAR TORRE DE FUGA(Sair)\n");
    printf("-------------------------------------------------\n");
    printf("Escolha sua ação: \n");
}
// Função Exibir Menu Ordenação
void exibirMenuOrdenacao()
{

    printf("--- Estratégia de Organização ---\n");
    printf("Como deseja ordenar os itens?\n");
    printf("1 - Por Nome (Ordem Alfabética)\n");
    printf("2 - Por Tipo\n");
    printf("3 - Por Prioridade\n");
    printf("0 - Cancelar\n");
    printf("Escolha o critério: \n");
}

void menuOrdenacao(Item mochila[])
{
    int op; // opção
    char entrada[20];

    exibirMenuOrdenacao();

    fgets(entrada, sizeof(entrada), stdin);

    entrada[strcspn(entrada, "\n")] = '\0';

    // verifica se digitou número e não letra
    if (sscanf(entrada, "%d", &op) != 1)
    {
        printf("\nEntrada invalida! Digite um numero.\n\n");
        return;
    }

    // verifica se opção existe
    if (op != 0 && op != 1 && op != 2 && op != 3)
    {
        printf("Opcao invalida!\n");
        return;
    }

    switch (op)
    {
    case 1:
        ordenarPorNome(mochila);
        break;
    case 2:
        ordenarPorTipo(mochila);
        break;
    case 3:
        ordenarPorPrioridade(mochila);
        break;
    case 0:
        printf("Cancelado.\n");
        break;
    }
}

// --- FUNÇÔES DE ORDENAÇÃO ---
// Bubble Sort
void ordenarPorNome(Item mochila[])
{
    clock_t inicio = clock();
    Item temp; // variável local do tipo Item(struct)
    int comparacoes = 0;
    for (int i = 0; i < totalItens - 1; i++)
    {
        for (int j = 0; j < totalItens - 1 - i; j++)
        {
            comparacoes++;
            if (strcmp(mochila[j].nome, mochila[j + 1].nome) > 0)
            {
                temp = mochila[j];
                mochila[j] = mochila[j + 1];
                mochila[j + 1] = temp;
            }
        }
    }
    ordenadoPorNome = 1;
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    listarItens(mochila);
    printf("Itens ordenados por nome!\n");
    printf("Comparacoes: %d\n", comparacoes);
    printf("Tempo: %f segundos\n", tempo);
}

// Insertion Sort
void ordenarPorTipo(Item mochila[])
{
    clock_t inicio = clock();
    Item chave;
    int comparacoes = 0;

    for (int i = 1; i < totalItens; i++)
    {
        chave = mochila[i];
        int j = i - 1;

        while (j >= 0)
        {
            comparacoes++;

            if (strcmp(mochila[j].tipo, chave.tipo) > 0)
            {
                mochila[j + 1] = mochila[j];
                j--;
            }
            else
            {
                break;
            }
        }

        mochila[j + 1] = chave;
    }

    clock_t fim = clock();

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    ordenadoPorNome = 0;

    listarItens(mochila);
    printf("Itens ordenados por tipo!\n");
    printf("Comparacoes: %d\n", comparacoes);
    printf("Tempo: %f segundos\n", tempo);
}
// Selection Sort
void ordenarPorPrioridade(Item mochila[])
{
    clock_t inicio = clock();
    int comparacoes = 0; // contador de comparações
    Item temp;

    for (int i = 0; i < totalItens - 1; i++)
    {

        int indice_min = i;

        for (int j = i + 1; j < totalItens; j++)
        {
            comparacoes++;

            if (mochila[j].prioridade < mochila[indice_min].prioridade)
            {
                indice_min = j;
            }
        }
        if (indice_min != i)
        {
            temp = mochila[i];
            mochila[i] = mochila[indice_min];
            mochila[indice_min] = temp;
        }
    }
    clock_t fim = clock();

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    ordenadoPorNome = 0; //atualiza já que saiu da ordem alfabetica

    listarItens(mochila);
    printf("Itens ordenados por Prioridade!\n");
    printf("Comparacoes: %d\n", comparacoes);
    printf("Tempo: %f segundos\n", tempo);
}
// --- BUSCAS ---
// ---=== Função Buscar Item ===---
int buscarItem(Item mochila[], int totalItens, const char *nome)
{
    for (int i = 0; i < totalItens; i++)
    {
        if (strcmp(mochila[i].nome, nome) == 0)
        {
            return i;
        }
    }
    return -1;
}
// Busca sequencial por nome
// Atualmente não utilizada no menu principal.
// Mantida para futuras comparações com busca binária.
void buscarItemPorNome(Item mochila[])
{
    char nomeBusca[30];

    printf("Digite o nome do item a buscar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int pos = buscarItem(mochila, totalItens, nomeBusca);

    if (pos != -1)
    {
        printf("Item encontrado!\n");
        printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
               mochila[pos].nome,
               mochila[pos].tipo,
               mochila[pos].qtd);
    }
    else
    {
        printf("Item nao encontrado!\n");
    }
}
// --- BUSCA BINÁRIA (lista deve estar ordenada!!) ---
int buscaBinariaPorNome(Item mochila[])
{

    if (!ordenadoPorNome)
    {
        printf("ALERTA!!! A mochila precisa estar ordenada por nome!\n\n");
        return -2;
    }

    int inicio = 0;
    int fim = totalItens - 1;
    char nomeBusca[30];

    printf("--- Busca Binária por Nome ---\n");
    printf("Nome do Item que deseja buscar: \n");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0'; // remove o '\n'(enter) do final

    while (inicio <= fim)
    {
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(mochila[meio].nome, nomeBusca);

        if (cmp == 0)
            return meio;
        else if (cmp < 0)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }

    return -1;
}


// Função Salvar dados no arquivo
void salvarDados(Item mochila[])
{

    FILE *arquivo;

    arquivo = fopen("mochila.dat", "wb"); // abre ou cria um arquivo binário

    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    // salva quantidade de itens
    fwrite(&totalItens, sizeof(int), 1, arquivo);

    // salva vetor de itens
    fwrite(mochila, sizeof(Item), totalItens, arquivo);

    fclose(arquivo);

    printf("Dados salvos com sucesso!\n");
}

// Função Carregar dados do arquivo
void carregarDados(Item mochila[])
{

    FILE *arquivo;

    arquivo = fopen("mochila.dat", "rb");

    if (arquivo == NULL)
    {
        printf("Nenhum arquivo encontrado.\n");
        return;
    }

    // lê quantidade de itens
    fread(&totalItens, sizeof(int), 1, arquivo);

    // lê os itens
    fread(mochila, sizeof(Item), totalItens, arquivo);

    fclose(arquivo);

    ordenadoPorNome = 0;

    printf("Dados carregados com sucesso!\n");
}
