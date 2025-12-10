/**
 * @file transformacao.h
 * @brief Módulo de Transformação de Formas em Anteparos
 * 
 * Este módulo implementa a conversão de formas geométricas (círculos,
 * retângulos, linhas) em segmentos de anteparo. Os círculos podem ser
 * convertidos em polígonos regulares (inscrito ou circunscrito).
 * 
 * @note Utilizado pelo comando 'a' do arquivo .qry.
 * @note Retângulos são convertidos em 4 segmentos (lados).
 * @note Círculos são aproximados por polígonos de N lados.
 * 
 * @author João Costa
 */

#ifndef TRANSFORMACAO_H
#define TRANSFORMACAO_H

#include "segmento.h"
#include "lista.h"
#include "forma.h"

/**
 * @brief Transforma uma forma geométrica em segmentos de anteparo.
 * 
 * Converte círculos, retângulos ou linhas em um ou mais segmentos
 * de linha que servirão como barreiras visuais.
 * 
 * @param f Forma a transformar
 * @param flag_circulo Tipo de aproximação para círculos ('i'=inscrito, 'c'=circunscrito)
 * @param id_seg_counter Ponteiro para contador de IDs de segmento (será incrementado)
 * @return Lista de segmentos gerados, ou NULL se tipo não suportado
 */
Lista transformar_em_anteparos(Forma f, char flag_circulo, int *id_seg_counter);

#endif