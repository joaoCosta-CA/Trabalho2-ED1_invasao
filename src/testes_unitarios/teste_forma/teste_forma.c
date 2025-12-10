#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "forma.h"
#include "circulo.h"
#include "retangulo.h"

void destruir_forma_cb(void *f) { destroy_forma((Forma)f); }

int main() {
    printf("[FORMA] Testando wrapper de forma...\n");
    
    // Teste com Circulo
    void *circ = CriarCirc(1, 50.0, 50.0, 25.0, "red", "blue");
    Forma f1 = create_forma(CIRCULO, circ);
    
    assert(f1 != NULL);
    assert(get_tipo_forma(f1) == CIRCULO);
    assert(get_forma_id_generico(f1) == 1);
    assert(strcmp(get_tipo_forma_str(f1), "circulo") == 0);
    
    printf("   > Forma 1: ID=%d, Tipo=%s\n", get_forma_id_generico(f1), get_tipo_forma_str(f1));
    
    // Teste com Retangulo
    void *rect = criarRec(2, 10.0, 10.0, 40.0, 30.0, "green", "yellow");
    Forma f2 = create_forma(RETANGULO, rect);
    
    assert(get_tipo_forma(f2) == RETANGULO);
    assert(get_forma_id_generico(f2) == 2);
    assert(strcmp(get_tipo_forma_str(f2), "retangulo") == 0);
    
    printf("   > Forma 2: ID=%d, Tipo=%s\n", get_forma_id_generico(f2), get_tipo_forma_str(f2));
    
    // Teste clone
    Forma clone = forma_clonar(f1, 10, 100.0, 100.0);
    assert(clone != NULL);
    assert(get_forma_id_generico(clone) == 10);
    printf("   > Clone: ID=%d\n", get_forma_id_generico(clone));
    
    destroy_forma(f1);
    destroy_forma(f2);
    destroy_forma(clone);
    
    printf(">>> SUCESSO: Modulo Forma OK!\n");
    return 0;
}
