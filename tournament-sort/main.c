/*
*   SCC0201 - Introdução à Ciência de Computação II
*
*   Trabalho Prático 2 - Tabela de notas
*
*   Nome: Leonardo Ishida   nUSP: 12873424
*/

#include "torneio.h"

int main() {
    int qtd_alunos; scanf("%d ", &qtd_alunos);
    int qtd_provas; scanf("%d ", &qtd_provas);

    ALUNO **a = cria_lista(qtd_alunos);
    read_input(a, qtd_alunos, qtd_provas);

    ALUNO **t = cria_torneio(a, qtd_alunos, qtd_provas);
    torneio_sort(t, a, qtd_alunos, qtd_provas);

    print_maior_media(a, qtd_alunos, qtd_provas);
    print_resposta(a, qtd_alunos, qtd_provas);

    desaloca(a, qtd_alunos);
    desaloca(t, 0);

    return 0;
}