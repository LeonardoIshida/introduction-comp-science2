#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct aluno ALUNO;
typedef struct hash_table H_TABLE;

char *read_string(FILE *arq_in);

double *read_notas();

unsigned long int hash_function(char *str);

H_TABLE *h_table_criar(int qtd_alunos);

void aluno_print_notas(ALUNO *a);

void ht_search(H_TABLE *ht, char *nusp, char *senha);

void ht_desalocar(H_TABLE **ht);

void aluno_desalocar(ALUNO **a);

void ht_print_debug(H_TABLE *ht);

void ht_insere(H_TABLE *ht, char *nusp, char *senha, double *notas);

ALUNO *aluno_set(char *nusp, char *senha, double *notas);

#endif