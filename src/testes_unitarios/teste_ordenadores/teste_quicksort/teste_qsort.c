#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "qsort.h"

/* --- Helpers (Copiados para manter o teste independente) --- */
int comparar_ints(const void *a, const void *b) {
    int *pa = *(int**)a;
    int *pb = *(int**)b;
    return (*pa) - (*pb);
}

void verificar_ordem(void **vetor, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (*(int*)vetor[i] > *(int*)vetor[i+1]) {
            fprintf(stderr, "[ERRO] QuickSort falhou no índice %d\n", i);
            exit(1);
        }
    }
}

int main() {
    printf("[QUICK] Iniciando teste...\n");
    srand(time(NULL));
    
    int n = 1000;
    int *dados = malloc(n * sizeof(int));
    void **vetor = malloc(n * sizeof(void*));

    // Popula com aleatórios
    for(int i=0; i<n; i++) {
        dados[i] = rand() % 10000;
        vetor[i] = &dados[i];
    }

    // Executa
    quick_sort(vetor, n, comparar_ints);

    // Valida
    verificar_ordem(vetor, n);
    
    free(dados);
    free(vetor);
    
    printf(">>> SUCESSO: QuickSort ordenou %d elementos corretamente.\n", n);
    return 0;
}