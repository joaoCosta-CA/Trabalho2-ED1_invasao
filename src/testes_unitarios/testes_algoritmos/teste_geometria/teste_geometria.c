#include <stdio.h>
#include <assert.h>
#include "geometria.h"
#include "lista.h"
#include "segmento.h"

int main() {
    printf("[GEOMETRIA] Iniciando testes...\n");
    Lista poligono = createList();

    // Cria um quadrado (0,0) a (10,10)
    // Sentido anti-horário
    insert(poligono, create_segmento(1, 0, 0, 10, 0));   // Baixo
    insert(poligono, create_segmento(2, 10, 0, 10, 10)); // Direita
    insert(poligono, create_segmento(3, 10, 10, 0, 10)); // Topo
    insert(poligono, create_segmento(4, 0, 10, 0, 0));   // Esquerda

    // Teste 1: Ponto DENTRO
    assert(ponto_dentro_poligono(5, 5, poligono) == 1);

    // Teste 2: Ponto FORA
    assert(ponto_dentro_poligono(15, 5, poligono) == 0);
    assert(ponto_dentro_poligono(5, -5, poligono) == 0);

    // Teste 3: Ponto na BORDA (Depende da implementação de > ou >=)
    // Geralmente raycasting considera borda superior/direita como fora
    // assert(ponto_dentro_poligono(0, 0, poligono) == ?);

    // Limpeza
    // Helper local para destruir segmentos
    void destroy_seg_wrapper(void* s) { destroy_segmento(s); }
    killList(poligono, destroy_seg_wrapper);

    printf("[GEOMETRIA] Sucesso!\n");
    return 0;
}