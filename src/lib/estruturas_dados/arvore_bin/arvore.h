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
typedef void* Posic;
                        
typedef int (*Comparador)(const void*, const void*);

Arvore tree_create(Comparador cmp);

void tree_insert(Arvore t, void* item);

void* tree_remove(Arvore t, void* item);

void* tree_find_min(Arvore t);

void* tree_find_max(Arvore t);

Posic tree_get_first(Arvore t);

Posic tree_get_next(Arvore t, Posic pos);

void* tree_get_value(Arvore t, Posic pos);

void tree_destroy(Arvore t, void (*freeItem)(void*));



#endif