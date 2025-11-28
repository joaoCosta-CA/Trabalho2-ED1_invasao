#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "arvore.h"

int comparar_ints(void* a, void* b) {
    int *ia = (int*)a;
    int *ib = (int*)b;
    return (*ia) - (*ib);
}

void imprimir_int(void* item) {
    printf("%d ", *(int*)item);
}

// --- Main ---

int main() {
    printf("=== [ARVORE] Iniciando Testes Detalhados ===\n\n");
    
    // Criação
    Arvore t = tree_create(comparar_ints);
    assert(t != NULL);

    int nums[] = {50, 30, 20, 40, 70, 60, 80};
    int qtd = 7;
    
    // 1. Inserção
    printf("--> Inserindo valores: ");
    for(int i=0; i<qtd; i++) {
        printf("%d ", nums[i]);
        tree_insert(t, &nums[i]);
    }
    printf("\n");

    // 2. Visualização (Se você implementou tree_print)
    printf("    Estado Atual (Em Ordem): ");
    tree_print(t, imprimir_int); 
    printf("\n");
    
    // 3. Verificando Extremos
    int *min = (int*)tree_find_min(t);
    int *max = (int*)tree_find_max(t);

    printf("\n--> Verificacao de Extremos:\n");
    printf("    Minimo encontrado: %d\n", *min);
    printf("    Maximo encontrado: %d\n", *max);
    
    assert(*min == 20);
    assert(*max == 80);

    // 4. Remoção da Raiz (50)
    printf("\n--> Removendo a raiz (50)...\n");
    int *removido = (int*)tree_remove(t, &nums[0]); // &nums[0] é 50
    
    if (removido) {
        printf("    Item removido com sucesso: %d\n", *removido);
        assert(*removido == 50);
    } else {
        printf("    [ERRO] Item nao encontrado para remocao!\n");
        exit(1);
    }

    printf("    Nova Arvore: ");
    tree_print(t, imprimir_int);
    printf("\n");

    // 5. Teste Crítico: Removendo o Mínimo (20)
    printf("\n--> Removendo o Mínimo atual (20)...\n");
    int val_20 = 20;
    tree_remove(t, &val_20);

    int *novo_min = (int*)tree_find_min(t);
    printf("    Novo Minimo: %d\n", *novo_min);
    assert(*novo_min == 30);

    printf("    Arvore Final: ");
    tree_print(t, imprimir_int);
    printf("\n");

    // 6. Destruição
    tree_destroy(t, NULL);

    printf("\n=== [SUCESSO] Todos os testes da Arvore passaram! ===\n");
    return 0;
}