/**
 * @file qsort.h
 * @brief Módulo Wrapper para Quick Sort da Biblioteca Padrão
 * 
 * Este módulo fornece uma interface simplificada para a função qsort da
 * biblioteca padrão C. Utilizado como opção de ordenação via parâmetro -o q.
 * 
 * Complexidade: O(n log n) no caso médio, O(n²) no pior caso.
 * 
 * @note Utiliza a implementação otimizada da stdlib.
 * @note Selecionado por padrão quando parâmetro -o não é especificado.
 * 
 * @author João Costa
 */

#ifndef QSORT_H
#define QSORT_H

#include "ordenacao_types.h"

/**
 * @brief Ordena um vetor usando Quick Sort da biblioteca padrão.
 * 
 * @param vetor Array de ponteiros a ser ordenado
 * @param n Número de elementos no vetor
 * @param cmp Função de comparação
 */
void quick_sort(void **vetor, int n, Comparador cmp);

#endif