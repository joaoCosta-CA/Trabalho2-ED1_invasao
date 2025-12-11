/**
 * @file svg.h
 * @brief Módulo de Geração de Arquivos SVG
 * 
 * Este módulo implementa a geração de arquivos Scalable Vector Graphics (SVG)
 * para visualização das formas geométricas, anteparos, polígono de visibilidade
 * e pontos de bomba. O SVG é gerado em camadas ordenadas para correta sobreposição.
 * 
 * Camadas (da mais baixa para a mais alta):
 * 1. Formas geométricas coloridas (círculos, retângulos, etc.)
 * 2. Polígono de visibilidade (preenchimento vermelho semi-transparente)
 * 3. Anteparos (linhas pretas)
 * 4. Pontos de bomba (círculos amarelos)
 * 
 * @note O viewBox é calculado automaticamente para incluir todos elementos.
 * 
 * @author João Costa
 */

#ifndef SVG_WRITER_H
#define SVG_WRITER_H

#include "lista.h"

/**
 * @brief Gera um arquivo SVG com as camadas visuais.
 * 
 * Cria um arquivo SVG contendo as formas geométricas, anteparos,
 * polígono de visibilidade e pontos de bomba em camadas ordenadas.
 * O viewBox é calculado automaticamente para incluir todos elementos.
 * 
 * @param formas Lista de formas geométricas (pode ser NULL)
 * @param anteparos Lista de segmentos de anteparo (pode ser NULL)
 * @param poligono Lista de segmentos do polígono de visibilidade (pode ser NULL)
 * @param pontos_bombas Lista de vetores double[2] com coordenadas {x, y} (pode ser NULL)
 * @param caminho_arquivo Caminho completo do arquivo SVG a criar
 */
void gerar_svg(Lista formas, Lista anteparos, Lista poligono, 
               Lista pontos_bombas, const char *caminho_arquivo);

// Versão para múltiplos polígonos (lista de listas de pontos)
void gerar_svg_multi(Lista formas, Lista anteparos, Lista lista_poligonos, 
                     Lista pontos_bombas, const char *caminho_arquivo);

#endif