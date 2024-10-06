/*considere uma matriz quadrada A de ordem Ne com
números inteiros não negativos, cujos valores são determinados pseudo- aleatoriamente no
futuro código-fonte entre 0 e 99. Calcule a média de todos os valores da matriz A e então
determine dois outros valores: (i) quantos elementos menores que esta média existem em
cada coluna j de A, e (ii) o desvio padrão populacional de cada coluna j

Insira os
resultados de (i) nas posições j de vetor medial com N posições. Insira os resultados de (ii) nas
posições j do vetor dsvpdr[] com N posições. As saidas da aplicação. como exemplificado
abaixo, devem ser o vetor medial [] e depois o vetor dsvpdr[].*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 10000
#define T 4

int main(){
    srand(1000);
    
    int **matriz = (int**)malloc(sizeof(int*) * N);
    for(int i = 0; i < N; i++){
        matriz[i] = (int *)malloc(sizeof(int) * N);
    }
        
    for(int i = 0; i < N; i++){    
        for(int j = 0; j < N; j++){
            matriz[i][j] = rand()%100;
        }
    }

    float *medial = (float*)malloc(sizeof(float*)*N);
    float *dp = (float*)malloc(sizeof(float*)*N);

    float time = omp_get_wtime();

    float media_global = 0.0;
    #pragma omp parallel for collapse(2) reduction(+:media_global) num_threads(T)
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            media_global += matriz[i][j];
        }
    }
    media_global /= (N*N);

    int count;
    #pragma omp parallel for private(count) num_threads(T)
    for(int j = 0; j < N; j++){
        count = 0;
        
        #pragma omp parallel for reduction(+:count)
        for(int i = 0; i < N; i++){
            if(matriz[i][j] < media_global){
                count++;
            }
        }

        medial[j] = count; 
    }

    float media_col;

    #pragma omp parallel for private(media_col) num_threads(T)
    for(int j = 0; j < N; j++){
        media_col = 0;

        #pragma omp simd reduction(+:media_col)
        for(int i = 0; i < N; i++){
            media_col += matriz[i][j];
        }
        media_col /= N;

        int sum = 0;
        #pragma omp simd reduction(+:sum)
        for(int i = 0; i < N; i++){
            sum += pow((matriz[i][j] - media_col), 2);
        }
        dp[j] = sqrt(sum/N);
    }

    for(int j = 0; j < N; j++){
        printf("media[%d] = %f dp[%d] = %f\n", j, medial[j], j, dp[j]);
    }

    printf("%f\n", omp_get_wtime() - time);

    return 0;
}



