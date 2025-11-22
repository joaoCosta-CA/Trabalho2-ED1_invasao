#include <stdlib.h>
#include "formas.h"

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
    // Atenção: Quem chama destroy_forma deve saber se precisa dar free no 'dados' antes
    // ou se o 'dados' será liberado pela struct específica (ex: DestruirCirc)
    free(fs);
}