/**
 * @file estilo_texto.h
 * @brief Módulo de Estilo de Texto para SVG
 * 
 * Este módulo implementa a estrutura de dados para armazenar configurações
 * de estilo de texto utilizadas na renderização SVG. Define família da fonte,
 * peso (normal/bold) e tamanho.
 * 
 * @note Utilizado pelo comando 'ts' do arquivo .geo.
 * @note O estilo é aplicado a todos os textos subsequentes.
 * 
 * @author João Costa
 */

#ifndef ESTILO_TEXTO_H
#define ESTILO_TEXTO_H

/**
 * @brief Tipo opaco para estilo de texto.
 */
typedef void* EstiloTexto;

/*
 * Cria um objeto de estilo de texto.
 * familia: "sans", "serif", "cursive", etc.
 * peso: 'n' (normal), 'b' (bold), etc.
 * tamanho: tamanho da fonte em pixels/pontos.
 */
EstiloTexto create_estilo_texto(const char* familia, const char* peso, double tamanho);

/* Getters */
const char* get_estilo_familia(EstiloTexto e);
const char* get_estilo_peso(EstiloTexto e); // Retorna string pois peso pode ser "b+" ou "n"
double get_estilo_tamanho(EstiloTexto e);

/* Destrutor */
void destroy_estilo_texto(EstiloTexto e);

#endif