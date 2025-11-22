#ifndef LEITOR_GEO_H
#define LEITOR_GEO_H

#include "../../estruturas_dados/lista/lista.h"

/*
 * Lê o arquivo .geo especificado e insere as formas na lista.
 * Atualiza o valor de max_id com o maior ID encontrado.
 */
void ler_arquivo_geo(const char *diretorio_base, const char *nome_arquivo, Lista formas, int *max_id);

#endif