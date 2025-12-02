#ifndef SEGMENTO_H
#define SEGMENTO_H

typedef void* Segmento;

/* Cria um segmento (anteparo) */
Segmento create_segmento(int id, double x1, double y1, double x2, double y2);

/* Getters */
int get_segmento_id(Segmento s);
double get_segmento_x1(Segmento s);
double get_segmento_y1(Segmento s);
double get_segmento_x2(Segmento s);
double get_segmento_y2(Segmento s);

/* Destrutor */
void destroy_segmento(Segmento s);

#endif