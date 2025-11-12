#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

/* Definição das estruturas */
typedef struct node {
    Item item;// Ponteiro genérico para o dado (Círculo, Retângulo, etc)
    struct node *next;
    struct node *prev;
} Node;

typedef struct list {
    Node *head;
    Node *tail;
    int length;
} StList;

Lista createList() {
    StList *new_list = (StList *) malloc(sizeof(StList));
    if (new_list) {
        new_list->head = NULL;
        new_list->tail = NULL;
        new_list->length = 0;
    }
    return (Lista) new_list;
}

Posic insert(Lista l, Item item) {
    StList *list = (StList *) l;
    Node *new_node = (Node *) malloc(sizeof(Node));
    if (!new_node) return NULL;

    new_node->item = item;
    new_node->next = NULL;
    new_node->prev = list->tail;

    if (list->length == 0) {
        list->head = new_node;
    } else {
        list->tail->next = new_node;
    }

    list->tail = new_node;
    list->length++;
    return (Posic) new_node;
}

Item get(Lista l, Posic p) {
    StList *list = (StList *) l; // Cast apenas para validação, se necessário
    Node *node = (Node *) p;
    if (!node) return NULL;
    return node->item;
}

Posic getFirst(Lista l) {
    StList *list = (StList *) l;
    return (Posic) list->head;
}

Posic getNext(Lista l, Posic p) {
    Node *node = (Node *) p;
    if (!node) return NULL;
    return (Posic) node->next;
}

/* * Exemplo de destruição com callback para limpar a memória dos itens
 */
void killList(Lista l, void (*freeItem)(void*)) {
    StList *list = (StList *) l;
    Node *current = list->head;
    Node *next_node;

    while (current != NULL) {
        next_node = current->next;
        
        // Se o usuário passou uma função para liberar o item, chama ela
        if (freeItem != NULL && current->item != NULL) {
            freeItem(current->item);
        }
        
        free(current); // Libera o nó da lista
        current = next_node;
    }
    free(list); // Libera a estrutura da lista
}

/* ... Implementar as outras funções (remove, getLast, etc) seguindo a lógica ... */