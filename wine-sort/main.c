/*
*   SCC0201 - Introdução à Ciência de Computação II
*
*   Trabalho Prático 1 - Catalogando Vinhos
*
*   Nome: Leonardo Ishida   nUSP: 12873424
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "vinho.h"


int main() {
    char *vinho_arq_name = readline(stdin);
    assert(vinho_arq_name != NULL);

    FILE *vinho_arq = fopen(vinho_arq_name, "r");
    assert(vinho_arq != NULL);

    int vinhos_len;
    VINHO *vinhos = readfile(vinho_arq, &vinhos_len);

    int qtd_buscas; scanf("%d ", &qtd_buscas);
    double key;
    char qual[20];

    for (int i = 0; i < qtd_buscas; i++) {
        scanf("%s %lf ", qual, &key);
        int total = 0;

        vinho_sort(vinhos, vinhos_len, qual);
        int id = busca(vinhos, qual, key, 0, vinhos_len-1, &total);
        vinho_print(vinhos, id, total);
    }

    fclose(vinho_arq);
    free(vinho_arq_name);
    free(vinhos);

    return 0;
}


