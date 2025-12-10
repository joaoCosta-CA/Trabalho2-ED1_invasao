#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "estilo_texto.h"

int main() {
    printf("[ESTILO_TEXTO] Testando criacao e getters...\n");
    
    EstiloTexto est = create_estilo_texto("serif", "bold", 16.0);
    assert(est != NULL);
    
    printf("   > Family: %s\n", get_estilo_familia(est));
    printf("   > Weight: %s\n", get_estilo_peso(est));
    printf("   > Size: %.1f\n", get_estilo_tamanho(est));
    
    assert(strcmp(get_estilo_familia(est), "serif") == 0);
    assert(strcmp(get_estilo_peso(est), "bold") == 0);
    assert(get_estilo_tamanho(est) == 16.0);
    
    destroy_estilo_texto(est);
    
    printf(">>> SUCESSO: Modulo Estilo Texto OK!\n");
    return 0;
}
