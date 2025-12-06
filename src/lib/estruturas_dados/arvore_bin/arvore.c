#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

typedef struct node {
    void* item;
    double key;          // Numeric key for ordering (used when cmp==NULL)
    struct node *left;
    struct node *right;
    struct node *parent;
    int height;
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
    node->key = 0.0;     // Default key (not used if comparator exists)
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->height = 1;
    return node;
}

static Node* newNodeWithKey(double key, void* item) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->item = item;
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->height = 1;
    return node;
}

/* Rotação à Direita */
static Node* rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    // Store y's parent
    Node *y_parent = y->parent;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update parent pointers
    x->parent = y_parent;  // x takes y's position
    y->parent = x;         // y becomes x's child
    if (T2) T2->parent = y;  // T2 becomes y's child

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

/* Rotação à Esquerda */
static Node* leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    // Store x's parent
    Node *x_parent = x->parent;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update parent pointers
    y->parent = x_parent;  // y takes x's position
    x->parent = y;         // x becomes y's child
    if (T2) T2->parent = x;  // T2 becomes x's child

    // Update heights
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

Posic tree_get_first(Arvore t) {
    TreeStruct *tree = (TreeStruct*) t;
    if (!tree || !tree->root) return NULL;
    
    Node* current = tree->root;
    while (current->left != NULL)
        current = current->left;
    
    return (Posic)current;
}

Posic tree_get_next(Arvore t, Posic pos) {
    (void)t;
    if (!pos) return NULL;
    
    Node* node = (Node*)pos;
    
    if (node->right != NULL) {
        node = node->right;
        while (node->left != NULL)
            node = node->left;
        return (Posic)node;
    }
    
    Node* parent = node->parent;
    while (parent != NULL && node == parent->right) {
        node = parent;
        parent = parent->parent;
    }
    
    return (Posic)parent;
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
 * NUMERIC KEY FUNCTIONS - Match reference implementation
 * ======================================================================== */

/* Insert with numeric key (no comparator needed) */
static Node* insertNodeWithKey(Node* node, double key, void* item) {
    /* 1. Normal BST insertion using numeric key */
    if (node == NULL)
        return newNodeWithKey(key, item);

    if (key < node->key) {
        node->left = insertNodeWithKey(node->left, key, item);
        if (node->left) node->left->parent = node;
    }
    else if (key > node->key) {
        node->right = insertNodeWithKey(node->right, key, item);
        if (node->right) node->right->parent = node;
    }
    else {
        // Equal keys - return node (no duplicates)
        // Note: This differs from reference, but our remove uses data pointer for disambiguation anyway
        return node;
    }

    /* 2. Update height */
    node->height = 1 + max(height(node->left), height(node->right));

    /* 3. Check balance */
    int balance = getBalance(node);

    // Left-Left
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right-Right
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left-Right
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right-Left
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
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
        // Key matches - check if data also matches
        if (root->item == data) {
            // This is the node to delete
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
            // Keys match but data doesn't - continue searching
            root->right = removeNodeWithKey(root->right, key, data, found);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

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

/* Public API for numeric key insertion */
void tree_insert_with_key(Arvore *t_ptr, double key, void* item) {
    if (!t_ptr) return;
    TreeStruct *tree = (TreeStruct*) *t_ptr;
    tree->root = insertNodeWithKey(tree->root, key, item);
}

/* Public API for numeric key removal */
int tree_remove_with_key(Arvore t, double key, void* data) {
    TreeStruct *tree = (TreeStruct*) t;
    int found = 0;
    tree->root = removeNodeWithKey(tree->root, key, data, &found);
    return found;
}