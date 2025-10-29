#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5 // Tamanho fixo da fila de peças

// Estrutura que representa uma peça do Tetris Stack
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único da peça
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// Função que inicializa a fila
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
}

// Função que verifica se a fila está cheia
int filaCheia(Fila *fila) {
    return fila->quantidade == TAM_FILA;
}

// Função que verifica se a fila está vazia
int filaVazia(Fila *fila) {
    return fila->quantidade == 0;
}

// Função que gera automaticamente uma nova peça
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Função para inserir nova peça ao final da fila (enqueue)
void inserirPeca(Fila *fila, Peca nova) {
    if (filaCheia(fila)) {
        printf("\n  A fila está cheia! Não é possível inserir nova peça.\n");
        return;
    }
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->pecas[fila->tras] = nova;
    fila->quantidade++;
    printf("\n Peça [%c %d] adicionada com sucesso!\n", nova.nome, nova.id);
}

// Função para jogar (remover) a peça da frente da fila (dequeue)
void jogarPeca(Fila *fila) {
    if (filaVazia(fila)) {
        printf("\n  Não há peças para jogar!\n");
        return;
    }
    Peca removida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->quantidade--;
    printf("\n Peça [%c %d] jogada!\n", removida.nome, removida.id);
}

// Função que exibe o estado atual da fila
void exibirFila(Fila *fila) {
    printf("\n=== Fila de Peças ===\n");
    if (filaVazia(fila)) {
        printf("(vazia)\n");
        return;
    }
    int i, indice;
    for (i = 0; i < fila->quantidade; i++) {
        indice = (fila->frente + i) % TAM_FILA;
        printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
    }
    printf("\n=====================\n");
}

// Função principal
int main() {
    Fila fila;
    int opcao, idAtual = 0;
    srand(time(NULL));

    inicializarFila(&fila);

    // Inicializa a fila com 5 peças automáticas
    for (int i = 0; i < TAM_FILA; i++) {
        inserirPeca(&fila, gerarPeca(idAtual++));
    }

    do {
        exibirFila(&fila);
        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                inserirPeca(&fila, gerarPeca(idAtual++));
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("\n  Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
