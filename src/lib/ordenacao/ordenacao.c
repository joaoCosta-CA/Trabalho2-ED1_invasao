#include "qsort.h"
#include "merge.h"

#include <stdio.h>

void ordenar_dados(void **vetor, int tamanho, char tipo_ord, int cutoff, Comparador cmp) {
    if (tipo_ord == 'm') {
        // Usa MergeSort (que usa o cutoff para chamar InsertionSort internamente)
        // Requisito: "versão modificada do mergesort" [cite: 108, 109]
        merge_sort(vetor, tamanho, cutoff, cmp);
    } else if (tipo_ord == 'q') {
        // Usa QuickSort Padrão (qsort)
        // Requisito: "usar o qsort (da biblioteca padrão do C)" [cite: 108]
        quick_sort(vetor, tamanho, cmp);
    }else {
        printf("Tipo de ordenação desconhecido. Usando QuickSort Padrão.\n");
        quick_sort(vetor, tamanho, cmp);
    }
}