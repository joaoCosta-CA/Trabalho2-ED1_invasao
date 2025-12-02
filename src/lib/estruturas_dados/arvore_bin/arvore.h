#ifndef ARVORE_H
#define ARVORE_H

/* Tipo opaco da árvore */
typedef void *Arvore;

/* Ponteiro de função para comparação.
   Retorna < 0 se a < b
   Retorna > 0 se a > b
   Retorna 0 se a == b
*/
typedef int (*Comparador)(const void* a, const void* b);

/* Cria uma árvore vazia passando a função que sabe comparar os itens */
Arvore tree_create(Comparador cmp);

/* Insere um item. Retorna o nó inserido ou NULL se erro. */
void tree_insert(Arvore t, void* item);

/* Remove um item da árvore. Retorna o item removido (para free externo) ou NULL */
void* tree_remove(Arvore t, void* item);

/* Retorna o item com o "menor" valor (mais à esquerda). */
void* tree_find_min(Arvore t);

/* Retorna o item com o "maior" valor (mais à direita) */
void* tree_find_max(Arvore t);

/* Destrói a árvore e limpa a memória dos nós */
void tree_destroy(Arvore t, void (*freeItem)(void*));

/* Debug: Imprime a árvore (opcional, útil para testes) */
void tree_print(Arvore t, void (*printItem)(void*));

#endif