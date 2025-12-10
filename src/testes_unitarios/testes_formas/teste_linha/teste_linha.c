#include <stdio.h>
#include "linha.h"


int main(){
    printf("Teste da linha iniciado....\n");
    int id = 1;
    float x1 = 10.0, y1 = 15.0;
    float x2 = 20.0, y2 = 25.0;
    const char *cor = "green";

    Linha l = criarLinha(id, x1, y1, x2, y2, cor);
    if(l == NULL){
        printf("Falha ao criar a linha.\n");
        return 1;
    }
    printf("Linha: cordenadas -> (%.2f, %.2f) a (%.2f, %.2f) cor -> %s\n",
           linha_get_x1(l), linha_get_y1(l), linha_get_x2(l), linha_get_y2(l), linha_get_cor(l));
    destruirLinha(l);
    return 0;
}