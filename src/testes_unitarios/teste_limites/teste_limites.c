#include <stdio.h>
#include <assert.h>
#include "limites.h"

int main() {
    printf("[LIMITES] Testando criacao e expansao...\n");
    
    Limites lim = calcular_limites_mundo(NULL);
    assert(lim != NULL);
    
    // Expande com pontos
    limites_expandir_ponto(lim, 10.0, 20.0);
    limites_expandir_ponto(lim, 100.0, 200.0);
    limites_expandir_ponto(lim, 50.0, 100.0);
    
    printf("   > Apos adicionar 3 pontos:\n");
    printf("     Min: (%.1f, %.1f)\n", get_limites_min_x(lim), get_limites_min_y(lim));
    printf("     Max: (%.1f, %.1f)\n", get_limites_max_x(lim), get_limites_max_y(lim));
    
    // Verifica valores
    assert(get_limites_min_x(lim) <= 10.0);
    assert(get_limites_min_y(lim) <= 20.0);
    assert(get_limites_max_x(lim) >= 100.0);
    assert(get_limites_max_y(lim) >= 200.0);
    
    destruir_limites(lim);
    
    printf(">>> SUCESSO: Modulo Limites OK!\n");
    return 0;
}
