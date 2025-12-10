#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lista.h"
#include "segmento.h"
#include "visibilidade.h"
#include "limites.h"
#include "svg.h"

void destroy_seg_void(void *p) { if(p) destroy_segmento((Segmento)p); }

int main() {
    printf("[VISIBILIDADE] 1. Preparando cenario...\\n");
    
    double bx = 500.0, by = 500.0;
    
    Lista anteparos = createList();
    int id = 1;

    insert(anteparos, create_segmento(id++, 400, 400, 600, 400));
    insert(anteparos, create_segmento(id++, 600, 400, 600, 600));
    insert(anteparos, create_segmento(id++, 600, 600, 400, 600));
    insert(anteparos, create_segmento(id++, 400, 600, 400, 400));

    printf("[VISIBILIDADE] 2. Executando algoritmo...\\n");
    
    Limites box = calcular_limites_mundo(NULL);
    limites_expandir_ponto(box, 300, 300);
    limites_expandir_ponto(box, 700, 700);
    
    Lista poligono = calcular_visibilidade(bx, by, anteparos, box, 'q', 10);

    printf("[VISIBILIDADE] 3. Verificando resultados...\\n");
    
    int qtd_arestas = length(poligono);
    printf("   > Arestas geradas: %d\\n", qtd_arestas);
    
    assert(poligono != NULL);
    assert(qtd_arestas >= 4);

    printf("[VISIBILIDADE] 4. Gerando SVG...\\n");
    
    Lista pontos_bomba = createList();
    double *pt = malloc(2 * sizeof(double));
    pt[0] = bx; pt[1] = by;
    insert(pontos_bomba, pt);
    
    gerar_svg(NULL, anteparos, poligono, pontos_bomba, "teste_visib.svg");

    destruir_limites(box);
    killList(anteparos, destroy_seg_void);
    killList(poligono, destroy_seg_void);
    killList(pontos_bomba, free);

    printf("\\n>>> SUCESSO! <<<\\n");
    return 0;
}