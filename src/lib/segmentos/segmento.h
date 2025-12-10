/**
 * @file segmento.h
 * @brief Módulo de Segmento de Linha (Anteparo)
 * 
 * Este módulo implementa a estrutura de dados para representar segmentos de
 * linha bidimensionais. Os segmentos são utilizados como anteparos (barreiras)
 * no sistema de visibilidade, definidos por dois pontos (x1,y1) e (x2,y2).
 * 
 * @note Cada segmento possui um ID único para identificação.
 * @note Os segmentos são fundamentais para o cálculo do polígono de visibilidade.
 * 
 * @author João Costa
 */

#ifndef SEGMENTO_H
#define SEGMENTO_H

/**
 * @brief Tipo opaco para segmento de linha.
 */
typedef void* Segmento;

/**
 * @brief Cria um novo segmento de linha.
 * @param id Identificador único do segmento
 * @param x1 Coordenada X do primeiro ponto
 * @param y1 Coordenada Y do primeiro ponto
 * @param x2 Coordenada X do segundo ponto
 * @param y2 Coordenada Y do segundo ponto
 * @return Ponteiro para o segmento criado
 */
Segmento create_segmento(int id, double x1, double y1, double x2, double y2);

/**
 * @brief Obtém o ID do segmento.
 * @param s Segmento a consultar
 * @return Identificador do segmento
 */
int get_segmento_id(Segmento s);

/**
 * @brief Obtém coordenada X do primeiro ponto.
 * @param s Segmento a consultar
 * @return Coordenada X1
 */
double get_segmento_x1(Segmento s);

/**
 * @brief Obtém coordenada Y do primeiro ponto.
 * @param s Segmento a consultar
 * @return Coordenada Y1
 */
double get_segmento_y1(Segmento s);

/**
 * @brief Obtém coordenada X do segundo ponto.
 * @param s Segmento a consultar
 * @return Coordenada X2
 */
double get_segmento_x2(Segmento s);

/**
 * @brief Obtém coordenada Y do segundo ponto.
 * @param s Segmento a consultar
 * @return Coordenada Y2
 */
double get_segmento_y2(Segmento s);

/**
 * @brief Libera memória do segmento.
 * @param s Segmento a destruir
 */
void destroy_segmento(Segmento s);

#endif