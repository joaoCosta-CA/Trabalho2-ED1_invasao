/**
 * @file insert.h
 * @brief Módulo de Insertion Sort
 * 
 * Este módulo implementa o algoritmo Insertion Sort para ordenação de arrays.
 * Utilizado como otimização do Merge Sort híbrido para subarrays pequenos,
 * onde a baixa sobrecarga do Insertion Sort supera a eficiência assintótica.
 * 
 * Complexidade: O(n²) no pior caso, O(n) no melhor caso (array ordenado).
 * 
 * @note Eficiente para arrays pequenos (tipicamente < 10-20 elementos).
 * @note Ordenação estável (preserva ordem relativa de elementos iguais).
 * @note Ordenação in-place (não requer memória auxiliar).
 * 
 * @author João Costa
 */

#ifndef INSERT_SORT_H
#define INSERT_SORT_H

#include "ordenacao_types.h"

/**
 * @brief Ordena um intervalo do vetor usando Insertion Sort.
 * 
 * @param vetor Array de ponteiros a ser ordenado
 * @param l Índice inicial do intervalo (inclusivo)
 * @param r Índice final do intervalo (inclusivo)
 * @param cmp Função de comparação
 */
void insertion_sort(void **vetor, int l, int r, Comparador cmp);

#endif