#define _POSIX_C_SOURCE 200809L // Necessário para strdup em alguns compiladores
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estilo_texto.h"

typedef struct {
    char *familia;
    char *peso;
    double tamanho;
} EstiloStruct;

EstiloTexto create_estilo_texto(const char* familia, const char* peso, double tamanho) {
    EstiloStruct *e = (EstiloStruct*) malloc(sizeof(EstiloStruct));
    
    if (e != NULL) {
        // Copia as strings para memória segura
        e->familia = familia ? strdup(familia) : strdup("sans-serif");
        e->peso = peso ? strdup(peso) : strdup("n");
        e->tamanho = tamanho;
    }
    
    return (EstiloTexto)e;
}

const char* get_estilo_familia(EstiloTexto e) {
    if (!e) return NULL;
    return ((EstiloStruct*)e)->familia;
}

const char* get_estilo_peso(EstiloTexto e) {
    if (!e) return NULL;
    return ((EstiloStruct*)e)->peso;
}

double get_estilo_tamanho(EstiloTexto e) {
    if (!e) return 0.0;
    return ((EstiloStruct*)e)->tamanho;
}

void destroy_estilo_texto(EstiloTexto e) {
    if (e) {
        EstiloStruct *es = (EstiloStruct*)e;
        if (es->familia) free(es->familia);
        if (es->peso) free(es->peso);
        free(es);
    }
}