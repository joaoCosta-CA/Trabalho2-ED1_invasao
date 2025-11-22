#ifndef LISTA_H
#define LISTA_H

/* * Definição de tipos opacos. 
 * O usuário só vê ponteiros void, sem saber o que há dentro.
 */
typedef void *Lista;
typedef void *Posic;
typedef void *Item;

/*
 * Cria uma lista vazia.
 * Retorna: Ponteiro para a lista criada ou NULL se falhar.
 */
Lista createList();

/*
 * Insere um elemento no final da lista.
 * l: A lista.
 * item: O ponteiro para o dado (ex: struct Circulo*).
 * Retorna: A posição do item inserido.
 */
Posic insert(Lista l, Item item);

/*
 * Remove o elemento indicado pela posição.
 * O ideal é retornar o item para que o usuário possa dar free() nele se necessário.
 */
Item removePosic(Lista l, Posic p);

/*
 * Retorna o tamanho da lista.
 */
int length(Lista l);

/*
 * Funções de travessia (Iteradores)
 */
Posic getFirst(Lista l);
Posic getNext(Lista l, Posic p);
Posic getLast(Lista l);
Posic getPrevious(Lista l, Posic p);

/*
 * Retorna o valor armazenado em uma posição.
 */
Item get(Lista l, Posic p);

/*
 * Destrói a lista.
 * Como a lista armazena void*, ela não sabe como liberar os itens internos (Formas).
 * Por isso, pode-se passar uma função de callback para liberar os itens,
 * ou NULL se os itens não devem ser liberados agora.
 */
void killList(Lista l, void (*freeItem)(void*));

#endif