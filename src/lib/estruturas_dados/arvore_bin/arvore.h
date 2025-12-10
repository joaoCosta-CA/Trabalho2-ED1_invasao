/**
 * @file arvore.h
 * @brief Módulo de Árvore Binária de Busca (BST)
 * 
 * Este módulo implementa uma estrutura de dados de Árvore Binária de Busca
 * genérica para gerenciamento eficiente de conjuntos ordenados. É utilizada
 * principalmente no algoritmo de visibilidade para manter os segmentos ativos
 * ordenados por distância.
 * 
 * @note Suporta inserção/remoção com chaves numéricas (double) para
 *       compatibilidade com o algoritmo de varredura angular.
 * @note A árvore não balanceada - complexidade média O(log n), pior caso O(n).
 * 
 * @author João Costa
 */

#ifndef ARVORE_H
#define ARVORE_H

/**
 * @brief Tipo opaco para a árvore binária de busca.
 */
typedef void *Arvore;

/**
 * @brief Tipo opaco para posição/nó na árvore (usado em iteração).
 */
typedef void *Posic;

/**
 * @brief Ponteiro de função para comparação de elementos.
 * 
 * @param a Primeiro elemento a comparar
 * @param b Segundo elemento a comparar
 * @return < 0 se a < b, > 0 se a > b, 0 se a == b
 */
typedef int (*Comparador)(const void* a, const void* b);

/**
 * @brief Cria uma árvore binária de busca vazia.
 * @param cmp Função de comparação para ordenar elementos
 * @return Ponteiro para a árvore criada
 */
Arvore tree_create(Comparador cmp);

/**
 * @brief Insere um item na árvore.
 * @param t Árvore onde inserir
 * @param item Ponteiro para o dado a inserir
 */
void tree_insert(Arvore t, void* item);

/**
 * @brief Remove um item da árvore.
 * @param arvore Árvore de onde remover
 * @param info Item a remover (usado para comparação)
 * @return Ponteiro para o item removido, ou NULL se não encontrado
 */
void* tree_remove(Arvore arvore, void* info);

/**
 * @brief Insere item com chave numérica.
 * @param arvore Ponteiro para ponteiro da árvore
 * @param key Chave numérica (distância)
 * @param info Dados a armazenar
 */
void tree_insert_with_key(Arvore* arvore, double key, void* info);

/**
 * @brief Remove item por chave numérica.
 * @param arvore Árvore de onde remover
 * @param key Chave numérica do item
 * @param data Dados do item para confirmar
 * @return 1 se removido, 0 se não encontrado
 */
int tree_remove_with_key(Arvore arvore, double key, void* data);

/**
 * @brief Encontra o item com menor valor.
 * @param t Árvore a consultar
 * @return Ponteiro para o item com menor chave
 */
void* tree_find_min(Arvore t);

/**
 * @brief Encontra o item com maior valor.
 * @param t Árvore a consultar
 * @return Ponteiro para o item com maior chave
 */
void* tree_find_max(Arvore t);

/**
 * @brief Obtém a primeira posição (menor elemento).
 * @param t Árvore a consultar
 * @return Posição do primeiro elemento
 */
Posic tree_get_first(Arvore t);

/**
 * @brief Obtém a próxima posição em ordem.
 * @param t Árvore a consultar
 * @param pos Posição atual
 * @return Próxima posição, ou NULL se fim
 */
Posic tree_get_next(Arvore t, Posic pos);

/**
 * @brief Obtém o valor em uma posição.
 * @param t Árvore a consultar
 * @param pos Posição do item
 * @return Ponteiro para o item armazenado
 */
void* tree_get_value(Arvore t, Posic pos);

/**
 * @brief Destrói a árvore e libera memória.
 * @param t Árvore a destruir
 * @param freeItem Função callback para liberar itens, ou NULL
 */
void tree_destroy(Arvore t, void (*freeItem)(void*));

#endif