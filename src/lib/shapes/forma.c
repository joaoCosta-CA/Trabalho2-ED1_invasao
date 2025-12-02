#include <stdlib.h>
#include <stdio.h>

#include "forma.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "estilo_texto.h"

typedef struct {
    ShapeType tipo;
    void *dados;
} FormaStruct;

Forma create_forma(ShapeType tipo, void *dados_figura) {
    FormaStruct *f = (FormaStruct *) malloc(sizeof(FormaStruct));
    f->tipo = tipo;
    f->dados = dados_figura;
    return (Forma) f;
}

ShapeType get_tipo_forma(Forma f) {
    FormaStruct *fs = (FormaStruct *) f;
    return fs->tipo;
}

void* get_dados_forma(Forma f) {
    FormaStruct *fs = (FormaStruct *) f;
    return fs->dados;
}

void destroy_forma(Forma f) {
    FormaStruct *fs = (FormaStruct *) f;
    
    switch (fs->tipo) {
        case CIRCULO:
            destruirCirc(fs->dados); 
            break;
            
        case RETANGULO:
            destruirRec(fs->dados);
            break;
            
        case LINHA:
            destruirLinha(fs->dados);
            break;
            
        case TEXTO:
            destruirTexto(fs->dados);
            break;

        case ESTILO_TEXTO:
            destroy_estilo_texto(fs->dados);
            break;
    }

    
    free(fs);
}

int get_forma_id_generico(Forma f) {
    if (!f) return -1;
    FormaStruct *fs = (FormaStruct *)f;
    
    // Você precisa ter os getters específicos implementados nos módulos
    switch (fs->tipo) {
        case CIRCULO:   return circulo_get_id(fs->dados);
        case RETANGULO: return retangulo_get_id(fs->dados);
        case LINHA:     return linha_get_id(fs->dados);
        case TEXTO:     return texto_get_id(fs->dados);
        default: return -1;
    }
}

/* ... includes anteriores ... */

void forma_atualizar_cor(Forma f, const char *cor) {
    if (!f) return;
    FormaStruct *fs = (FormaStruct *)f;

    switch(fs->tipo) {
        case CIRCULO:
            circulo_set_corb(fs->dados, cor);
            circulo_set_corp(fs->dados, cor);
            break;
        case RETANGULO:
            retangulo_set_corb(fs->dados, cor);
            retangulo_set_corp(fs->dados, cor);
            break;
        case LINHA:
            // Linha geralmente só tem uma cor
            linha_set_cor(fs->dados, cor);
            break;
        case TEXTO:
            texto_set_corb(fs->dados, cor);
            texto_set_corp(fs->dados, cor);
            break;
        case ESTILO_TEXTO:
            // Estilos não são pintados individualmente dessa forma
            break;
        default:
            break;
    }
}

Forma forma_clonar(Forma f, int novo_id, double dx, double dy) {
    if (!f) return NULL;
    FormaStruct *fs = (FormaStruct *)f;
    void *novos_dados = NULL;

    switch(fs->tipo) {
        case CIRCULO:
            novos_dados = circulo_clonar(fs->dados, novo_id, dx, dy);
            break;
        case RETANGULO:
            novos_dados = retangulo_clonar(fs->dados, novo_id, dx, dy);
            break;
        case LINHA:
            novos_dados = linha_clonar(fs->dados, novo_id, dx, dy);
            break;
        case TEXTO:
            novos_dados = texto_clonar(fs->dados, novo_id, dx, dy);
            break;
        case ESTILO_TEXTO:
            // Geralmente não clonamos definições de estilo via comando 'cln' geométrico
            // Mas se precisar, implemente estilo_clonar
            return NULL; 
        default:
            return NULL;
    }

    if (novos_dados) {
        return create_forma(fs->tipo, novos_dados);
    }
    
    // CORREÇÃO DO WARNING "control reaches end":
    return NULL; 
}