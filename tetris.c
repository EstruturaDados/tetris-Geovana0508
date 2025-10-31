#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça do jogo
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[TAM_FILA];
    int inicio, fim, total;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// ---------- Funções de inicialização ----------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// ---------- Geração de peças ----------
char tipos[] = {'I', 'O', 'T', 'L'};
int contadorID = 0;

Peca gerarPeca() {
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contadorID++;
    return nova;
}

// ---------- Operações da Fila ----------
int filaCheia(Fila *f) { return f->total == TAM_FILA; }
int filaVazia(Fila *f) { return f->total == 0; }

void enfileirar(Fila *f, Peca p) {
    if (!filaCheia(f)) {
        f->pecas[f->fim] = p;
        f->fim = (f->fim + 1) % TAM_FILA;
        f->total++;
    }
}

Peca desenfileirar(Fila *f) {
    Peca p = {' ', -1};
    if (!filaVazia(f)) {
        p = f->pecas[f->inicio];
        f->inicio = (f->inicio + 1) % TAM_FILA;
        f->total--;
    }
    return p;
}

// ---------- Operações da Pilha ----------
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

void empilhar(Pilha *p, Peca pec) {
    if (!pilhaCheia(p)) {
        p->pecas[++p->topo] = pec;
    }
}

Peca desempilhar(Pilha *p) {
    Peca pec = {' ', -1};
    if (!pilhaVazia(p)) {
        pec = p->pecas[p->topo--];
    }
    return pec;
}

// ---------- Função de exibição ----------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n=== Estado atual ===\n");
    printf("Fila de peças:\t");
    int i, idx = f->inicio;
    for (i = 0; i < f->total; i++) {
        printf("[%c %d] ", f->pecas[idx].nome, f->pecas[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }

    printf("\nPilha de reserva (Topo -> Base): ");
    for (i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }
    printf("\n=====================\n");
}

// ---------- Trocas avançadas ----------
void trocarPecaAtual(Fila *f, Pilha *p) {
    if (!filaVazia(f) && !pilhaVazia(p)) {
        Peca temp = f->pecas[f->inicio];
        f->pecas[f->inicio] = p->pecas[p->topo];
        p->pecas[p->topo] = temp;
        printf("\nAção: Troca entre frente da fila e topo da pilha realizada!\n");
    } else {
        printf("\nNão há peças suficientes para realizar a troca!\n");
    }
}

void trocaMultipla(Fila *f, Pilha *p) {
    if (f->total >= 3 && p->topo >= 2) {
        for (int i = 0; i < 3; i++) {
            int idxFila = (f->inicio + i) % TAM_FILA;
            Peca temp = f->pecas[idxFila];
            f->pecas[idxFila] = p->pecas[p->topo - i];
            p->pecas[p->topo - i] = temp;
        }
        printf("\nAção: Troca múltipla entre as 3 primeiras peças realizada!\n");
    } else {
        printf("\nNão há peças suficientes em ambas as estruturas para trocar!\n");
    }
}

// ---------- Menu principal ----------
int main() {
    srand(time(NULL));
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;
    do {
        exibirEstado(&fila, &pilha);
        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar peça atual (fila <-> pilha)\n");
        printf("5 - Troca múltipla (3 peças)\n");
        printf("0 - Sair\n");
        printf("Opção escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca());
                } else {
                    printf("\nFila vazia!\n");
                }
                break;
            }
            case 2: {
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca p = desenfileirar(&fila);
                    empilhar(&pilha, p);
                    printf("\nPeça reservada: [%c %d]\n", p.nome, p.id);
                    enfileirar(&fila, gerarPeca());
                } else {
                    printf("\nNão foi possível reservar (fila vazia ou pilha cheia)!\n");
                }
                break;
            }
            case 3: {
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("\nPeça usada da reserva: [%c %d]\n", usada.nome, usada.id);
                } else {
                    printf("\nPilha vazia!\n");
                }
                break;
            }
            case 4:
                trocarPecaAtual(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("\nOpção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
