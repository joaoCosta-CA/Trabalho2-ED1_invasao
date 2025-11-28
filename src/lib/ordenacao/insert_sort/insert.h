#ifndef INSERT_SORT_H
#define INSERT_SORT_H

#include "ordenacao_types.h"

/* Ordena o intervalo [l, r] do vetor usando Insertion Sort */
void insertion_sort(void **vetor, int l, int r, Comparador cmp);

#endif