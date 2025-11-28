#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lista.h"

// Função auxiliar para imprimir a lista inteira (como discutido anteriormente)
void printList_Int(Lista l) {
    printf("   [Estado Atual]: ");
    Posic p = getFirst(l);
    while(p != NULL) {
        int *val = (int*)get(l, p);
        printf("%d -> ", *val);
        p = getNext(l, p);
    }
    printf("NULL (Tam: %d)\n", length(l));
}

int main() {
    printf("=== [LISTA] Iniciando Testes Detalhados ===\n\n");
    
    Lista l = createList();
    assert(l != NULL);

    int a = 10, b = 20, c = 30;
    
    // --- 1. Inserção ---
    printf("--> Inserindo 10, 20 e 30...\n");
    insert(l, &a);
    insert(l, &b);
    insert(l, &c);
    
    assert(length(l) == 3);
    printList_Int(l); // Mostra: 10 -> 20 -> 30 -> NULL

    // --- 2. Visualizando Primeiro e Último (Antes de remover) ---
    Posic p_primeiro = getFirst(l);
    Posic p_ultimo = getLast(l);

    // Cast para int* e depois * para pegar o valor
    int *val_prim = (int*)get(l, p_primeiro);
    int *val_ult  = (int*)get(l, p_ultimo);

    printf("\n--> Verificacao de Extremos:\n");
    printf("    Primeiro valor: %d\n", *val_prim); // Deve ser 10
    printf("    Ultimo valor:   %d\n", *val_ult);  // Deve ser 30
    
    assert(*val_prim == 10);
    assert(*val_ult == 30);

    // --- 3. Remoção do Primeiro Elemento ---
    printf("\n--> Removendo o primeiro elemento...\n");
    
    // removePosic retorna o 'Item' (void*), nós capturamos para imprimir
    int *item_removido = (int*)removePosic(l, p_primeiro);
    
    printf("    Valor deletado: %d\n", *item_removido); // Deve ser 10
    assert(*item_removido == 10);
    assert(length(l) == 2);

    printList_Int(l); // Mostra: 20 -> 30 -> NULL

    // --- 4. Verificando o Novo Primeiro ---
    Posic p_novo_primeiro = getFirst(l);
    int *val_novo_prim = (int*)get(l, p_novo_primeiro);
    
    printf("\n--> Verificando novo primeiro:\n");
    printf("    Novo Primeiro:  %d\n", *val_novo_prim); // Deve ser 20
    assert(*val_novo_prim == 20);

    // --- 5. Destruição ---
    killList(l, NULL);
    
    printf("\n=== [SUCESSO] Todos os testes passaram! ===\n");
    return 0;
}