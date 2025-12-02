#include <stdlib.h>
#include "merge.h"
#include "insert.h"

/* Função auxiliar de intercalação */
static void merge(void **vetor, void **aux, int l, int m, int r, Comparador cmp) {
    for (int i = l; i <= r; i++) aux[i] = vetor[i];

    int i = l, j = m + 1, k = l;

    while (i <= m && j <= r) {
        if (cmp(&aux[i], &aux[j]) <= 0) vetor[k++] = aux[i++];
        else vetor[k++] = aux[j++];
    }

    while (i <= m) vetor[k++] = aux[i++];
}

/* Função recursiva interna */
static void m_sort_recursivo(void **vetor, void **aux, int l, int r, int cutoff, Comparador cmp) {
    // Se o tamanho do subvetor for menor ou igual ao cutoff, usa InsertionSort
    if (r - l + 1 <= cutoff) {
        insertion_sort(vetor, l, r, cmp);
        return;
    }

    int m = l + (r - l) / 2;
    m_sort_recursivo(vetor, aux, l, m, cutoff, cmp);
    m_sort_recursivo(vetor, aux, m + 1, r, cutoff, cmp);

    // Otimização: se já estiver ordenado, não faz merge
    if (cmp(&vetor[m], &vetor[m+1]) <= 0) return;

    merge(vetor, aux, l, m, r, cmp);
}

void merge_sort(void **vetor, int n, int cutoff, Comparador cmp) {
    if (n <= 1) return;
    
    // Aloca vetor auxiliar uma única vez
    void **aux = malloc(n * sizeof(void *));
    if (!aux) return; 

    m_sort_recursivo(vetor, aux, 0, n - 1, cutoff, cmp);

    free(aux);
}