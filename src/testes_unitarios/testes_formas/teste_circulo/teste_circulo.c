#include <stdio.h>
#include "circulo.h"

int main(){
    printf("Teste de circulo iniciado.\n");
    int id = 1;
    float x = 10.0, y = 15.0;
    double raio = 5.0;
    const char *corborda = "red";
    const char *corpreenchimento = "blue";

    Circulo c = CriarCirc(id, x, y, raio, corborda, corpreenchimento);
    if(c == NULL){
        printf("Falha ao criar o circulo.\n");
        return 1;
    }
    printf("Circulo: cordenadas -> (%2f, %2f), raio -> %2f, borda -> %s, preenchimento -> %s\n",
           circulo_get_x(c), circulo_get_y(c), circulo_get_raio(c),
           circulo_get_corb(c), circulo_get_corp(c));
    destruirCirc(c);
    return 0;
}