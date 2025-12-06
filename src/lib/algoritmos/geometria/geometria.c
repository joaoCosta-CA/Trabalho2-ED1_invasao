#include "geometria.h"
#include "segmento.h"
#include <math.h>
#include <stdbool.h>

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

    if (((abc > 0 && abd < 0) || (abc < 0 && abd > 0)) &&
        ((cda > 0 && cdb < 0) || (cda < 0 && cdb > 0))) {
        return true;
    }
    return false; 
}

void calcular_interseccao(double ax, double ay, double bx, double by, 
                          double cx, double cy, double dx, double dy,
                          double *x_int, double *y_int) {
    double a1 = by - ay;
    double b1 = ax - bx;
    double c1 = a1 * ax + b1 * ay;
 
    double a2 = dy - cy;
    double b2 = cx - dx;
    double c2 = a2 * cx + b2 * cy;
 
    double determinant = a1 * b2 - a2 * b1;
 
    if (determinant != 0) {
        *x_int = (b2 * c1 - b1 * c2) / determinant;
        *y_int = (a1 * c2 - a2 * c1) / determinant;
    } else {
        // Retas paralelas
        *x_int = 0; *y_int = 0;
    }
}

bool calcular_interseccao_raio_segmento(
    double raio_x, double raio_y,
    double dest_x, double dest_y,
    double seg_x1, double seg_y1,
    double seg_x2, double seg_y2,
    double *x_int, double *y_int
) {
    calcular_interseccao(raio_x, raio_y, dest_x, dest_y, 
                         seg_x1, seg_y1, seg_x2, seg_y2, 
                         x_int, y_int);
    
    double seg_dx = seg_x2 - seg_x1;
    double seg_dy = seg_y2 - seg_y1;
    
    double t;
    if (fabs(seg_dx) > fabs(seg_dy)) {
        t = (*x_int - seg_x1) / seg_dx;
    } else if (fabs(seg_dy) > 1e-9) {
        t = (*y_int - seg_y1) / seg_dy;
    } else {
        return false;
    }
    
    const double epsilon = 1e-6;
    if (t < -epsilon || t > 1.0 + epsilon) {
        return false;
    }
    
    double ray_dx = dest_x - raio_x;
    double ray_dy = dest_y - raio_y;
    double to_int_dx = *x_int - raio_x;
    double to_int_dy = *y_int - raio_y;
    
    double dot = ray_dx * to_int_dx + ray_dy * to_int_dy;
    if (dot < -epsilon) {
        return false;
    }
    
    return true;
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

        // 1. Verifica se a coordenada Y do ponto está dentro do intervalo Y do segmento
        if (((y1 > py) != (y2 > py))) {
            
            // 2. Calcula a coordenada X onde a linha horizontal cruza o segmento
            double x_interseccao = (x2 - x1) * (py - y1) / (y2 - y1) + x1;

            // 3. Verifica se o cruzamento acontece à direita do ponto P
            if (px < x_interseccao) {
                inside = !inside;
            }
        }

        p = getNext(poligono, p);
    }

    return inside;
}