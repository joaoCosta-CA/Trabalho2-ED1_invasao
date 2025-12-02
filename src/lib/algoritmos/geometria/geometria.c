#include "geometria.h"
#include "segmento.h"
#include <math.h>

/* Baseado no PDF (Pag 5) */
double orientacao(double ax, double ay, double bx, double by, double cx, double cy) {
    return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

/* * Lógica clássica de Geometria Computacional:
 * Dois segmentos AB e CD se interceptam se e somente se:
 * 1. C e D estão em lados opostos da reta AB.
 * 2. A e B estão em lados opostos da reta CD.
 */
bool tem_interseccao(double ax, double ay, double bx, double by, 
                     double cx, double cy, double dx, double dy) {
    
    double abc = orientacao(ax, ay, bx, by, cx, cy);
    double abd = orientacao(ax, ay, bx, by, dx, dy);
    double cda = orientacao(cx, cy, dx, dy, ax, ay);
    double cdb = orientacao(cx, cy, dx, dy, bx, by);

    // Se sinais opostos, produto é negativo
    if (((abc > 0 && abd < 0) || (abc < 0 && abd > 0)) &&
        ((cda > 0 && cdb < 0) || (cda < 0 && cdb > 0))) {
        return true;
    }
    
    // Casos de colinearidade não são o foco principal aqui para 'tem_interseccao' 
    // simples do raio, mas em implementações robustas devem ser tratados.
    return false; 
}

void calcular_interseccao(double ax, double ay, double bx, double by, 
                          double cx, double cy, double dx, double dy,
                          double *x_int, double *y_int) {
    // Reta 1: a1*x + b1*y = c1
    double a1 = by - ay;
    double b1 = ax - bx;
    double c1 = a1 * ax + b1 * ay;
 
    // Reta 2: a2*x + b2*y = c2
    double a2 = dy - cy;
    double b2 = cx - dx;
    double c2 = a2 * cx + b2 * cy;
 
    double determinant = a1 * b2 - a2 * b1;
 
    if (determinant != 0) {
        *x_int = (b2 * c1 - b1 * c2) / determinant;
        *y_int = (a1 * c2 - a2 * c1) / determinant;
    } else {
        // Retas paralelas (não deve acontecer no contexto do raio x segmento)
        *x_int = 0; *y_int = 0;
    }
}

bool ponto_dentro_poligono(double px, double py, Lista poligono) {
    bool inside = false;
    Posic p = getFirst(poligono);

    while (p) {
        void* seg = get(poligono, p);
        double x1 = get_segmento_x1(seg);
        double y1 = get_segmento_y1(seg);
        double x2 = get_segmento_x2(seg);
        double y2 = get_segmento_y2(seg);

        /*
         * ALGORITMO DE RAY CASTING
         * Imagina uma linha horizontal saindo do ponto P para a direita.
         * Contamos quantas vezes essa linha cruza as arestas do polígono.
         * Se cruzar um número ÍMPAR de vezes -> Está Dentro.
         * Se cruzar um número PAR de vezes -> Está Fora.
         */

        // 1. Verifica se a coordenada Y do ponto está dentro do intervalo Y do segmento
        // A condição ((y1 > py) != (y2 > py)) garante que uma ponta está acima e outra abaixo
        if (((y1 > py) != (y2 > py))) {
            
            // 2. Calcula a coordenada X onde a linha horizontal cruza o segmento
            double x_interseccao = (x2 - x1) * (py - y1) / (y2 - y1) + x1;

            // 3. Verifica se o cruzamento acontece à direita do ponto P
            if (px < x_interseccao) {
                inside = !inside; // Inverte o estado (Par <-> Ímpar)
            }
        }

        p = getNext(poligono, p);
    }

    return inside;
}