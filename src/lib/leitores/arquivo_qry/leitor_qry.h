/**
 * @file leitor_qry.h
 * @brief Módulo de Processamento de Arquivos .qry (Consultas)
 * 
 * Este módulo implementa o parser e executor de comandos do arquivo .qry.
 * Cada comando manipula as formas geométricas e anteparos, gerando
 * relatórios em .txt e visualizações em .svg.
 * 
 * Comandos principais suportados:
 * - a: Transforma formas em anteparos
 * - b: Calcula polígono de visibilidade (bomba)
 * - d: Remove formas atingidas pela explosão
 * - dsp: Dispara formas em padrão de cruz
 * - rjd: Dispara formas em padrão diagonal
 * - cln: Clona anteparos atingidos pela explosão
 * 
 * @note Comandos 'd' e 'cln' utilizam o polígono de visibilidade calculado.
 * @note Gera arquivos de saída: nome_base-nome_qry.txt e .svg
 * 
 * @author João Costa
 */

#ifndef LEITOR_QRY_H
#define LEITOR_QRY_H

#include "lista.h"

/**
 * @brief Processa um arquivo de consultas (.qry).
 * 
 * Interpreta e executa os comandos do arquivo .qry, manipulando
 * as formas geométricas e anteparos conforme especificado. Gera
 * arquivos de saída (.txt e .svg) com resultados.
 * 
 * @param path_qry Caminho completo do arquivo .qry a processar
 * @param output_dir Diretório onde salvar os arquivos de saída
 * @param nome_base_geo Nome base do arquivo .geo (sem extensão)
 * @param lista_formas Lista de formas do .geo (será modificada)
 * @param tipo_ord Tipo de ordenação: 'q' (qsort), 'i' (insert), 'm' (merge)
 * @param cutoff Limite para uso de insertion sort no merge híbrido
 */
void processar_qry(const char *path_qry, const char *output_dir, const char *nome_base_geo, 
                   Lista lista_formas, char tipo_ord, int cutoff);

#endif