/*Construa um programa que, a partir de 3 números inteiros A, B, C e uma seed S para geração de números
inteiros pseudoaleatórios, utilize das funcionalidades do OpenMP para calcular a matriz resultante de uma
multiplicação de duas matrizes, de tamanhos A x B e B x C, compostas de números inteiros no intervalo [0, 9],
gerados aleatoriamente a partir da seed S. Escolha o número de threads entre 5 e 10.

Após realizar a multiplicação, imprima a matriz resultante, de dimensões A x C, separando cada coluna por
um único espaço, e cada linha por uma nova linha. Realize a impressão da matriz de forma que não haja
espaços sobrando ao final de cada linha.*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(){
    //Entradas
    int A, B, C, seed;
    scanf("%d %d %d %d", &A, &B, &C, &seed);

    //Alocar memoria
    int **m1 = (int**)malloc(sizeof(int*)*A);
    for(int i = 0; i < A; i++){
        m1[i] = (int*)malloc(sizeof(int)*B);
    }

    int **m2 = (int**)malloc(sizeof(int*)*B);
    for(int i = 0; i < B; i++){
        m2[i] = (int*)malloc(sizeof(int)*C);
    }

    int **m = (int**)malloc(sizeof(int*)*A);
    for(int i = 0; i < A; i++){
        m[i] = (int*)malloc(sizeof(int)*C);
    }

    //Montar matrizes
    srand(seed);

    for(int i = 0; i < A; i++){
        for(int j = 0; j < B; j++){
            m1[i][j] = rand()%10;
        }
    }

    for(int i = 0; i < B; i++){
        for(int j = 0; j < C; j++){
            m2[i][j] = rand()%10;
        }
    }

    //Multiplicacao
    #pragma omp parallel for collapse(2) num_threads(5)
    for(int i = 0; i < A; i++){
        for(int j = 0; j < C; j++){
            int sum = 0;
            #pragma for reduction(+:sum)
            for(int k = 0; k < B; k++){
                sum += m1[i][k] * m2[k][j];
            }
            m[i][j] = sum;
        }
    }

    for(int i = 0; i < A-1; i++){
        for(int j = 0; j < C-1; j++){
            printf("%d ", m[i][j]);
        }
        printf("%d", m[i][C-1]);
        printf("\n");
    }
    for(int j = 0; j < C-1; j++){
        printf("%d ", m[A-1][j]);
    }
    printf("%d", m[A-1][C-1]);

    //Liberar memoria
    for(int i = 0; i < A; i++){
        free(m1[i]);
    }
    free(m1);

    for(int i = 0; i < B; i++){
        free(m2[i]);
    }
    free(m2);

    for(int i = 0; i < A; i++){
        free(m[i]);
    }
    free(m);
}