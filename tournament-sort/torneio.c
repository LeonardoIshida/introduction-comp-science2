#include "torneio.h"

struct aluno {
    char *nome;
    int *notas;
    double media;//usado no torneio sort, eventualmente sera -1
    double media_desempate;//usado nas comparacoes de desempate e maior media
    int pos_torneio;
};

/*cria um vetor de ponteiros para struct aluno, com valores flag*/
ALUNO **cria_lista(int qtd_alunos) {
    ALUNO **a = (ALUNO **)(malloc(sizeof(ALUNO *) * qtd_alunos));
    assert(a != NULL);

    for (int i = 0; i < qtd_alunos; i++) {
        a[i] = (ALUNO *)malloc(sizeof(ALUNO));
        assert(a[i] != NULL);

        a[i]->nome = NULL;
        a[i]->notas = NULL;
    }

    return a;
}

/*le o nome e retorna do endereco do nome de cada aluno*/
char *read_name(FILE *arq_in) {
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
        if (aux == ' ' || aux == EOF) {
            str[n_char] = '\0';
            break;
        }

        str[n_char++] = aux;

    } while (!feof(arq_in));

    str = realloc(str, sizeof(char) * (n_char + 1));

    return str;
}

/*le as notas dos alunos e retorna do endereco do vetor de notas*/
int *read_notas(int qtd_provas) {
    int *notas = (int *)malloc(sizeof(int) * qtd_provas);
    assert(notas != NULL);

    for (int i = 0; i < qtd_provas; i++) 
        scanf("%d ", &notas[i]);
    
    return notas;
}

void calcula_media(ALUNO *a, int qtd_provas) {
    assert(a != NULL);
    double cont = 0;

    for (int i = 0; i < qtd_provas; i++) {
        cont += (double)a->notas[i];
    }

    a->media = cont/qtd_provas;
    a->media_desempate = cont/qtd_provas;
}

//alocando dados do input
void read_input(ALUNO **a, int qtd_alunos, int qtd_provas) {
    assert(a != NULL);

    for (int i = 0; i < qtd_alunos; i++) {
        a[i]->nome = read_name(stdin);
        a[i]->notas = read_notas(qtd_provas);
        calcula_media(a[i], qtd_provas);
    }
}

void print_debug(ALUNO **a, int qtd_alunos, int qtd_provas) {
    for (int i = 0; i < qtd_alunos; i++) {
        if (a[i] == NULL) continue;

        printf("nome : %s\nnotas: ", a[i]->nome);

        for (int j = 0; j < qtd_provas; j++) {
            printf("%d ", a[i]->notas[j]);
        }

        printf("\nmedia: %lf\n", a[i]->media);
    }
}

/*encontra a potencia de 2 que eh maior que a qtd de alunos*/
int maior_pot2(int qtd_alunos) {
    int pot = 1;
    while (qtd_alunos/pot > 0)
        pot = pot * 2; 
    
    return pot;
}

/*cria vetor de ponteiros para a struct aluno de tamanho 2k no formato de torneio*/
ALUNO **cria_torneio(ALUNO **a, int qtd_alunos, int qtd_provas) {
    int pos = maior_pot2(qtd_alunos);
    int tam = pos;

    ALUNO **torneio = (ALUNO **)malloc(sizeof(ALUNO *) * tam * 2);
    assert(torneio != NULL);

    int contador = 0;
    //inserindo cada aluno nas posicoes mais baixas do torneio
    while (contador < qtd_alunos) {
        a[contador]->pos_torneio = pos;
        torneio[pos] = a[contador];

        pos++; contador++;
    }

    //setando para NULL as posicoes do torneio que nao tem alunos
    while (contador < tam) {
        torneio[pos++] = NULL;
        contador++;
    }

    //selecionando e inserindo os alunos com prioridade nas posicoes
    //de menor indices no torneio 
    while (tam >= 1){
        separa2(torneio, tam, (tam*2)-1, qtd_provas);
        tam /= 2;
    }

    return torneio;
}

/*retorna 1 se o da esq for maior ou 0 se o da direita for maior*/
int compara(ALUNO **a, int esq, int dir, int qtd_provas) {
    if (a[dir] == NULL)//essa posicao na lista nao existe
        return 1;

    //comparando a mesma pessoa, no torneio sort se for a mesma pessoa na comparacao
    //tenho que atualizar o campeonato, pois essa ja foi ordenada
    if (strcmp(a[esq]->nome, a[dir]->nome) == 0) 
        return 0;

    if (a[esq]->media > a[dir]->media) //esq maior
        return 1;

    //empate na media
    else if (a[esq]->media == a[dir]->media) {

        for (int i = 0; i < qtd_provas; i++) {
            if (a[esq]->notas[i] == a[dir]->notas[i])//mesma nota na msm prova
                continue;

            if (a[esq]->notas[i] > a[dir]->notas[i])//nota do da esq maior
                return 1;
            
            else //nota do da direita maior
                return 0;
        }
    }
    else //direita maior
        return 0;
}

/*quebra a lista ate sobrarem dois elementos, depois seleciona o maior*/
void separa2(ALUNO **a, int ini, int fim, int qtd_provas) {
    if (ini >= fim)
        return;

    if (fim - ini == 1) {//caso base onde tem 2 elementos na lista

        if (compara(a, ini, fim, qtd_provas) == 1) //esquerda maior
            a[ini/2] = a[ini];
        
        else //direita maior
            a[ini/2] = a[fim];

        return;
    }

    int meio = (ini + fim) / 2;
    separa2(a, ini, meio, qtd_provas);
    separa2(a, meio+1, fim, qtd_provas);
}

void torneio_sort(ALUNO **torneio, ALUNO **a, int qtd_alunos, int qtd_provas) {
    int pos = maior_pot2(qtd_alunos);
    int n = 0;

    while (n < qtd_alunos) {
        torneio[1]->media = -1;//alterando a media do 1 colocado do torneio
        a[n] = torneio[1];//inserindo na posicao mais a esq da lista original

        atualiza_torneio(torneio, torneio[1]->pos_torneio, qtd_provas); 
        n++;   
    }
}

void atualiza_torneio(ALUNO **torneio, int filho, int qtd_provas) {
    assert(torneio != NULL);

    int pai = filho / 2;
    if (pai == 0) {//ultrapassei a posicao 1, a maior do torneio
        return;
    }
        
    //caso o filho seja o da direta(indice impar), tenho que tratar esse caso
    //para nao acessar posicao inexistente na minha lista
    if (filho % 2 == 1)
        filho = filho - 1;

    //comparando para ver se algum filho tem prioridade sobre o pai
    if (compara(torneio, pai, filho, qtd_provas) == 0 || (torneio[filho+1] != NULL && 
    compara(torneio, pai, filho+1, qtd_provas) == 0)) {

        //verificando se o filho da direita tem prioridade sobre o da esq
        if (compara(torneio, filho, filho+1, qtd_provas) == 0) {
            filho = filho + 1;
        }

        //inserindo o filho na posicao pai, ou seja,
        //atualizando o torneio na parte superior
        torneio[pai] = torneio[filho];

        //chamada recursiva para ir atualizando ate a posicao 1
        atualiza_torneio(torneio, pai, qtd_provas);
    }
}

void print_maior_media(ALUNO **a, int qtd_alunos, int qtd_provas) {
    double maior = -10;

    for (int i = 0; i < qtd_alunos; i++) 
        if (a[i]->media_desempate > maior)//atualizando a maior media
            maior = a[i]->media_desempate;

    printf("Maior media: %.3lf\n", maior);
}

void print_resposta(ALUNO **a, int qtd_alunos, int qtd_provas) {
    int i;
    for (i = 0; i < qtd_alunos-1; i++) {
        printf("%d. %s", i+1, a[i]->nome);

        //media maior do elemento de menor indice
        if (a[i]->media_desempate > a[i+1]->media_desempate) {
            printf(" - media\n");
            continue;
        }
        //mesma media
        for (int j = 0; j < qtd_provas; j++) {
            //comparando as notas
            if (a[i]->notas[j] > a[i+1]->notas[j]) {
                printf(" - desempate: nota %d\n", j+1);
                break;
            }
        }
    }
    printf("%d. %s\n", i+1, a[i]->nome);
}

void desaloca(ALUNO **a, int qtd_alunos) {
    for (int i = 0; i < qtd_alunos; i++) {
        free(a[i]->nome);
        free(a[i]->notas);
        free(a[i]);
        a[i] = NULL;
    }
    free(a);
    a = NULL;
}