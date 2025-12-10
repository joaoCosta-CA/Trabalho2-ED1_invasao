#include "qsort.h"
#include "merge.h"

#include <stdio.h>

void ordenar_dados(void **vetor, int tamanho, char tipo_ord, int cutoff, Comparador cmp) {
    if (tipo_ord == 'm') {
        // Usa MergeSort (que usa o cutoff para chamar InsertionSort internamente)
        merge_sort(vetor, tamanho, cutoff, cmp);
    } else if (tipo_ord == 'q') {
        // Usa QuickSort Padrão (qsort)
        quick_sort(vetor, tamanho, cmp);
    } else {
        printf("Tipo de ordenação desconhecido. Usando QuickSort.\n");
        quick_sort(vetor, tamanho, cmp);
    }
}