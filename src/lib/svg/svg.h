#ifndef SVG_H
#define SVG_H

#include "lista.h"


/*
 * Recebe a lista de formas (já populada) e o caminho onde o arquivo será salvo.
 * Gera um arquivo SVG válido desenhando todas as formas da lista.
 */
void gerar_svg(Lista formas, const char *caminho_arquivo);


#endif