#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

typedef struct node {
    void* item;
    struct node *left;
    struct node *right;
    int height; // Altura do nó para cálculo de balanceamento
} Node;

typedef struct {
    Node *root;
    Comparador cmp;
} TreeStruct;

/* --- Funções Auxiliares Privadas --- */

static int height(Node *N) {
    if (N == NULL) return 0;
    return N->height;
}

static int max(int a, int b) {
    return (a > b) ? a : b;
}

static Node* newNode(void* item) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->item = item;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

/* Rotação à Direita */
static Node* rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

/* Rotação à Esquerda */
static Node* leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

/* Obtém o fator de balanceamento */
static int getBalance(Node *N) {
    if (N == NULL) return 0;
    return height(N->left) - height(N->right);
}

/* --- Funções de Inserção Recursiva --- */

static Node* insertNode(Node* node, void* item, Comparador cmp) {
    /* 1. Inserção normal de BST */
    if (node == NULL)
        return newNode(item);

    int comparison = cmp(item, node->item);

    if (comparison < 0)
        node->left = insertNode(node->left, item, cmp);
    else if (comparison > 0)
        node->right = insertNode(node->right, item, cmp);
    else
        return node; // Duplicatas não permitidas (ou ignoradas)

    /* 2. Atualiza altura */
    node->height = 1 + max(height(node->left), height(node->right));

    /* 3. Verifica balanceamento */
    int balance = getBalance(node);

    // Caso Esquerda-Esquerda
    if (balance > 1 && cmp(item, node->left->item) < 0)
        return rightRotate(node);

    // Caso Direita-Direita
    if (balance < -1 && cmp(item, node->right->item) > 0)
        return leftRotate(node);

    // Caso Esquerda-Direita
    if (balance > 1 && cmp(item, node->left->item) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Caso Direita-Esquerda
    if (balance < -1 && cmp(item, node->right->item) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

static void freeNodes(Node* node, void (*freeItem)(void*)) {
    if (node == NULL) return;
    freeNodes(node->left, freeItem);
    freeNodes(node->right, freeItem);
    if (freeItem) freeItem(node->item);
    free(node);
}

static void printInOrder(Node* node, void (*printItem)(void*)) {
    if (node == NULL) return;

    // 1. Visita a sub-árvore esquerda
    printInOrder(node->left, printItem);

    // 2. Processa (imprime) o nó atual
    if (printItem) {
        printItem(node->item);
    }

    // 3. Visita a sub-árvore direita
    printInOrder(node->right, printItem);
}
/* --- Funções de Remoção Recursiva --- */

static Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

static Node* deleteNodeSafe(Node* root, void* item, Comparador cmp, void** item_removido) {
    if (!root) return root;

    int comparison = cmp(item, root->item);

    if (comparison < 0) {
        root->left = deleteNodeSafe(root->left, item, cmp, item_removido);
    } else if (comparison > 0) {
        root->right = deleteNodeSafe(root->right, item, cmp, item_removido);
    } else {
        // Achou o nó
        if (item_removido) *item_removido = root->item;

        if (!root->left || !root->right) {
            Node *temp = root->left ? root->left : root->right;
            if (!temp) {
                // Sem filhos
                temp = root;
                root = NULL;
            } else {
                // Um filho
                *root = *temp;
            }
            free(temp);
        } else {
            // Dois filhos: pega o sucessor in-order (menor da direita)
            Node* temp = minValueNode(root->right);
            root->item = temp->item; // Copia apenas o dado
            // Remove o sucessor
            root->right = deleteNodeSafe(root->right, temp->item, cmp, NULL);
        }
    }

    if (root == NULL) return root;

    // Atualiza Altura
    root->height = 1 + max(height(root->left), height(root->right));

    // Balanceia
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
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



void tree_destroy(Arvore t, void (*freeItem)(void*)) {
    TreeStruct *tree = (TreeStruct*) t;
    if (tree) {
        freeNodes(tree->root, freeItem);
        free(tree);
    }
}