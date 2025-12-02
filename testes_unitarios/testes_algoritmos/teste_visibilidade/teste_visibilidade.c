#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lista.h"
#include "segmento.h"
#include "visibilidade.h" // Sua função calcular_visibilidade
#include "svg.h"   // Para gerar o debug visual

// Helper para limpar memória
void destroy_seg_void(void *p) { if(p) destroy_segmento((Segmento)p); }

int main() {
    printf("[VISIBILIDADE] 1. Preparando cenário (Caixa Fechada)...\n");
    
    // Centro da bomba
    double bx = 500.0, by = 500.0;
    
    // Lista de anteparos
    Lista anteparos = createList();
    int id = 1;

    // Cria uma caixa de 200x200 ao redor da bomba (400 a 600)
    // Parede Superior
    insert(anteparos, create_segmento(id++, 400, 400, 600, 400));
    // Parede Direita
    insert(anteparos, create_segmento(id++, 600, 400, 600, 600));
    // Parede Inferior
    insert(anteparos, create_segmento(id++, 600, 600, 400, 600));
    // Parede Esquerda
    insert(anteparos, create_segmento(id++, 400, 600, 400, 400));

    printf("[VISIBILIDADE] 2. Executando algoritmo...\n");
    
    // Roda com perturbação para evitar instabilidade numérica
    Lista poligono = calcular_visibilidade(bx + 0.00001, by + 0.00001, anteparos);

    printf("[VISIBILIDADE] 3. Verificando resultados...\n");
    
    // Devemos ter pelo menos 4 segmentos de resposta (o contorno da caixa)
    int qtd_arestas = length(poligono);
    printf("   > Arestas geradas: %d\n", qtd_arestas);
    
    // Validação básica: Não pode ser vazio e deve ser >= 4 para fechar a caixa
    assert(poligono != NULL);
    assert(qtd_arestas >= 4);

    printf("[VISIBILIDADE] 4. Gerando SVG de prova ('teste_visib.svg')...\n");
    
    // Gera SVG passando NULL nas formas (só queremos ver o polígono e anteparos)
    gerar_svg_debug(NULL, anteparos, poligono, bx, by, "teste_visib.svg");

    // Limpeza
    killList(anteparos, destroy_seg_void);
    killList(poligono, destroy_seg_void);

    printf("\n>>> SUCESSO: Algoritmo rodou sem crashes! Verifique o SVG. <<<\n");
    return 0;
}