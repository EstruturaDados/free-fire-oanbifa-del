#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Código da Ilha – Edição Free Fire
// Nível: Novato
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.
typedef struct {    // Estrutura contendo 3 atributos
    char nome[30];
    char tipo[20];
    int qtd;
} Item;

Item mochila[10]; // vetor do tipo Item que contêm 10 elementos com 3 campos cada

int totalItens = 0;

// --- Protótipos das funçoes ---
void limparBufferEntrada();
void inserirItem(Item mochila[]);
void removerItem(Item mochila[]);
void listarItens(Item mochila[]);
int buscarItem(Item mochila[], int totalItens, const char *nome);
void buscarItemPorNome(Item mochila[]);
void exibirMenu();

int main() {
    int opcao;
    char entrada[10];

    #ifdef _WIN32 // Se a macro _WIN32 estiver DEFINIDA (compilando no Windows)
        system("cls"); //limpa a tela no win
    #else              // Senão (Linux, Mac, etc.)
        system("clear"); //limpa tela no linux, mac, etc
    #endif

    printf("-----------------------------------------------------\n");
    printf("    MOCHILA DE SOBREVIVÊNCIA - CÓDIGO DA ILHA\n");
    printf("-----------------------------------------------------\n");
    listarItens(mochila);
// ====================================
  // --- Laço Principal do menu ---
// ====================================
  do {
      exibirMenu();

      fgets(entrada, sizeof(entrada), stdin);
      // remove o \n
      entrada[strcspn(entrada, "\n")] = '\0';
      // tenta converter para número
      if (sscanf(entrada, "%d", &opcao) != 1) {
          printf("\nEntrada invalida! Digite um numero.\n\n");
          continue;
      }

      // valida opções do menu
      if (opcao != 0 && opcao != 1 && opcao != 2 && opcao != 3 && opcao != 4) {
          printf("Opcao invalida!\n\n");
          continue;
      }

      #ifdef _WIN32 //limpando a tela se for win
          system("cls");
      #else
          system("clear"); // limpando tela no Linux
      #endif

      switch (opcao) {
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
          buscarItemPorNome(mochila);
          break;
      case 0:
          printf("\nSaindo do sistema...\n");
          break;
      }
  } while (opcao != 0);

  return 0;
}
  // ---=== Implementação das FUNÇÕES ===---
  
  // Função Buffer de entrada
  void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
  }
  // Função Adicionar item
  void inserirItem(Item mochila[]) {
      if (totalItens >= 10) {
          printf("Mochila cheia!\n");
          return;
      }

      Item novo;

      printf("\n--- Adicionar novo item ---\n");

      printf("Nome do Item: ");
      fgets(novo.nome, sizeof(novo.nome), stdin);
      novo.nome[strcspn(novo.nome, "\n")] = '\0'; // remove \n

      printf("Tipo do Item (arma, munição, cura, etc.): ");
      fgets(novo.tipo, sizeof(novo.tipo), stdin);
      novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

      printf("Quantidade: ");
      scanf("%d", &novo.qtd);
      limparBufferEntrada();

      // adiciona na mochila
      mochila[totalItens] = novo;
      totalItens++;

      printf("Item %s adicionado com sucesso!\n", novo.nome);
  }
    // ---=== Função Buscar Item ===---

  int buscarItem(Item mochila[], int totalItens, const char *nome) {
      for (int i = 0; i < totalItens; i++) {
          if (strcmp(mochila[i].nome, nome) == 0) {
              return i;
          }
      }
      return -1;
  }

  void buscarItemPorNome(Item mochila[]) {
      char nomeBusca[30];

      printf("Digite o nome do item a buscar: ");
      fgets(nomeBusca, sizeof(nomeBusca), stdin);
      nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

      int pos = buscarItem(mochila, totalItens, nomeBusca);

      if (pos != -1) {
          printf("Item encontrado!\n");
          printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                 mochila[pos].nome,
                 mochila[pos].tipo,
                 mochila[pos].qtd);
      } else {
          printf("Item nao encontrado!\n");
      }
  }

    // Função Remover Itens
  void removerItem(Item mochila[]) {
      if (totalItens == 0) {
          printf("Mochila vazia!\n");
          return;
      }

      char nomeBusca[30];
      printf("Digite o nome do item a ser removido: ");
      fgets(nomeBusca, sizeof(nomeBusca), stdin);
      nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int pos = buscarItem(mochila, totalItens, nomeBusca); //reaproveita a buscarItem

    if (pos != -1) {
        for (int i = pos; i < totalItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        totalItens--;
        printf("Item %s removido com sucesso!\n", nomeBusca);
    } else {
        printf("Item nao encontrado!\n\n");
    }
}

  // Função listar itens
  void listarItens(Item mochila[]){
      printf("\n--- Itens na Mochila %d/10 ---\n", totalItens);
      if (totalItens == 0){
          printf("Mochila vazia, adicione itens!\n");
          return;
      }

      printf("-----------------------------------------------------\n");
      printf(" NOME                 | TIPO             | QUANTIDADE\n");
      printf("-----------------------------------------------------\n");

      for(int i = 0; i < totalItens; i++){
          printf("%d. %-18s | %-16s | %d\n", i+1, mochila[i].nome, mochila[i].tipo, mochila[i].qtd);
      }
      printf("-------------------------------------------------\n");
   }

  // Função Exibir Menu
  void exibirMenu() {

    printf("\n--- MENU DE AÇÕES ---\n");
    printf("1 - Adicionar Item.\n");
    printf("2 - Remover Item\n");
    printf("3 - Listar Itens na Mochila\n");
    printf("4 - Buscar Item por nome\n");
    printf("0 - Sair\n");
    printf("-------------------------------------------------\n");
    printf("Escolha sua ação: \n");
  }


// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
