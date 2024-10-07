/*Desenvolver uma versão paralela do algoritmo para encontrar o maior elemento em um vetor, 
seguindo as orientações abaixo. Como entrada, seu programa receberá um valor N, indicando o 
tamanho do vetor.

Gere os números usando o valor 1 para todos os elementos e em seguida substitua o valor da posição 
do meio do vetor (N/2) pelo valor de N;
Paralelize a geração dos números no vetor e também a busca do maior valor;
O seu programa deve imprimir na tela o maior valor encontrado no vetor, como feito em sala de aula.
Considere também as seguintes instruções adicionais:

- Não use a diretiva for do OpenMP (não confundir com o for do C);
- Calcule os limites das iterações dos loops de cada thread no seu código explicitamente;
- Sincronize com uma diretiva barrier, entre a geração dos números e a busca do maior valor;
- Não use a cláusula reduction, porém, sim, otimize o uso da memória compartilhada de outra forma;
- Na paralelização dos for’s, distribua as iterações estáticamente por blocos contínuos:

- Thread 00 – gera elems do bloco 0 (0 até N/num_threads);
- Thread 01 – gera elems do bloco 1 (N/numthreads até (N/numthreads)*2);
E assim por diante.*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(){
    int N;
    scanf("%d", &N);

    int *vetor = (int*)malloc(sizeof(int)*N); 
    int num_threads;

    #pragma omp parallel
    {
        num_threads = omp_get_num_threads();
    }

    int *maiores = (int*)malloc(sizeof(int)*num_threads);

    #pragma omp parallel
    {
        int thread = omp_get_thread_num();
        int ini = thread * (N / num_threads);
        int fim = (thread == num_threads-1) ? N : (thread + 1)*(N / num_threads);
    
        for(int i = ini; i < fim; i++){
            vetor[i] = 1;
        }
    
        #pragma omp barrier

        #pragma omp single
        {
            vetor[N/2] = N;
        }
    
        #pragma omp barrier

        int maior_local = vetor[ini];
        for(int i = ini+1; i < fim; i++){
            if(vetor[i] > maior_local){
                maior_local = vetor[i];
            }
        }

        maiores[thread] = maior_local;
    }

    int maior = maiores[0];
    for(int i = 1; i < num_threads; i++){
        if(maiores[i] > maior){
            maior = maiores[i];
        }
    }

    printf("%d", maior);

    return 0;
}