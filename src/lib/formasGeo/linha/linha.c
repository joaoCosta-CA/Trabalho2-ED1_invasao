#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "linha.h"

typedef struct{
    int id;
    float x1;
    float y1;
    float x2;
    float y2;
    double area;
    char cor[32];
}linha;

static double CalcArea(const linha* l){
    // Calcula o comprimento usando a fórmula da distância euclidiana
    double dx = l->x2 - l->x1;
    double dy = l->y2 - l->y1;
    double comprimento = sqrt(pow(dx, 2) + pow(dy, 2));

    return 2.0 * comprimento;
}

Linha criarLinha(int id, float x1, float y1, float x2, float y2, const char* cor){
    linha* l = (linha*)malloc(sizeof(linha));
    if(l == NULL){
        return NULL;
    }

    l->id = id;
    l->x1 = x1;
    l->y1 = y1;
    l->x2 = x2;
    l->y2 = y2;
    l->area = CalcArea(l);
    strncpy(l->cor, cor, sizeof(l->cor) - 1);
    l->cor[sizeof(l->cor) - 1] = '\0';
    
    return l;
}

void destruirLinha(Linha l_g){
    linha* l = (linha*)l_g;
    if(l != NULL){
        free(l);
    }
}

int linha_get_id(const Linha l_g){
    const linha* l = (const linha*)l_g;
    if (l == NULL)return -1;
    return l->id;
}

float linha_get_x1(const Linha l_g){
    const linha* l = (const linha*)l_g;
    if(l == NULL)return 0;
    return l->x1;
}
float linha_get_x2(const Linha l_g){
    const linha* l = (const linha*)l_g;
    if(l == NULL)return 0;
    return l->x2;
}

float linha_get_y1(const Linha l_g){
    const linha* l = (const linha*)l_g;
    if(l == NULL)return 0;
    return l->y1;
}

float linha_get_y2(const Linha l_g){
    const linha* l = (const linha*)l_g;
    if(l == NULL)return 0;
    return l->y2;
}

double linha_get_area(const Linha l_g){
    const linha* l = (const linha*)l_g;
    if(l == NULL)return 0;
    return l->area;
}

const char* linha_get_cor(const Linha l_g){
    const linha* l = (const linha*)l_g;
    if(l == NULL) return NULL;
    return l->cor;
}

void linha_set_x1(Linha l_g, float x1){
    linha* l = (linha*)l_g;
    if(l != NULL){
        l->x1 = x1;
    }
}

void linha_set_y1(Linha l_g, float y1){
    linha* l = (linha*)l_g;
    if(l != NULL){
        l->y1 = y1;
    }
}

void linha_set_x2(Linha l_g, float x2){
    linha* l = (linha*)l_g;
    if(l != NULL){
        l->x2 = x2;
    }
}

void linha_set_y2(Linha l_g, float y2){
    linha* l = (linha*)l_g;
    if(l != NULL){
        l->y2 = y2;
    }
}


void linha_set_cor(Linha l_g, const char* cor){
    linha* l = (linha*)l_g;
    if(l != NULL){
        strncpy(l->cor, cor, sizeof(l->cor) - 1);
        l->cor[sizeof(l->cor) - 1] = '\0';
    }
}

void* linha_clonar(void* l, int novo_id, double dx, double dy) {
    double x1 = linha_get_x1(l);
    double y1 = linha_get_y1(l);
    double x2 = linha_get_x2(l);
    double y2 = linha_get_y2(l);
    const char* cor = linha_get_cor(l);

    return criarLinha(novo_id, x1 + dx, y1 + dy, x2 + dx, y2 + dy, cor);
}