/**
 * @file merge.h
 * @brief Módulo de Merge Sort Híbrido
 * 
 * Este módulo implementa o algoritmo Merge Sort com otimização híbrida.
 * Para subarrays menores que o cutoff, utiliza Insertion Sort que é mais
 * eficiente para conjuntos pequenos.
 * 
 * Complexidade: O(n log n) no caso médio e pior caso.
 * 
 * @note O cutoff é configurável pelo parâmetro -in da linha de comando.
 * @note Utiliza memória auxiliar O(n) para a mesclagem.
 * 
 * @author João Costa
 */

#ifndef MERGE_H
#define MERGE_H

#include "ordenacao_types.h"

/**
 * @brief Ordena um vetor usando Merge Sort Híbrido.
 * 
 * @param vetor Array de ponteiros a ser ordenado
 * @param n Número de elementos no vetor
 * @param cutoff Limite abaixo do qual usa Insertion Sort
 * @param cmp Função de comparação
 */
void merge_sort(void **vetor, int n, int cutoff, Comparador cmp);

#endif