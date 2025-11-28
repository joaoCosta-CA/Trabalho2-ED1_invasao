#include <stdlib.h>
#include <stdio.h>

#include "forma.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

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
            printf("Tipo não criado ainda.\n");
            break;
    }

    
    free(fs);
}