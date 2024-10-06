/*Construa um programa que, a partir de um número N e uma seed S para geração de números inteiros pseudoaleatórios, 
utilize das funcionalidades do OpenMP para determiner, em uma matriz N x N composta por números inteiros no intervalo
[0, 99] gerados pela seed S, qual a maior diferença entre um dado número desta matriz em relação aos seus (até 8)
vizinhos mais próximos. Como saída dessa aplicação, imprima estes dois números da matriz. Imprima na mesma linha
primeiro o maior valor e depois o menor valor. Os valores limites de linhas e colunas não consideram filas circulares,
i.e., o valor na posição [0,0] da matriz não é comparado, por exemplo, com os valores [0,(N-1)] e [(N-1),0].

Considere a matriz 4 x 4 abaixo como exemplo, sem perda de generalidade. Neste exemplo, a maior diferença está entre os
números destacados nas posições [2][1] e [1][1], sendo estes 94 e 1, respectivamente, resultando em uma diferença de 93.

8      2      30     10
5      1       2      0
10    94     50     75
7      15     35     15

Neste caso, a saída esperada é 94 1.

Atenção: para garantir a saída determinística do problema, a geração da matriz não precisa ser paralelizada.*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

int main(){
    int N;
    int seed;

    scanf("%d %d", &N, &seed);

    srand(seed);

    //criacao da matriz
    int **matriz = (int**)malloc(sizeof(int*)*N);
    for(int i = 0; i < N; i++){
        matriz[i] = (int*)malloc(sizeof(int)*N);
    }

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            matriz[i][j] = rand()%100;
        }
    }

    int direcoes_vizinhos[8][2] = {{-1, -1},
                            {-1, 0}, 
                            {-1, 1}, 
                            {0, -1}, 
                            {0, 1}, 
                            {1, -1}, 
                            {1, 0}, 
                            {1, 1}};

    int max_value;
    int min_value;
    int diff = 0;

    #pragma omp parallel for collapse(2) shared(diff, max_value, min_value)
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            for(int d = 0; d < 8; d++){
                int i_vizinho = i + direcoes_vizinhos[d][0];
                int j_vizinho = j + direcoes_vizinhos[d][1];

                if(i_vizinho >= 0 && i_vizinho < N && j_vizinho >= 0 && j_vizinho < N){
                    int vizinho = matriz[i_vizinho][j_vizinho];
                    int central = matriz[i][j];
                    int aux_diff = abs(central - vizinho);
                    
                    if(aux_diff > diff){
                        #pragma omp critial
                        {
                            diff = aux_diff;
                            if (central > vizinho){
                                max_value = central;
                                min_value = vizinho;
                            } else {
                                max_value = vizinho;
                                min_value = central;
                            }
                        }
                    }
                }
            } 
        }
    }

    printf("%d %d\n", max_value, min_value);

    for (int i = 0; i < N; i++) {
        free(matriz[i]);
    }
    free(matriz);


    return 0;
}