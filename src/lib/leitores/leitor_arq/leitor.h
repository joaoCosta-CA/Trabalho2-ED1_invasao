#ifndef LEITOR_H
#define LEITOR_H

#include "lista.h"

/* Tipo opaco para os dados do arquivo */
typedef void *DadosArquivo;

/* * Abre o arquivo, lê todas as linhas e armazena em uma lista.
 * Retorna a estrutura DadosArquivo ou NULL em caso de erro.
 */
DadosArquivo dados_arquivo_criar(const char *caminho_arquivo);

/* Destrói a estrutura e libera a memória de todas as linhas lidas */
void dados_arquivo_destruir(DadosArquivo dados);

/* Retorna apenas o nome do arquivo (sem o caminho) */
const char *obter_nome_arquivo(DadosArquivo dados);

/* Retorna a LISTA contendo as linhas (strings char*) do arquivo */
Lista leitor_obter_linhas(DadosArquivo dados);

#endif