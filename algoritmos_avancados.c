#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// =======================================================
//                  NÍVEL NOVATO
//      Árvore binária representando as salas da mansão
// =======================================================
//

typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

Sala* criarSala(const char *nome) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    s->esq = s->dir = NULL;
    return s;
}

void explorarSalas(Sala *atual) {
    char opc;
    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);
        printf("[e] Ir para a esquerda\n[d] Ir para a direita\n[s] Sair\n> ");
        scanf(" %c", &opc);

        if (opc == 's') {
            printf("Exploração encerrada!\n");
            return;
        }

        if (opc == 'e') {
            if (atual->esq != NULL)
                atual = atual->esq;
            else {
                printf("Caminho à esquerda termina aqui!\n");
                return;
            }
        } else if (opc == 'd') {
            if (atual->dir != NULL)
                atual = atual->dir;
            else {
                printf("Caminho à direita termina aqui!\n");
                return;
            }
        } else {
            printf("Opção inválida!\n");
        }
    }
}

//
// =======================================================
//                  NÍVEL AVENTUREIRO
//       Árvore de busca (BST) para armazenar pistas
// =======================================================
//

typedef struct NoBST {
    char pista[50];
    struct NoBST *esq, *dir;
} NoBST;

NoBST* inserirBST(NoBST *raiz, const char *pista) {
    if (raiz == NULL) {
        NoBST *novo = (NoBST*) malloc(sizeof(NoBST));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirBST(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirBST(raiz->dir, pista);

    return raiz;
}

void imprimirBST(NoBST *raiz) {
    if (raiz == NULL) return;
    imprimirBST(raiz->esq);
    printf("- %s\n", raiz->pista);
    imprimirBST(raiz->dir);
}

//
// =======================================================
//                  NÍVEL MESTRE
//     Tabela hash relacionando pistas → suspeitos
// =======================================================
//

#define TAM_HASH 26

typedef struct HashNode {
    char pista[50];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

HashNode* tabela[TAM_HASH];

int hashFunc(const char *chave) {
    return (chave[0] % 26);
}

void inserirHash(const char *pista, const char *suspeito) {
    int idx = hashFunc(pista);

    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[idx];

    tabela[idx] = novo;
}

void listarHash() {
    printf("\n===== PISTAS E SUSPEITOS =====\n");
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode *aux = tabela[i];
        while (aux != NULL) {
            printf("Pista: %-15s → Suspeito: %s\n", aux->pista, aux->suspeito);
            aux = aux->prox;
        }
    }
}

char* suspeitoMaisCitado() {
    static char nome[50];
    int cont[100] = {0};
    char suspeitos[100][50];
    int qnt = 0;

    for (int i = 0; i < TAM_HASH; i++) {
        HashNode *aux = tabela[i];
        while (aux != NULL) {

            int encontrado = -1;
            for (int j = 0; j < qnt; j++) {
                if (strcmp(suspeitos[j], aux->suspeito) == 0) {
                    encontrado = j;
                    break;
                }
            }

            if (encontrado == -1) {
                strcpy(suspeitos[qnt], aux->suspeito);
                cont[qnt]++;
                qnt++;
            } else {
                cont[encontrado]++;
            }

            aux = aux->prox;
        }
    }

    int max = 0, pos = 0;
    for (int i = 0; i < qnt; i++) {
        if (cont[i] > max) {
            max = cont[i];
            pos = i;
        }
    }

    strcpy(nome, suspeitos[pos]);
    return nome;
}

//
// =======================================================
//                          MAIN
// =======================================================
//

int main() {
    // ---------------- NÍVEL NOVATO --------------------

    Sala *hall = criarSala("Hall de Entrada");
    hall->esq = criarSala("Biblioteca");
    hall->dir = criarSala("Sala de Jantar");

    hall->esq->esq = criarSala("Porão");
    hall->esq->dir = criarSala("Escritório");

    hall->dir->esq = criarSala("Cozinha");
    hall->dir->dir = criarSala("Jardim");

    // Exploração simples
    printf("==== EXPLORAÇÃO DA MANSÃO ====\n");
    explorarSalas(hall);

    // ---------------- NÍVEL AVENTUREIRO --------------------

    NoBST *pistas = NULL;

    printf("\nAdicionando pistas...\n");
    pistas = inserirBST(pistas, "Pegada");
    pistas = inserirBST(pistas, "Chave");
    pistas = inserirBST(pistas, "Luvas");
    pistas = inserirBST(pistas, "Cabelo");

    printf("\nPistas coletadas (ordem alfabética):\n");
    imprimirBST(pistas);

    // ---------------- NÍVEL MESTRE --------------------

    inserirHash("Pegada", "Mordomo");
    inserirHash("Chave", "Cozinheira");
    inserirHash("Cabelo", "Jardineiro");
    inserirHash("Luvas", "Mordomo");
    inserirHash("Pegada", "Mordomo");

    listarHash();

    printf("\nSuspeito mais citado: %s\n", suspeitoMaisCitado());

    return 0;
}
