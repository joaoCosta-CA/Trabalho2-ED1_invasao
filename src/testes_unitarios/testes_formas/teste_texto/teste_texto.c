#include <stdio.h>
#include "texto.h"


int main(){
    printf("Teste do texto iniciado....\n");
    int id = 1;
    float x = 10.0, y = 15.0;
    const char *corborda = "purple";
    const char *corpreenchimento = "yellow";
    char ancora = 'n';
    const char *conteudo = "Texto de teste";

    Texto t = criar_texto(id, x, y, corborda, corpreenchimento, ancora, conteudo);
    if(t == NULL){
        printf("Falha ao criar o texto.\n");
        return 1;
    }
    printf("Texto: cordenadas -> (%2f, %2f), borda -> %s, preenchimento -> %s, ancora -> %c, conteudo -> %s\n",
           texto_get_x(t), texto_get_y(t),
           texto_get_corb(t), texto_get_corp(t),
           texto_get_ancora(t), texto_get_conteudo(t));
    destruirTexto(t);
    return 0;
}