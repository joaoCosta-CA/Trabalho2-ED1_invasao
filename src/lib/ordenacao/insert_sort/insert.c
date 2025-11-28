#include "insert.h"

void insertion_sort(void **vetor, int l, int r, Comparador cmp) {
    for (int i = l + 1; i <= r; i++) {
        void *aux = vetor[i];
        int j = i - 1;
        
        // cmp recebe os endereços dos ponteiros
        while (j >= l && cmp(&vetor[j], &aux) > 0) {
            vetor[j + 1] = vetor[j];
            j--;
        }
        vetor[j + 1] = aux;
    }
}