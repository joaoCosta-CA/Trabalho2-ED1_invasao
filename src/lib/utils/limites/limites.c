#include <stdio.h>
#include <stdlib.h>
#include <float.h> // Para DBL_MAX
#include "limites.h"
#include "forma.h"
#include "segmento.h"

// Includes específicos para pegar coordenadas
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

// A definição da struct fica ESCONDIDA aqui dentro
typedef struct {
    double min_x;
    double min_y;
    double max_x;
    double max_y;
} StLimites;

Limites calcular_limites_mundo(Lista formas) {
    // Aloca a estrutura na memória (Heap)
    StLimites *box = (StLimites*) malloc(sizeof(StLimites));
    
    // Inicializa com valores invertidos para permitir expansão correta
    box->min_x = DBL_MAX;  box->min_y = DBL_MAX;
    box->max_x = -DBL_MAX; box->max_y = -DBL_MAX;

    if (!formas || length(formas) == 0) {
        // Retorna bbox vazia (com valores invertidos) para ser expandida depois
        return (Limites)box;
    }

    Posic p = getFirst(formas);
    while (p) {
        Forma f = get(formas, p);
        void *dados = get_dados_forma(f);
        ShapeType tipo = get_tipo_forma(f);
        
        double x1=0, y1=0, x2=0, y2=0;

        switch(tipo) {
            case CIRCULO: {
                double cx = circulo_get_x(dados);
                double cy = circulo_get_y(dados);
                double r = circulo_get_raio(dados);
                x1 = cx - r; x2 = cx + r;
                y1 = cy - r; y2 = cy + r;
                break;
            }
            case RETANGULO: {
                double rx = retangulo_get_x(dados);
                double ry = retangulo_get_y(dados);
                double w = retangulo_get_larg(dados);
                double h = retangulo_get_altura(dados);
                x1 = rx; x2 = rx + w;
                y1 = ry; y2 = ry + h;
                break;
            }
            case LINHA: {
                double lx1 = linha_get_x1(dados);
                double ly1 = linha_get_y1(dados);
                double lx2 = linha_get_x2(dados);
                double ly2 = linha_get_y2(dados);
                x1 = (lx1 < lx2) ? lx1 : lx2;
                x2 = (lx1 > lx2) ? lx1 : lx2;
                y1 = (ly1 < ly2) ? ly1 : ly2;
                y2 = (ly1 > ly2) ? ly1 : ly2;
                break;
            }
            case TEXTO: {
                double tx = texto_get_x(dados);
                double ty = texto_get_y(dados);
                x1 = tx; x2 = tx + 50; // Estimativa
                y1 = ty; y2 = ty + 10;
                break;
            }
            default: break;
        }

        if (x1 < box->min_x) box->min_x = x1;
        if (x2 > box->max_x) box->max_x = x2;
        if (y1 < box->min_y) box->min_y = y1;
        if (y2 > box->max_y) box->max_y = y2;

        p = getNext(formas, p);
    }

    return (Limites)box;
}

/* Implementação dos Getters */
double get_limites_min_x(Limites l) { return ((StLimites*)l)->min_x; }
double get_limites_min_y(Limites l) { return ((StLimites*)l)->min_y; }
double get_limites_max_x(Limites l) { return ((StLimites*)l)->max_x; }
double get_limites_max_y(Limites l) { return ((StLimites*)l)->max_y; }

void destruir_limites(Limites l) {
    if (l) free(l);
}

void limites_expandir_ponto(Limites l, double x, double y) {
    StLimites *box = (StLimites*) l;
    if (!box) return;

    if (x < box->min_x) box->min_x = x;
    if (x > box->max_x) box->max_x = x;
    if (y < box->min_y) box->min_y = y;
    if (y > box->max_y) box->max_y = y;
}

void limites_expandir_segmentos(Limites l, Lista segmentos) {
    StLimites *box = (StLimites*) l;
    if (!box || !segmentos) return;

    Posic p = getFirst(segmentos);
    while (p) {
        void *s = get(segmentos, p);
        
        // Testa o ponto inicial do segmento
        double x1 = get_segmento_x1(s);
        double y1 = get_segmento_y1(s);
        limites_expandir_ponto(l, x1, y1);

        // Testa o ponto final do segmento
        double x2 = get_segmento_x2(s);
        double y2 = get_segmento_y2(s);
        limites_expandir_ponto(l, x2, y2);

        p = getNext(segmentos, p);
    }
}