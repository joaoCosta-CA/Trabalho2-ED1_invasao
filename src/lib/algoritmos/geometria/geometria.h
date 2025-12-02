#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include <stdbool.h>
#include "lista.h"

/*
 * Calcula a orientação dos pontos (A, B, C).
 * Retorno > 0: Virada à Esquerda (Sentido Anti-horário)
 * Retorno < 0: Virada à Direita (Sentido Horário)
 * Retorno = 0: Colineares
 * Fórmula: O determinante 3x3 descrita no PDF.
 */
double orientacao(double ax, double ay, double bx, double by, double cx, double cy);

/*
 * Verifica se o segmento AB intercepta o segmento CD.
 * Retorna true se houver intersecção.
 * (Útil para saber onde o raio de visão bate no segmento).
 */
bool tem_interseccao(double ax, double ay, double bx, double by, 
                     double cx, double cy, double dx, double dy);

/*
 * Retorna o PONTO de intersecção entre a reta definida por AB e o segmento CD.
 * Preenche *x_int e *y_int.
 */
void calcular_interseccao(double ax, double ay, double bx, double by, 
                          double cx, double cy, double dx, double dy,
                          double *x_int, double *y_int);

/*
 * Verifica se o ponto (px, py) está dentro do polígono definido pela lista de segmentos.
 * Retorna 1 (true) se estiver dentro, 0 (false) se fora.
 */
bool ponto_dentro_poligono(double px, double py, Lista poligono);

#endif