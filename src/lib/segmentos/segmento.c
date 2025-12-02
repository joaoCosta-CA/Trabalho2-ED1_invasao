#include <stdlib.h>
#include "segmento.h"

typedef struct {
    int id;
    double x1, y1;
    double x2, y2;
} SegmentoStruct;

Segmento create_segmento(int id, double x1, double y1, double x2, double y2) {
    SegmentoStruct* s = malloc(sizeof(SegmentoStruct));
    s->id = id;
    s->x1 = x1; s->y1 = y1;
    s->x2 = x2; s->y2 = y2;
    return (Segmento)s;
}

int get_segmento_id(Segmento s) { return ((SegmentoStruct*)s)->id; }
double get_segmento_x1(Segmento s) { return ((SegmentoStruct*)s)->x1; }
double get_segmento_y1(Segmento s) { return ((SegmentoStruct*)s)->y1; }
double get_segmento_x2(Segmento s) { return ((SegmentoStruct*)s)->x2; }
double get_segmento_y2(Segmento s) { return ((SegmentoStruct*)s)->y2; }

void destroy_segmento(Segmento s) { free(s); }