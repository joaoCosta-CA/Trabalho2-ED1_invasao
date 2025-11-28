#ifndef QSORT_H
#define QSORT_H

#include "ordenacao_types.h"

/* Wrapper para o qsort da stdlib */
void quick_sort(void **vetor, int n, Comparador cmp);

#endif