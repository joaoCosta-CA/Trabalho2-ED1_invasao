#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

// Nó BST simples
typedef struct node {
    void* item;
    double key;          // Chave numérica para ordenação
    struct node *left;
    struct node *right;
} Node;

typedef struct {
    Node *root;
    Comparador cmp;
} TreeStruct;

/* --- Funções Auxiliares BST --- */

static Node* newNode(void* item) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->item = item;
    node->key = 0.0;     // Chave padrão
    node->left = NULL;
    node->right = NULL;
    return node;
}

static Node* newNodeWithKey(double key, void* item) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->item = item;
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* --- Funções de Inserção Recursiva --- */

/* Inserção BST simples */
static Node* insertNode(Node* node, void* item, Comparador cmp) {
    if (node == NULL)
        return newNode(item);

    int comparison = cmp(item, node->item);

    if (comparison < 0)
        node->left = insertNode(node->left, item, cmp);
    else if (comparison > 0)
        node->right = insertNode(node->right, item, cmp);
    // Duplicata, ignora

    return node;
}

static void freeNodes(Node* node, void (*freeItem)(void*)) {
    if (node == NULL) return;
    freeNodes(node->left, freeItem);
    freeNodes(node->right, freeItem);
    if (freeItem) freeItem(node->item);
    free(node);
}

/* --- Funções de Remoção Recursiva --- */
static Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

/* Remoção BST simples */
static Node* deleteNodeSafe(Node* root, void* item, Comparador cmp, void** item_removido) {
    if (!root) return root;

    int comparison = cmp(item, root->item);

    if (comparison < 0) {
        root->left = deleteNodeSafe(root->left, item, cmp, item_removido);
    } else if (comparison > 0) {
        root->right = deleteNodeSafe(root->right, item, cmp, item_removido);
    } else {
        // Nó encontrado para remover
        if (item_removido) *item_removido = root->item;

        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        // Dois filhos: obtém o sucessor inorder
        Node* temp = minValueNode(root->right);
        root->item = temp->item;
        root->right = deleteNodeSafe(root->right, temp->item, cmp, NULL);
    }

    return root;
}


/* --- Implementações Públicas --- */

Arvore tree_create(Comparador cmp) {
    TreeStruct *t = malloc(sizeof(TreeStruct));
    t->root = NULL;
    t->cmp = cmp;
    return t;
}

void tree_insert(Arvore t, void* item) {
    TreeStruct *tree = (TreeStruct*) t;
    tree->root = insertNode(tree->root, item, tree->cmp);
}

void* tree_remove(Arvore t, void* item) {
    TreeStruct *tree = (TreeStruct*) t;
    void* removido = NULL;
    tree->root = deleteNodeSafe(tree->root, item, tree->cmp, &removido);
    return removido;
}

void* tree_find_min(Arvore t) {
    TreeStruct *tree = (TreeStruct*) t;
    Node* current = tree->root;
    if (!current) return NULL;
    
    while (current->left != NULL)
        current = current->left;
    
    return current->item;
}

void* tree_find_max(Arvore t) {
    TreeStruct *tree = (TreeStruct*) t;
    Node* current = tree->root;
    if (!current) return NULL;
    
    while (current->right != NULL)
        current = current->right;
    
    return current->item;
}

Posic tree_get_first(Arvore t) {
    TreeStruct *tree = (TreeStruct*) t;
    if (!tree || !tree->root) return NULL;
    
    Node* current = tree->root;
    while (current->left != NULL)
        current = current->left;
    
    return (Posic)current;
}

/* Helper to build path from root to node (alternative to parent pointers) */
static int find_path_to_node(Node* root, Node* target, Node** path, int* path_len) {
    if (!root) return 0;
    
    path[*path_len] = root;
    (*path_len)++;
    
    if (root == target) return 1;
    
    if (find_path_to_node(root->left, target, path, path_len) ||
        find_path_to_node(root->right, target, path, path_len)) {
        return 1;
    }
    
    (*path_len)--;
    return 0;
}

Posic tree_get_next(Arvore t, Posic pos) {
    if (!t || !pos) return NULL;
    
    TreeStruct *tree = (TreeStruct*)t;
    Node* node = (Node*)pos;
    
    // Se existe subárvore direita, retorna o mais à esquerda dela
    if (node->right != NULL) {
        node = node->right;
        while (node->left != NULL)
            node = node->left;
        return (Posic)node;
    }
    
    // Precisa subir - usa array de caminho
    Node* path[100];
    int path_len = 0;
    
    if (!find_path_to_node(tree->root, node, path, &path_len)) {
        return NULL;
    }
    
    // Encontra o primeiro ancestral onde o nó está na subárvore esquerda
    for (int i = path_len - 2; i >= 0; i--) {
        if (path[i]->left == path[i+1]) {
            return (Posic)path[i];
        }
    }
    
    return NULL; // Sem sucessor
}

void* tree_get_value(Arvore t, Posic pos) {
    (void)t;
    if (!pos) return NULL;
    
    Node* node = (Node*)pos;
    return node->item;
}


void tree_destroy(Arvore t, void (*freeItem)(void*)) {
    TreeStruct *tree = (TreeStruct*) t;
    if (tree) {
        freeNodes(tree->root, freeItem);
        free(tree);
    }
}

/* ========================================================================
 * FUNÇÕES COM CHAVE NUMÉRICA
 * ======================================================================== */

// Inserção BST simples (iterativa)
static Node* insertNodeWithKey(Node* root, double key, void* item) {
    Node** current = &root;
    
    while (*current != NULL) {
        if (key < (*current)->key) {
            current = &((*current)->left);
        } else {
            current = &((*current)->right);
        }
    }
    
    *current = newNodeWithKey(key, item);
    return root;
}

/* Remove with numeric key + data pointer for disambiguation */
static Node* removeNodeWithKey(Node* root, double key, void* data, int* found) {
    if (root == NULL) {
        *found = 0;
        return root;
    }

    if (key < root->key) {
        root->left = removeNodeWithKey(root->left, key, data, found);
    }
    else if (key > root->key) {
        root->right = removeNodeWithKey(root->right, key, data, found);
    }
    else {
        // Chave coincide - verifica se dado também coincide
        if (root->item == data) {
            // Nó a ser removido
            *found = 1;
            
            if ((root->left == NULL) || (root->right == NULL)) {
                Node *temp = root->left ? root->left : root->right;

                if (temp == NULL) {
                    temp = root;
                    root = NULL;
                } else {
                    *root = *temp;
                }
                free(temp);
            } else {
                Node* temp = minValueNode(root->right);
                root->item = temp->item;
                root->key = temp->key;
                root->right = removeNodeWithKey(root->right, temp->key, temp->item, found);
            }
        } else {
            // Chaves iguais mas dados diferentes - continua buscando
            root->right = removeNodeWithKey(root->right, key, data, found);
        }
    }

    // BST simples
    return root;
}

/* API pública para inserção com chave numérica */
void tree_insert_with_key(Arvore *t_ptr, double key, void* item) {
    if (!t_ptr) return;
    TreeStruct *tree = (TreeStruct*) *t_ptr;
    tree->root = insertNodeWithKey(tree->root, key, item);
}

/* API pública para remoção com chave numérica */
int tree_remove_with_key(Arvore t, double key, void* data) {
    TreeStruct *tree = (TreeStruct*) t;
    int found = 0;
    tree->root = removeNodeWithKey(tree->root, key, data, &found);
    return found;
}