#ifndef ESTILO_TEXTO_H
#define ESTILO_TEXTO_H

/* Tipo opaco */
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