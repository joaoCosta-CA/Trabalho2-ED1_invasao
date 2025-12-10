/**
 * @file ordenacao_types.h
 * @brief Tipos Compartilhados para Algoritmos de Ordenação
 * 
 * Este módulo define tipos e estruturas compartilhadas entre os diferentes
 * algoritmos de ordenação (Merge Sort, Insertion Sort, Quick Sort).
 * 
 * @note O typedef Comparador é compatível com a função qsort da stdlib.
 * 
 * @author João Costa
 */

#ifndef ORDENACAO_TYPES_H
#define ORDENACAO_TYPES_H

/**
 * @brief Ponteiro de função para comparação de elementos.
 * 
 * Função callback compatível com qsort da stdlib para comparação de elementos.
 * 
 * @param a Ponteiro para o primeiro elemento
 * @param b Ponteiro para o segundo elemento
 * @return < 0 se a < b, > 0 se a > b, 0 se a == b
 */
typedef int (*Comparador)(const void *a, const void *b);

#endif