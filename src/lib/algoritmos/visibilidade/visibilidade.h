/**
 * @file visibilidade.h
 * @brief Módulo de Cálculo de Polígono de Visibilidade
 * 
 * Este módulo implementa o algoritmo de varredura angular (angular sweep)
 * para cálculo do polígono de visibilidade a partir de um ponto (bomba).
 * O algoritmo processa eventos de início/fim de segmentos ordenados por
 * ângulo, mantendo uma árvore de segmentos ativos para determinar a
 * visibilidade em cada direção.
 * 
 * @note Suporta diferentes algoritmos de ordenação (qsort, merge, insert).
 * @note Cria paredes temporárias do bbox para delimitar a área de visão.
 * 
 * @author João Costa
 */

#ifndef VISIBILIDADE_H
#define VISIBILIDADE_H

#include "lista.h"
#include "limites.h"

/**
 * @brief Tipo opaco para vértice de segmento.
 */
typedef void* Vertice;

/**
 * @brief Enumeração dos tipos de vértice (evento).
 */
typedef enum {
    TIPO_INICIO = 0,  /**< Vértice de início do segmento */
    TIPO_FIM = 1      /**< Vértice de fim do segmento */
} TipoVertice;

/**
 * @brief Prepara e ordena vértices por ângulo para varredura angular.
 * 
 * Converte cada segmento em dois vértices (início e fim), calcula
 * o ângulo de cada vértice em relação ao centro e ordena o vetor.
 * 
 * @param centro_x Coordenada X do ponto central (bomba)
 * @param centro_y Coordenada Y do ponto central (bomba)
 * @param lista_segmentos Lista de segmentos a processar
 * @param qtd_out Ponteiro para retornar quantidade de vértices (saída)
 * @param tipo_ord Algoritmo de ordenação: 'q', 'i' ou 'm'
 * @param cutoff Parâmetro cutoff para merge sort híbrido
 * @return Vetor de vértices ordenados por ângulo (alocar com free)
 */
Vertice* preparar_vertices_ordenados(double centro_x, double centro_y, 
                                     Lista lista_segmentos, int *qtd_out,
                                     char tipo_ord, int cutoff);

/**
 * @brief Calcula o polígono de visibilidade a partir de um ponto.
 * 
 * Implementa o algoritmo de varredura angular (angular sweep) para
 * determinar a área visível a partir do ponto (x, y).
 * 
 * @param x Coordenada X do ponto de observação (bomba)
 * @param y Coordenada Y do ponto de observação (bomba)
 * @param anteparos Lista de segmentos que bloqueiam visão
 * @param box_mundo Bounding box para criar paredes temporárias
 * @param tipo_ord Algoritmo de ordenação: 'q', 'i' ou 'm'
 * @param cutoff Parâmetro cutoff para merge sort híbrido
 * @return Lista de segmentos que formam o polígono de visibilidade
 */
Lista calcular_visibilidade(double x, double y, Lista anteparos, 
                            Limites box_mundo, char tipo_ord, int cutoff);

/**
 * @brief Obtém coordenada X do vértice.
 * @param v Vértice a consultar
 * @return Coordenada X
 */
double get_vertice_x(Vertice v);

/**
 * @brief Obtém coordenada Y do vértice.
 * @param v Vértice a consultar
 * @return Coordenada Y
 */
double get_vertice_y(Vertice v);

/**
 * @brief Obtém ângulo do vértice em relação ao centro.
 * @param v Vértice a consultar
 * @return Ângulo em radianos
 */
double get_vertice_angulo(Vertice v);

/**
 * @brief Obtém o segmento associado ao vértice.
 * @param v Vértice a consultar
 * @return Ponteiro para o segmento
 */
void* get_vertice_segmento(Vertice v);

/**
 * @brief Obtém o tipo do vértice (início ou fim).
 * @param v Vértice a consultar
 * @return TIPO_INICIO (0) ou TIPO_FIM (1)
 */
int get_vertice_tipo(Vertice v);

/**
 * @brief Libera memória de um vértice.
 * @param v Vértice a destruir
 */
void destroy_vertice(Vertice v);

/**
 * @brief Libera memória de um vetor de vértices.
 * @param vetor Vetor de vértices a destruir
 * @param qtd Quantidade de vértices no vetor
 */
void destruir_vetor_vertices(Vertice *vetor, int qtd);

/**
 * @brief Define o contexto da bomba para comparações.
 * @param x Coordenada X da bomba
 * @param y Coordenada Y da bomba
 */
void set_contexto_bomba(double x, double y);

/**
 * @brief Função de comparação de segmentos para árvore.
 * @param a Primeiro segmento
 * @param b Segundo segmento
 * @return < 0 se a mais próximo, > 0 se b mais próximo
 */
int comparar_segmentos_arvore(const void* a, const void* b);

#endif