#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retangulo.h"

typedef struct{
    int id;
    float x;
    float y;
    double w;
    double h;
    double area;
    char corp[32];
    char corb[32];
}retangulo;

static double CalcArea(double w, double h){
    if(w<0 || h<0){
        return 0;
    }

    return w*h;
}

Retangulo criarRec(int id, float x, float y, double w, double h, const char* corb, const char* corp){
    retangulo* r = (retangulo*)malloc(sizeof(retangulo));
    if(r == NULL){
        return NULL;
    }

    r->id = id;
    r->x = x;
    r->y = y;
    r->w = w;
    r->h = h;
    r->area = CalcArea(w,h);
    strncpy(r->corb, corb, sizeof(r->corb) - 1);
    r->corb[sizeof(r->corb) - 1] = '\0';
    strncpy(r->corp, corp, sizeof(r->corp) - 1);
    r->corp[sizeof(r->corp) - 1] = '\0';

    return r;
}

void destruirRec(Retangulo r_g){
    retangulo* r = (retangulo*)r_g;
    if(r != NULL){
        free(r);
    }
}

float retangulo_get_x(const Retangulo r_g){
    const retangulo* r = (const retangulo*)r_g; 
    if(r == NULL)return 0;
    return r->x;
}

float retangulo_get_y(const Retangulo r_g){
    const retangulo* r = (const retangulo*)r_g; 
    if(r == NULL)return 0;
    return r->y;
}


int retangulo_get_id(const Retangulo r_g){
    const retangulo* r = (const retangulo*)r_g; 
    if(r == NULL)return -1;
    return r->id;
}

double retangulo_get_larg(const Retangulo r_g){
    const retangulo* r = (const retangulo*)r_g; 
    if(r == NULL)return 0;
    return r->w;
}

double retangulo_get_altura(const Retangulo r_g){
    const retangulo* r = (const retangulo*)r_g; 
    if(r == NULL)return 0;
    return r->h;
}

const char* retangulo_get_corp(const Retangulo r_g){
    const retangulo* r = (const retangulo*)r_g; 
    if(r == NULL)return NULL;
    return r->corp;
}

const char* retangulo_get_corb(const Retangulo r_g){
    const retangulo* r = (const retangulo*)r_g; 
    if(r == NULL)return NULL;
    return r->corb;
}

double retangulo_get_area(const Retangulo r_g){
    const retangulo* r = (const retangulo*)r_g; 
    if(r == NULL)return 0.0;
    return r->area;
}

void retangulo_set_x(Retangulo r_g, float x){
    retangulo* r = (retangulo*)r_g;
    if(r != NULL){
        r->x = x;
    }
}

void retangulo_set_y(Retangulo r_g, float y){
    retangulo* r = (retangulo*)r_g;
    if(r != NULL){
        r->y = y;
    }
}

void retangulo_set_corp(Retangulo r_g, const char* corp){
    retangulo* r = (retangulo*)r_g;
    if(r != NULL){
        strncpy(r->corp, corp, sizeof(r->corp) - 1);
        r->corp[sizeof(r->corp) - 1] = '\0';
    }
}

void retangulo_set_corb(Retangulo r_g, const char* corb){
    retangulo* r = (retangulo*)r_g;
    if(r != NULL){
        strncpy(r->corb, corb, sizeof(r->corb) - 1);
        r->corb[sizeof(r->corb) - 1] = '\0';
    }
}

void* retangulo_clonar(void* r, int novo_id, double dx, double dy) {
    double x = retangulo_get_x(r);
    double y = retangulo_get_y(r);
    double w = retangulo_get_larg(r);
    double h = retangulo_get_altura(r);
    const char* corb = retangulo_get_corb(r);
    const char* corp = retangulo_get_corp(r);

    return criarRec(novo_id, x + dx, y + dy, w, h, corb, corp);
}