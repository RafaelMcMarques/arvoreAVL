#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

int main() {
    struct nodo* raiz = NULL;
    char operacao[2];
    int valor;
    
    imprimirDadosAlunos();

    while (1) {
        scanf("%s", operacao);

        if (strcmp(operacao, "i") == 0) {
            scanf("%d", &valor);
            inserir(&raiz, valor);
        } else if (strcmp(operacao, "r") == 0) {
            scanf("%d", &valor);
            excluir(&raiz, valor);
        } else if (strcmp(operacao, "b") == 0) {
            scanf("%d", &valor);
            if (buscar(raiz, valor) != NULL) {
                printf("Encontrado %d.\n", valor);
            } else {
                printf("Nao encontrado %d.\n",valor);
            }
        } else if (strcmp(operacao, "e") == 0) {
            imprimirEmOrdem(raiz);
        } else if (strcmp(operacao, "l") == 0) {
            imprimirEmLargura(raiz);
        } else if (strcmp(operacao, "f") == 0) {
            liberaArvore(raiz);
            break;
        } else {
            printf("Operação inválida\n");
        }
    }

    return 0;
}
