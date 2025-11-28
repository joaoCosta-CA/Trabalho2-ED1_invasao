#include <stdlib.h>
#include "qsort.h"

void quick_sort(void **vetor, int n, Comparador cmp) {
    // qsort ordena o vetor de ponteiros (void*), logo o tamanho é sizeof(void*)
    qsort(vetor, n, sizeof(void*), cmp);
}