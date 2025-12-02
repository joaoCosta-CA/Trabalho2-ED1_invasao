#ifndef SVG_WRITER_H
#define SVG_WRITER_H

#include "lista.h"

/*
 * Gera um arquivo SVG contendo as camadas fornecidas.
 * * Parâmetros:
 * - formas: Lista de formas originais (Pode ser NULL)
 * - anteparos: Lista de segmentos de barreira (Pode ser NULL)
 * - poligono: Lista de segmentos da visibilidade (Pode ser NULL)
 * - bx, by: Coordenadas do centro da bomba (se poligono != NULL)
 * - caminho_arquivo: Destino do arquivo
 */
void gerar_svg(Lista formas, Lista anteparos, Lista poligono, 
               double bx, double by, const char *caminho_arquivo);

#endif