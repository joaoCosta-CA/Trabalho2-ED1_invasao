/**
 * @file limites.h
 * @brief Módulo de Cálculo de Limites do Mundo (Bounding Box)
 * 
 * Este módulo implementa o cálculo e gerenciamento de bounding box (caixa
 * delimitadora) para o conjunto de formas e segmentos. Utilizado para
 * definir o viewBox do SVG e criar as paredes temporárias do algoritmo
 * de visibilidade.
 * 
 * @note Suporta expansão incremental do bbox com novos pontos e segmentos.
 * @note Valores iniciais são extremos invertidos (DBL_MAX/-DBL_MAX).
 * 
 * @author João Costa
 */

#ifndef LIMITES_H
#define LIMITES_H

#include "lista.h"

/**
 * @brief Tipo opaco para estrutura de limites (bounding box).
 */
typedef void* Limites;

/**
 * @brief Calcula o bounding box de uma lista de formas.
 * 
 * Percorre todas as formas da lista e determina os valores
 * mínimo e máximo para X e Y (extremos do mundo).
 * 
 * @param formas Lista de formas geométricas
 * @return Estrutura Limites com os extremos calculados
 * @note O chamador deve usar destruir_limites() para liberar
 */
Limites calcular_limites_mundo(Lista formas);

/**
 * @brief Expande os limites para incluir um ponto específico.
 * 
 * @param l Estrutura de limites a expandir
 * @param x Coordenada X do ponto
 * @param y Coordenada Y do ponto
 */
void limites_expandir_ponto(Limites l, double x, double y);

/**
 * @brief Expande os limites para incluir todos os segmentos.
 * 
 * @param l Estrutura de limites a expandir
 * @param segmentos Lista de segmentos a incluir
 */
void limites_expandir_segmentos(Limites l, Lista segmentos);

/**
 * @brief Obtém coordenada X mínima.
 * @param l Estrutura de limites
 * @return Valor mínimo de X
 */
double get_limites_min_x(Limites l);

/**
 * @brief Obtém coordenada Y mínima.
 * @param l Estrutura de limites
 * @return Valor mínimo de Y
 */
double get_limites_min_y(Limites l);

/**
 * @brief Obtém coordenada X máxima.
 * @param l Estrutura de limites
 * @return Valor máximo de X
 */
double get_limites_max_x(Limites l);

/**
 * @brief Obtém coordenada Y máxima.
 * @param l Estrutura de limites
 * @return Valor máximo de Y
 */
double get_limites_max_y(Limites l);

/**
 * @brief Libera memória da estrutura de limites.
 * @param l Estrutura a destruir
 */
void destruir_limites(Limites l);

#endif