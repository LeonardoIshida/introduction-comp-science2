#include "hash.h"

#define qtd_provas 4

struct hash_table {
    ALUNO **arr_aluno;
    int hash_table_len;
};

struct aluno {
    char *nusp;
    double *notas;
    unsigned long int senha_hash;
    ALUNO *next;
};

H_TABLE *h_table_criar(int qtd_alunos) {
    if (qtd_alunos <= 0) return NULL;

    H_TABLE *ht = (H_TABLE *)malloc(sizeof(H_TABLE));
    assert(ht != NULL);

    ht->hash_table_len = qtd_alunos * 2;

    ht->arr_aluno = (ALUNO **)malloc(sizeof(ALUNO *) * ht->hash_table_len);
    assert(ht->arr_aluno != NULL);

    for (int i = 0; i < ht->hash_table_len; i++) {
        ht->arr_aluno[i] = NULL;
    }

    return ht;
}

char *read_string(FILE *arq_in) {
    int size = 25;
    int n_char = 0;
    char aux;
    char *str = (char *)malloc(sizeof(char) * size);

    do {
        //alocando mais espaco caso necessario
        if(n_char == size) {
            size *= 2;
            str = realloc(str, sizeof(char) * size);
        }

        //lendo caracter
        aux = fgetc(arq_in);

        //fazendo as verificacoes de quebra de linha e inserindo \0
        if (aux == '\n') {
            str[n_char - 1] = '\0';
            break;
        }

        //fazendo as verificacoes de espaco e eof para inserir \0
        if (aux == EOF || aux == ' ') {
            str[n_char] = '\0';
            break;
        }

        str[n_char++] = aux;

    } while (!feof(arq_in));

    str = realloc(str, sizeof(char) * (n_char + 1));

    return str;
}

double *read_notas() {
    double *aux = (double *)malloc(sizeof(double) * qtd_provas);
    assert(aux != NULL);

    for (int i = 0; i < qtd_provas; i++) {
        scanf("%lf ", &aux[i]);
    }

    return aux;
}

// funcao que gera o hash de acordo com a especificacao pedida
unsigned long int hash_function(char *str) {
    unsigned int low = 1; 
    unsigned int high = 0;
    int i = 0;

    while (str[i] != '\0') {
        low = (low + str[i]) % 65521;
        high = (high + low) % 65521;
        i += 1;
    }

    unsigned long int hash = 0;
    hash = high << 16;
    hash += low;
    
    return hash;
}

// cria e seta as informacoes do aluno
ALUNO *aluno_set(char *nusp, char *senha, double *notas) {
    ALUNO *a = (ALUNO *)malloc(sizeof(ALUNO));
    assert(a != NULL);

    a->senha_hash = hash_function(senha);
    a->nusp = nusp;
    a->notas = notas;
    a->next = NULL;

    return a;
}

// insere na hash table se nao for repetido
void ht_insere(H_TABLE *ht, char *nusp, char *senha, double *notas) {
    if (ht == NULL) {
        //informacoes nao serao utilizadas
        free(nusp);
        free(senha);
        free(notas);

        return;
    }

    //encontrando o hash
    long unsigned int hash_index = hash_function(nusp) % ht->hash_table_len;
    
    if (ht->arr_aluno[hash_index] == NULL) {//local vazio para ser inserido, sem colisoes
        ht->arr_aluno[hash_index] = aluno_set(nusp, senha, notas);
        printf("Cadastro efetuado com sucesso\n");
    }
    else { //lidando com colisoes
        ALUNO *aux = ht->arr_aluno[hash_index];
        ALUNO *anterior = NULL;

        while (aux != NULL) {
            if (strcmp(aux->nusp, nusp) == 0) {//aluno ja cadastrado
                printf("NUSP ja cadastrado\n");

                //desalocando as informacoes nao uteis
                free(notas);
                free(senha);
                free(nusp);

                return;
            }

            anterior = aux;
            aux = aux->next;

        }//temimei o loop e nao achei nusp

        //cadastrando aluno novo
        aux = aluno_set(nusp, senha, notas);
        anterior->next = aux;
        printf("Cadastro efetuado com sucesso\n");
    }
}

void aluno_print_notas(ALUNO *a) {
    printf("Notas: "); 
    printf("P1=%.1lf, ", a->notas[0]);
    printf("P2=%.1lf, ", a->notas[1]);
    printf("T1=%.1lf, ", a->notas[2]);
    printf("T2=%.1lf\n", a->notas[3]);
}

//buscando um aluno na hash table
void ht_search(H_TABLE *ht, char *nusp, char *senha) {
    if (ht == NULL) {
        printf("NUSP invalido\n");
        return;
    }

    //encontrando o indice a partir do nusp
    long unsigned int hash_index = hash_function(nusp) % ht->hash_table_len;
    
    //tentando acessar um indice sem ter um aluno registrado
    if (ht->arr_aluno[hash_index] == NULL) {
        printf("NUSP invalido\n");
        return;
    }

    //encontrando a senha codificada pelo hash
    unsigned long int senha_search = hash_function(senha);

    //percorrendo a lista encadeada no indice obtido pela funcao hash
    ALUNO *aux = ht->arr_aluno[hash_index];
    while (aux != NULL) {
        if (strcmp(aux->nusp, nusp) == 0) {//achei o nusp

            if (senha_search == aux->senha_hash) {//nusp com senha correta
                aluno_print_notas(aux);
            }
            else {//nusp com senha incorreta
                printf("Senha incorreta para o NUSP digitado\n");
            }

            return;
        }

        aux = aux->next;
    }

    printf("NUSP invalido\n");//nao achei o nusp que procurei
}

void aluno_desalocar(ALUNO **a) {
    ALUNO **aux = a;
    ALUNO **ant = NULL;

    while (*aux != NULL) {//desalocando todos os nos da lista encadeada
        free((*aux)->nusp);
        free((*aux)->notas);

        (*aux)->nusp = NULL;
        (*aux)->notas = NULL;

        ant = aux;
        aux = &((*aux)->next);

        free(*ant);
        *ant = NULL;
    }
}

void ht_desalocar(H_TABLE **ht) {
    if (*ht == NULL) return;

    int len = (*ht)->hash_table_len;
    ALUNO **aux = NULL;

    for (int i = 0; i < len; i++) {
        aux = &((*ht)->arr_aluno[i]);

        if (*aux != NULL) {//existe aluno alocado nesse index
            aluno_desalocar(aux);
        }
        else {//nao existe aluno alocado nesse index
            free(*aux);
            *aux = NULL;
        }
    }
    
    free((*ht)->arr_aluno);
    free(*ht);
    *ht = NULL;
}

// funcao para debug
void ht_print_debug(H_TABLE *ht) {
    ALUNO *aux = NULL;
    ALUNO *tmp = NULL;
    for (int i = 0; i < ht->hash_table_len; i++) {
        aux = ht->arr_aluno[i];
        tmp = aux;
        while (tmp != NULL) {
            printf("{nusp : %s | senha: %lu | index : %d}\n", tmp->nusp, tmp->senha_hash, i);
            tmp = tmp->next;
        }
    }
}