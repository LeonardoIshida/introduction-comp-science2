/*
*   SCC0201 - Introdução à Ciência de Computação II
*
*   Trabalho Prático 3 - Login
*
*   Nome: Leonardo Ishida   nUSP: 12873424
*/

#include "hash.h"

int main() {
    int qtd_alunos;
    scanf("%d ", &qtd_alunos);

    char *str_senha = NULL;
    char *str_nusp = NULL;
    double *arr_notas = NULL;

    H_TABLE *ht = h_table_criar(qtd_alunos);
    
    for (int i = 0; i < qtd_alunos; i++) {
        str_nusp = read_string(stdin);
        str_senha = read_string(stdin);
        arr_notas = read_notas();

        ht_insere(ht, str_nusp, str_senha, arr_notas);
    }

    int qtd_login;
    scanf("%d ", &qtd_login);
    
    for (int i = 0; i < qtd_login; i++) {
        str_nusp = read_string(stdin);
        str_senha = read_string(stdin);

        ht_search(ht, str_nusp, str_senha);

        free(str_nusp);
        free(str_senha);
    }

    ht_desalocar(&ht);

    return 0;
}