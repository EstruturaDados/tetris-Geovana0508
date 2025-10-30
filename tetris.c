#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5     // tamanho fixo da fila de peças futuras
#define CAP_PILHA 3    // capacidade máxima da pilha de reserva

/* Representa uma peça do jogo */
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // identificador único
} Peca;

/* Fila circular */
typedef struct {
    Peca pecas[TAM_FILA];
    int frente;     // índice da frente
    int tras;       // índice do fim (último inserido)
    int quantidade; // quantidade atual de elementos
} Fila;

/* Pilha linear (vetor) */
typedef struct {
    Peca pilha[CAP_PILHA];
    int topo; // índice do topo (-1 quando vazia)
} Pilha;

/* ----- Assinaturas de funções ----- */
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
int enfileirar(Fila *f, Peca p);
int desenfileirar(Fila *f, Peca *p); // remove da frente e retorna via ponteiro

void inicializarPilha(Pilha *p);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);
int push(Pilha *p, Peca peça);
int pop(Pilha *p, Peca *peça);

Peca gerarPeca(int id);
void exibirEstado(Fila *f, Pilha *p);
void limparBufferEntrada(void);

/* ----- Implementações ----- */

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

int filaCheia(Fila *f) {
    return f->quantidade == TAM_FILA;
}

/* Enfileira: retorna 1 se sucesso, 0 se fila cheia */
int enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->pecas[f->tras] = p;
    f->quantidade++;
    return 1;
}

/* Desenfileira: retorna 1 se sucesso e copia peça removida para *p, 0 se vazia */
int desenfileirar(Fila *f, Peca *p) {
    if (filaVazia(f)) return 0;
    *p = f->pecas[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->quantidade--;
    return 1;
}

/* Pilha */
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == CAP_PILHA - 1;
}

/* Push: retorna 1 se sucesso, 0 se cheia */
int push(Pilha *p, Peca peça) {
    if (pilhaCheia(p)) return 0;
    p->topo++;
    p->pilha[p->topo] = peça;
    return 1;
}

/* Pop: retorna 1 se sucesso e copia peça removida para *peça, 0 se vazia */
int pop(Pilha *p, Peca *peça) {
    if (pilhaVazia(p)) return 0;
    *peça = p->pilha[p->topo];
    p->topo--;
    return 1;
}

/* Gera peça aleatória com id único */
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

/* Exibe fila e pilha no formato amigável */
void exibirEstado(Fila *f, Pilha *p) {
    printf("\nEstado atual:\n");
    printf("Fila de peças: ");
    if (filaVazia(f)) {
        printf("(vazia)");
    } else {
        for (int i = 0; i < f->quantidade; i++) {
            int idx = (f->frente + i) % TAM_FILA;
            printf("[%c %d] ", f->pecas[idx].nome, f->pecas[idx].id);
        }
    }
    printf("\n");

    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->pilha[i].nome, p->pilha[i].id);
        }
    }
    printf("\n");
}

/* Limpa o buffer da entrada para evitar problemas após scanf */
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* ----- Programa principal ----- */
int main(void) {
    Fila fila;
    Pilha reserva;
    int idAtual = 0;
    int opcao;
    srand((unsigned int) time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&reserva);

    /* Preenche a fila inicial com TAM_FILA peças automáticas */
    for (int i = 0; i < TAM_FILA; i++) {
        Peca p = gerarPeca(idAtual++);
        if (!enfileirar(&fila, p)) {
            // Não deve acontecer aqui, mas é seguro checar
            fprintf(stderr, "Erro: não foi possível inicializar a fila.\n");
            return 1;
        }
    }

    do {
        exibirEstado(&fila, &reserva);

        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça (mover da fila para a pilha)\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida. Saindo.\n");
            break;
        }
        limparBufferEntrada();

        if (opcao == 1) { /* Jogar peça: remove frente da fila (dequeue) */
            Peca jogada;
            if (!desenfileirar(&fila, &jogada)) {
                printf("\n  A fila está vazia — nenhuma peça para jogar.\n");
            } else {
                printf("\n Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                /* gerar nova peça para manter fila cheia */
                Peca nova = gerarPeca(idAtual++);
                if (!enfileirar(&fila, nova)) {
                    printf("  Não foi possível enfileirar nova peça (fila cheia).\n");
                } else {
                    printf(" Nova peça gerada e adicionada: [%c %d]\n", nova.nome, nova.id);
                }
            }

        } else if (opcao == 2) { /* Reservar peça: move frente da fila para topo da pilha */
            if (pilhaCheia(&reserva)) {
                printf("\n  Pilha de reserva cheia! Não é possível reservar.\n");
            } else {
                Peca mover;
                if (!desenfileirar(&fila, &mover)) {
                    printf("\n  A fila está vazia — nenhuma peça para reservar.\n");
                } else {
                    if (!push(&reserva, mover)) {
                        /* Se não conseguiu empilhar, devolver à fila (tentar restaurar) */
                        printf("\n  Erro ao mover para a pilha; restaurando peça à fila.\n");
                        /* Tentar enfileirar de volta (deve ter espaço mas checamos) */
                        if (!enfileirar(&fila, mover)) {
                            printf("Erro crítico: não foi possível restaurar peça à fila.\n");
                        }
                    } else {
                        printf("\n Peça reservada: [%c %d]\n", mover.nome, mover.id);
                        /* gerar nova peça para manter fila cheia */
                        Peca nova = gerarPeca(idAtual++);
                        if (!enfileirar(&fila, nova)) {
                            printf("  Não foi possível enfileirar nova peça (fila cheia).\n");
                        } else {
                            printf(" Nova peça gerada e adicionada: [%c %d]\n", nova.nome, nova.id);
                        }
                    }
                }
            }

        } else if (opcao == 3) { /* Usar peça reservada: pop da pilha */
            Peca usada;
            if (!pop(&reserva, &usada)) {
                printf("\n  Pilha vazia — não há peça reservada para usar.\n");
            } else {
                printf("\n Peça usada (da reserva): [%c %d]\n", usada.nome, usada.id);
                /* Se a fila não estiver cheia (por qualquer razão), gera e enfileira nova peça */
                if (!filaCheia(&fila)) {
                    Peca nova = gerarPeca(idAtual++);
                    if (!enfileirar(&fila, nova)) {
                        printf("  Falha ao enfileirar nova peça.\n");
                    } else {
                        printf(" Nova peça gerada e adicionada: [%c %d]\n", nova.nome, nova.id);
                    }
                } else {
                    /* A fila já está cheia — nada a fazer; mantemos invariantes */
                }
            }

        } else if (opcao == 0) {
            printf("\nSaindo...\n");
        } else {
            printf("\n  Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
