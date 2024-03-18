#ifndef _VINHO_H
    #define _VINHO_H

    #include <stdio.h>
    #include <stdlib.h>

    typedef struct vinho VINHO;

    char *readline(FILE *arq_in);

    VINHO *readfile(FILE *arq_vinho, int *size);

    void vinho_sort(VINHO *arr, int size, char *qualidade);

    double seleciona_qualidade(VINHO *arr, int index, char *qualidade);

    void swap_vinho(VINHO *arr, int index_maior_qual, int final_arr);

    void print(VINHO *arr, int size);

    int vinho_busca_esq(VINHO *arr, char *qualidade, double key, int index, int *total);

    void vinho_busca_dir(VINHO *arr, char *qualidade, double key, int index, int *total);

    int busca(VINHO *arr, char *qualidade, double key, int ini, int fim, int *total);

    void vinho_print(VINHO *vinho, int index, int total);

    void get_quali_e_valor(char *str, char *qualidade, double *valor);

#endif