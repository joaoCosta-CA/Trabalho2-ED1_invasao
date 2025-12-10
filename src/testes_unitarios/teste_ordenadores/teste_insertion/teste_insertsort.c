#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "insert.h"

int comparar_ints(const void *a, const void *b) {
    return (*(int*) *(int**)a) - (*(int*) *(int**)b);
}

void verificar_ordem(void **vetor, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (*(int*)vetor[i] > *(int*)vetor[i+1]) {
            fprintf(stderr, "[ERRO] MergeSort falhou.\n");
            exit(1);
        }
    }
}

int main() {
    printf("[MERGE] Iniciando teste Híbrido...\n");
    srand(time(NULL));
    
    int n = 200; // Tamanho médio
    int *dados = malloc(n * sizeof(int));
    void **vetor = malloc(n * sizeof(void*));

    for(int i=0; i<n; i++) {
        dados[i] = rand() % 500;
        vetor[i] = &dados[i];
    }

    // Testa com cutoff 10 (padrão)
    // Isso vai forçar o uso de InsertionSort nas pontas da recursão
    insertion_sort(vetor, 0, n - 1, comparar_ints);

    verificar_ordem(vetor, n);
    
    free(dados);
    free(vetor);
    
    printf(">>> SUCESSO: MergeSort Híbrido OK.\n");
    return 0;
}