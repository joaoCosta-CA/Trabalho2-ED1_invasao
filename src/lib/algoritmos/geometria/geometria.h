/**
 * @file geometria.h
 * @brief Módulo de Funções Geométricas Computacionais
 * 
 * Este módulo fornece funções para cálculos geométricos fundamentais
 * utilizados no algoritmo de visibilidade. Inclui determinação de
 * orientação de pontos, detecção de interseções entre segmentos,
 * e verificação de ponto dentro de polígono.
 * 
 * @note Utiliza produto vetorial para determinação de orientação.
 * @note Implementa algoritmo ray-casting para ponto em polígono.
 * 
 * @author João Costa
 */

#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include <stdbool.h>
#include "lista.h"

/**
 * @brief Calcula a orientação de três pontos no plano.
 * 
 * Utiliza o produto vetorial para determinar a orientação relativa
 * dos pontos A, B e C (sentido horário, anti-horário ou colineares).
 * 
 * @param ax Coordenada X do ponto A
 * @param ay Coordenada Y do ponto A
 * @param bx Coordenada X do ponto B
 * @param by Coordenada Y do ponto B
 * @param cx Coordenada X do ponto C
 * @param cy Coordenada Y do ponto C
 * @return > 0: anti-horário, < 0: horário, = 0: colineares
 */
double orientacao(double ax, double ay, double bx, double by, double cx, double cy);

/**
 * @brief Verifica se dois segmentos de reta se interceptam.
 * 
 * @param ax Coordenada X do ponto inicial do segmento 1
 * @param ay Coordenada Y do ponto inicial do segmento 1
 * @param bx Coordenada X do ponto final do segmento 1
 * @param by Coordenada Y do ponto final do segmento 1
 * @param cx Coordenada X do ponto inicial do segmento 2
 * @param cy Coordenada Y do ponto inicial do segmento 2
 * @param dx Coordenada X do ponto final do segmento 2
 * @param dy Coordenada Y do ponto final do segmento 2
 * @return true se houver interseção, false caso contrário
 */
bool tem_interseccao(double ax, double ay, double bx, double by, 
                     double cx, double cy, double dx, double dy);

/**
 * @brief Calcula o ponto de interseção entre uma reta e um segmento.
 * 
 * A reta é definida pelos pontos A e B, o segmento pelos pontos C e D.
 * O ponto de interseção é retornado através dos ponteiros x_int e y_int.
 * 
 * @param ax Coordenada X do ponto A da reta
 * @param ay Coordenada Y do ponto A da reta
 * @param bx Coordenada X do ponto B da reta
 * @param by Coordenada Y do ponto B da reta
 * @param cx Coordenada X do ponto inicial do segmento
 * @param cy Coordenada Y do ponto inicial do segmento
 * @param dx Coordenada X do ponto final do segmento
 * @param dy Coordenada Y do ponto final do segmento
 * @param x_int Ponteiro para armazenar X da interseção (saída)
 * @param y_int Ponteiro para armazenar Y da interseção (saída)
 */
void calcular_interseccao(double ax, double ay, double bx, double by, 
                          double cx, double cy, double dx, double dy,
                          double *x_int, double *y_int);

/**
 * @brief Calcula interseção entre um raio e um segmento.
 * 
 * O raio parte de (raio_x, raio_y) em direção a (dest_x, dest_y).
 * Verifica se intercepta o segmento definido pelos pontos (seg_x1, seg_y1)
 * e (seg_x2, seg_y2).
 * 
 * @param raio_x Coordenada X da origem do raio
 * @param raio_y Coordenada Y da origem do raio
 * @param dest_x Coordenada X do destino do raio
 * @param dest_y Coordenada Y do destino do raio
 * @param seg_x1 Coordenada X do ponto inicial do segmento
 * @param seg_y1 Coordenada Y do ponto inicial do segmento
 * @param seg_x2 Coordenada X do ponto final do segmento
 * @param seg_y2 Coordenada Y do ponto final do segmento
 * @param x_int Ponteiro para armazenar X da interseção (saída)
 * @param y_int Ponteiro para armazenar Y da interseção (saída)
 * @return true se houver interseção válida, false caso contrário
 */
bool calcular_interseccao_raio_segmento(
    double raio_x, double raio_y,
    double dest_x, double dest_y,
    double seg_x1, double seg_y1,
    double seg_x2, double seg_y2,
    double *x_int, double *y_int
);

/**
 * @brief Verifica se um ponto está dentro de um retângulo.
 * 
 * @param px Coordenada X do ponto
 * @param py Coordenada Y do ponto
 * @param x Coordenada X do canto superior esquerdo do retângulo
 * @param y Coordenada Y do canto superior esquerdo do retângulo
 * @param w Largura do retângulo
 * @param h Altura do retângulo
 * @return 1 se está dentro, 0 se está fora
 */
int ponto_dentro_retangulo(double px, double py, double x, double y, double w, double h);

/**
 * @brief Verifica se um ponto está dentro de um polígono.
 * 
 * Utiliza o algoritmo ray-casting: traça um raio horizontal a partir
 * do ponto e conta quantas vezes cruza as arestas do polígono.
 * Número ímpar de cruzamentos = dentro; par = fora.
 * 
 * @param px Coordenada X do ponto a verificar
 * @param py Coordenada Y do ponto a verificar
 * @param poligono Lista de segmentos que formam o polígono
 * @return true se está dentro, false se está fora
 */
bool ponto_dentro_poligono(double px, double py, Lista poligono);

#endif