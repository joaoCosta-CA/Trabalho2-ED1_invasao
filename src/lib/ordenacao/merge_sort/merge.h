#ifndef MERGE_H
#define MERGE_H

#include "ordenacao_types.h"

/* * Ordena usando MergeSort Híbrido.
 * cutoff: valor do parâmetro -in (limite para usar insertion sort)
 */
void merge_sort(void **vetor, int n, int cutoff, Comparador cmp);

#endif