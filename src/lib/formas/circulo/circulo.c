#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circulo.h"
#define pi 3.141592653589793

typedef struct{
    int id;
    float x;
    float y;
    double r;
    double area;
    char corp[32];
    char corb[32];
}circulo;

static double CalcArea(double r){
    if(r<0){
        return 0;
    }
    return pi * r * r;
}


Circulo CriarCirc(int id, float x, float y, double r, const char* corb, const char* corp){
    circulo* c = (circulo*)malloc(sizeof(circulo));
    if(c == NULL){
        return NULL;
    }
    c->id = id;
    c->x = x;
    c->y = y;
    c->r = r;
    c->area = CalcArea(c->r);
    strncpy(c->corb, corb, sizeof(c->corb) - 1);
    c->corb[sizeof(c->corb) - 1] = '\0';
    strncpy(c->corp, corp, sizeof(c->corp) - 1);
    c->corp[sizeof(c->corp) - 1] = '\0';

    return c;
}

void DestruirCirc(Circulo c_g){
    circulo* c = (circulo*)c_g;
    if(c != NULL){
        free(c);
    }
    return;
}

int circulo_get_id(const Circulo c_g) {
    const circulo* c = (const circulo*)c_g;
    return (c != NULL) ? c->id : -1;
}

float circulo_get_x(const Circulo c_g) {
    const circulo* c = (const circulo*)c_g;
    return (c != NULL) ? c->x : 0;
}

float circulo_get_y(const Circulo c_g) {
    const circulo* c = (const circulo*)c_g;
    return (c != NULL) ? c->y : 0;
}

double circulo_get_raio(const Circulo c_g) {
    const circulo* c = (const circulo*)c_g;
    return (c != NULL) ? c->r : 0.0;
}

double circulo_get_area(const Circulo c_g) {
    const circulo* c = (const circulo*)c_g;
    return (c != NULL) ? c->area : 0.0;
}

const char* circulo_get_corb(const Circulo c_g) {
    const circulo* c = (const circulo*)c_g;
    return (c != NULL) ? c->corb : "invalido";
}

const char* circulo_get_corp(const Circulo c_g) {
    const circulo* c = (const circulo*)c_g;
    return (c != NULL) ? c->corp : "invalido";
}

void circulo_set_x(Circulo c_g, float x){
    circulo* c = (circulo*)c_g;
    if(c != NULL){
        c->x = x;
    };
}

void circulo_set_y(Circulo c_g, float y){
    circulo* c = (circulo*)c_g;
    if(c != NULL){
        c->y = y;
    }
}

void circulo_set_corp(Circulo c_g, const char* corP){
    circulo* c = (circulo*)c_g;
    if(c != NULL && corP != NULL){
        strncpy(c->corp, corP, sizeof(c->corp) - 1);
        c->corp[sizeof(c->corp) - 1] = '\0';
    }
}

void circulo_set_corb(Circulo c_g, const char* corB){
    circulo* c = (circulo*)c_g;
    if(c != NULL && corB != NULL){
        strncpy(c->corb, corB, sizeof(c->corb) - 1);
        c->corb[sizeof(c->corb) - 1] = '\0';
    }
}