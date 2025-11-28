#include <stdio.h>
#include "retangulo.h"


int main(){
    printf("Teste do retangulo iniciado....\n");
    int id = 1;
    float x = 10.0, y = 15.0;
    double largura = 20.0, altura = 10.0;
    const char *corborda = "orange";
    const char *corpreenchimento = "black";

    Retangulo r = criarRec(id, x, y, largura, altura, corborda, corpreenchimento);
    if(r == NULL){
        printf("Falha ao criar o retangulo.\n");
        return 1;
    }
    printf("Retangulo: cordenadas -> (%2f, %2f), largura -> %2lf, altura -> %2lf, borda -> %s, preenchimento -> %s\n",
           retangulo_get_x(r), retangulo_get_y(r), retangulo_get_larg(r), retangulo_get_altura(r),
           retangulo_get_corb(r), retangulo_get_corp(r));
    destruirRec(r);
    return 0;
}