#ifndef _TORNEIO_H
#define _TORNEIO_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct aluno ALUNO;

char *read_name(FILE *arq_in);
int *read_notas(int qtd_provas);
void calcula_media(ALUNO *a, int qtd_provas);
void print_debug(ALUNO **a, int qtd_alunos, int qtd_provas);
void read_input(ALUNO **a, int qtd_alunos, int qtd_provas);
int maior_pot2(int qtd_alunos);
ALUNO **cria_lista(int qtd_alunos);
ALUNO **cria_torneio(ALUNO **a, int qtd_alunos, int qtd_provas);
void separa2(ALUNO **a, int ini, int fim, int qtd_provas);
int compara(ALUNO **a, int esq, int dir, int qtd_provas);
void torneio_sort(ALUNO **torneio, ALUNO **a, int qtd_alunos, int qtd_provas);
void atualiza_torneio(ALUNO **torneio, int filho, int qtd_provas);
void print_maior_media(ALUNO **a, int qtd_alunos, int qtd_provas);
void bubble(ALUNO **a, int alu);
void print_resposta(ALUNO **a, int qtd_alunos, int qtd_provas);
void desaloca(ALUNO **a, int qtd_alunos);

#endif