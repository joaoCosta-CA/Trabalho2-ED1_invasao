#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

// Includes das Estruturas
#include "lista.h"
#include "forma.h"
#include "segmento.h"
#include "leitor_qry.h" 
#include "transformacao.h"

// Includes das Formas
#include "circulo.h"
#include "retangulo.h"

// Função auxiliar para imprimir um segmento
void debug_segmento(Segmento s) {
    printf("   [SEG ID=%d] (%.1f, %.1f) -> (%.1f, %.1f)\n", 
           get_segmento_id(s),
           get_segmento_x1(s), get_segmento_y1(s),
           get_segmento_x2(s), get_segmento_y2(s));
}

void teste_retangulo() {
    printf("\n>>> Testando Transformacao de RETANGULO\n");
    
    // Cria Retângulo: x=10, y=10, w=20, h=10
    void *r = criarRec(1, 10.0, 10.0, 20.0, 10.0, "black", "blue");
    Forma f = create_forma(RETANGULO, r);
    
    int id_counter = 100;
    
    // Ação: Transforma
    Lista segs = transformar_em_anteparos(f, 'h', &id_counter);
    
    // Verificação
    printf("   Esperado: 4 segmentos (lados do retangulo)\n");
    printf("   Obtido: %d segmentos\n", length(segs));
    assert(length(segs) == 4);

    // Lista os segmentos gerados para conferência visual
    Posic p = getFirst(segs);
    while(p) {
        Segmento s = get(segs, p);
        debug_segmento(s);
        p = getNext(segs, p);
    }
    
    // Limpeza
    killList(segs, NULL); // Nota: Em produção use free corretamente
    destroy_forma(f); // Destroi o wrapper e o retangulo
}

void teste_circulo() {
    printf("\n>>> Testando Transformacao de CIRCULO (Vertical)\n");
    
    // Cria Círculo: x=50, y=50, r=10
    // Se for 'v' (vertical), deve gerar segmento de (50, 40) a (50, 60)
    void *c = CriarCirc(2, 50.0, 50.0, 10.0, "red", "red");
    Forma f = create_forma(CIRCULO, c);
    
    int id_counter = 200;
    
    // Ação: Transforma com flag 'v'
    Lista segs = transformar_em_anteparos(f, 'v', &id_counter);
    
    // Verificação
    assert(length(segs) == 1);
    
    Segmento s = get(segs, getFirst(segs));
    debug_segmento(s);
    
    // Validação matemática
    assert(get_segmento_x1(s) == 50.0);
    assert(get_segmento_x2(s) == 50.0);
    // Y deve variar do centro-raio ao centro+raio
    assert(get_segmento_y1(s) == 40.0); 
    assert(get_segmento_y2(s) == 60.0);

    killList(segs, NULL);
    destroy_forma(f);
}

int main() {
    teste_retangulo();
    teste_circulo();
    
    printf("\n[SUCESSO] As formas estao virando anteparos corretamente!\n");
    return 0;
}