#ifndef LIMITES_H
#define LIMITES_H

#include "lista.h"

/* Tipo Opaco (Ponteiro para a estrutura oculta) */
typedef void* Limites;

/* * Cria a estrutura, percorre a lista, calcula os extremos e retorna o objeto.
 * O usuário é responsável por chamar destruir_limites() depois.
 */
Limites calcular_limites_mundo(Lista formas);

/* Getters para acessar os dados */
double get_limites_min_x(Limites l);
double get_limites_min_y(Limites l);
double get_limites_max_x(Limites l);
double get_limites_max_y(Limites l);

/* Destrutor */
void destruir_limites(Limites l);

#endif