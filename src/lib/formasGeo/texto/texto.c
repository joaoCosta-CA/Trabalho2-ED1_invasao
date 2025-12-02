#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texto.h"

#define TAMANHO_COR 30
#define TAMANHO_CONTEUDO 256

struct TextoStruct {
    int id;
    float x;
    float y;
    double area;
    char corb[TAMANHO_COR];
    char corp[TAMANHO_COR];
    char ancora;
    char conteudo[TAMANHO_CONTEUDO];
};

static double texto_calc_area(const Texto t){
    if(!t)return 0.0;
    struct TextoStruct* texto = (struct TextoStruct*)t;
    return 20.0 * (double)strlen(texto->conteudo);
}

Texto criar_texto(int id, float x, float y, const char* corb, const char* corp, char ancora, const char* conteudo) {
    struct TextoStruct* novo_texto = malloc(sizeof(struct TextoStruct));
    if (!novo_texto) {
        perror("Falha ao alocar memória para o texto");
        return NULL;
    }

    novo_texto->id = id;
    novo_texto->x = x;
    novo_texto->y = y;
    novo_texto->ancora = ancora;

    snprintf(novo_texto->corb, TAMANHO_COR, "%s", corb ? corb : "");
    snprintf(novo_texto->corp, TAMANHO_COR, "%s", corp ? corp : "");
    snprintf(novo_texto->conteudo, TAMANHO_CONTEUDO, "%s", conteudo ? conteudo : "");

    novo_texto->area = texto_calc_area((Texto)novo_texto);

    return novo_texto;
}

float area_texto(const Texto t) {
    if (!t) {
        return 0.0f;
    }
    const struct TextoStruct* texto_interno = (const struct TextoStruct*) t;

    return 20.0f * (float)strlen(texto_interno->conteudo);
}

void destruirTexto(Texto t) {
    Texto* texto = (Texto*)t;               
    if(texto != NULL){
        free(texto);
    }

}

int texto_get_id(const Texto t) {
    if (!t) return -1;
    struct TextoStruct* texto = (struct TextoStruct*)t;
    return texto->id;
}

float texto_get_x(const Texto t) {
    if (!t) return 0.0f;
    struct TextoStruct* texto = (struct TextoStruct*)t;
    return texto->x;
}

float texto_get_y(const Texto t) {
    if (!t) return 0.0f;
    struct TextoStruct* texto = (struct TextoStruct*)t;
    return texto->y;
}

double texto_get_area(const Texto t) {
    if (!t) return 0.0;
    struct TextoStruct* texto = (struct TextoStruct*)t;
    return texto->area;
}

char texto_get_ancora(const Texto t) {
    if (!t) return '\0';
    
    struct TextoStruct* texto = (struct TextoStruct*)t;
    
    return texto->ancora;
}

const char* texto_get_corb(const Texto t) {
    if (!t) return NULL;
    struct TextoStruct* texto = (struct TextoStruct*)t;
    return texto->corb;
}

const char* texto_get_corp(const Texto t) {
    if (!t) return NULL;
    struct TextoStruct* texto = (struct TextoStruct*)t;
    return texto->corp;
}

const char* texto_get_conteudo(const Texto t) {
    if (!t) return NULL;
    struct TextoStruct* texto = (struct TextoStruct*)t;
    return texto->conteudo;
}

void texto_set_corb(Texto t, const char* corb){
    if (!t || !corb) return;
    struct TextoStruct* texto = (struct TextoStruct*)t;
    snprintf(texto->corb, TAMANHO_COR, "%s", corb);
}

void texto_set_corp(Texto t, const char* corp){
    if (!t || !corp) return;
    struct TextoStruct* texto = (struct TextoStruct*)t;
    snprintf(texto->corp, TAMANHO_COR, "%s", corp);
}

void texto_set_conteudo(Texto t, const char* conteudo){
    if (!t || !conteudo) return;
    struct TextoStruct* texto = (struct TextoStruct*)t;
    snprintf(texto->conteudo, TAMANHO_CONTEUDO, "%s", conteudo);
}

void texto_set_x(Texto t, float x){
    if (!t) return;
    struct TextoStruct* texto = (struct TextoStruct*)t;
    texto->x = x;
}

void texto_set_y(Texto t, float y){
    if (!t) return;
    struct TextoStruct* texto = (struct TextoStruct*)t;
    texto->y = y;
}

void* texto_clonar(void* t, int novo_id, double dx, double dy) {
    double x = texto_get_x(t);
    double y = texto_get_y(t);
    const char* corb = texto_get_corb(t);
    const char* corp = texto_get_corp(t);
    char ancora = texto_get_ancora(t);
    const char* conteudo = texto_get_conteudo(t);

    return criar_texto(novo_id, x + dx, y + dy, corb, corp, ancora, conteudo);
}