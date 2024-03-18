#include "vinho.h"
#include <string.h>
#include <assert.h>

struct vinho {
    int id;
    double citric_acid;
    double residual_sugar;
    double density;
    double pH;
    double alcohol;
};

char *readline(FILE *arq_in) {
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
        if (aux == '\r')
            continue;
        if (aux == '\n' || aux == EOF) {
            str[n_char] = '\0';
            break;
        }

        str[n_char++] = aux;

    } while (!feof(arq_in));

    str = realloc(str, sizeof(char) * (n_char + 1));

    return str;
}

VINHO *readfile(FILE *arq_vinho, int *size) {
    VINHO *v_arr = NULL;
    int arr_size = 0;
    int index = 0;
    char *str_tok_aux = NULL;
    char *str_file_line = NULL;
    char virgula[] = ",";

    //lendo a primeira linha, nao sera utilizado esses dados
    str_file_line = readline(arq_vinho);

    do {
        //alocando um vetor de struct VINHO
        arr_size++;
        v_arr = realloc(v_arr, sizeof(VINHO) * arr_size);

        str_file_line = readline(arq_vinho);

        //separando e convertendo a string para int id
        str_tok_aux = strtok(str_file_line, virgula);
        v_arr[index].id = atoi(str_tok_aux);

        //separando e convertendo a string para double citric_acid
        str_tok_aux = strtok(NULL, virgula);
        v_arr[index].citric_acid = atof(str_tok_aux);

        //separando e convertendo a string para double residual_sugar
        str_tok_aux = strtok(NULL, virgula);
        v_arr[index].residual_sugar = atof(str_tok_aux);

        //separando e convertendo a string para double density
        str_tok_aux = strtok(NULL, virgula);
        v_arr[index].density = atof(str_tok_aux);

        //separando e convertendo a string para double pH
        str_tok_aux = strtok(NULL, virgula);
        v_arr[index].pH = atof(str_tok_aux);

        //separando e convertendo a string para double alcohol
        str_tok_aux = strtok(NULL, virgula);
        v_arr[index].alcohol = atof(str_tok_aux);

        index++;

    } while (!feof(arq_vinho));

    *size = index;

    return v_arr;
}

double seleciona_qualidade(VINHO *arr, int index, char *qualidade) {
    //retorna o valor da qualidade dependendo da string passado como argumento
    if (strcmp(qualidade, "citric_acid") == 0)
        return arr[index].citric_acid;

    if (strcmp(qualidade, "density") == 0)
        return arr[index].density;

    if (strcmp(qualidade, "alcohol") == 0)
        return arr[index].alcohol;

    if (strcmp(qualidade, "pH") == 0)
        return arr[index].pH;

    if (strcmp(qualidade, "residual_sugar") == 0)
        return arr[index].residual_sugar;
}

void swap_vinho(VINHO *arr, int index_maior_qual, int final_arr) {
    VINHO tmp = arr[index_maior_qual];
    arr[index_maior_qual] = arr[final_arr];
    arr[final_arr] = tmp;
}

void vinho_sort(VINHO *arr, int size, char *qualidade) {
    double maior_qualidade;
    int maior_index;
    double qual_tmp;

    for (int i = size - 1; i > 0; i--) {
        //setando primeiro como aquele que possui a qualidade maior,
        //para que seja possivel comparar valores.
        maior_qualidade = seleciona_qualidade(arr, 0, qualidade);
        maior_index = 0;
        
        for (int j = 1; j < i+1; j++) {
            qual_tmp = seleciona_qualidade(arr, j, qualidade);
            if (qual_tmp >= maior_qualidade) {
                if (qual_tmp > maior_qualidade) {
                    maior_index = j;
                    maior_qualidade = qual_tmp;
                    continue;
                } else if (qual_tmp == maior_qualidade && arr[j].id > arr[maior_index].id) {
                    maior_index = j;
                    maior_qualidade = qual_tmp;
                }
            }
        }
        //inserindo o vinho com a qualidade maior na sua posicao
        swap_vinho(arr, maior_index, i);
    }
}

//funcao que conta os vinhos da esquerda e retorna o menor indice
int vinho_busca_esq(VINHO *arr, char *qualidade, double key, int index, int *total) {
    //inicializando i como index -1 para nao contar 2 vezes o mesmo vinho
    //ja que o index atual ja foi contado fazendo a busca na direita
    int index_tmp = index - 1;
    int qtd_vinhos = 0;
    
    while (seleciona_qualidade(arr, index_tmp, qualidade) ==  key && index_tmp >= 0){
        index_tmp--;
        qtd_vinhos++;
    }  
    *total += qtd_vinhos;

    //index_tmp passa a ser o menor index, mas deve-se adicionar 1, pois a condicional
    //no while pode ser falsa na primeira iteracao, logo, se nao adicionar 1
    //o index estaria errado
    return index_tmp + 1; 
}

void vinho_busca_dir(VINHO *arr, char *qualidade, double key, int index, int *total) {
    //funcao para encontrar todos os vinhos que podem estar a direita
    //do vinho encontrado pela busca binaria
    int qtd_vinhos = 0;
    while (seleciona_qualidade(arr, index, qualidade) ==  key) {
        qtd_vinhos++;
        index++;
    } 

    *total = qtd_vinhos;
}

int busca(VINHO *arr, char *qualidade, double key, int ini, int fim, int *total) {
    if (ini > fim)
        return -1;

    int meio = (ini + fim) / 2;
    
    double qualidade_meio = seleciona_qualidade(arr, meio, qualidade);

    if (qualidade_meio == key) {
        vinho_busca_dir(arr, qualidade, key, meio, total);
        //busca esq retorna o menor indice
        return vinho_busca_esq(arr, qualidade, key, meio, total);
    }

    if (key > qualidade_meio) {
        return busca(arr, qualidade, key, meio+1, fim, total);
    } else {
        return busca(arr, qualidade, key, ini, meio-1, total);
    }
    
}

void vinho_print(VINHO *vinho, int index, int total) {
    if (total == 0) {
        printf("Nenhum vinho encontrado\n");
        return;
    }

    printf("ID: %d, ", vinho[index].id);
    printf("Citric Acid: %.5lf, ", vinho[index].citric_acid);
    printf("Residual Sugar %.5lf, ", vinho[index].residual_sugar);
    printf("Density %.5lf, ", vinho[index].density);
    printf("pH %.5lf, ", vinho[index].pH);
    printf("Alcohol %.5lf\n", vinho[index].alcohol);
    printf("Total de vinhos encontrados: %d\n", total);
}