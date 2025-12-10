#include <stdio.h>
#include <assert.h>
#include "segmento.h"

int main() {
    printf("[SEGMENTO] Testando criacao e getters...\n");
    
    Segmento s = create_segmento(42, 10.0, 20.0, 30.0, 40.0);
    assert(s != NULL);
    
    assert(get_segmento_id(s) == 42);
    assert(get_segmento_x1(s) == 10.0);
    assert(get_segmento_y1(s) == 20.0);
    assert(get_segmento_x2(s) == 30.0);
    assert(get_segmento_y2(s) == 40.0);
    
    printf("   > ID: %d\n", get_segmento_id(s));
    printf("   > P1: (%.1f, %.1f)\n", get_segmento_x1(s), get_segmento_y1(s));
    printf("   > P2: (%.1f, %.1f)\n", get_segmento_x2(s), get_segmento_y2(s));
    
    destroy_segmento(s);
    
    printf(">>> SUCESSO: Modulo Segmento OK!\n");
    return 0;
}
