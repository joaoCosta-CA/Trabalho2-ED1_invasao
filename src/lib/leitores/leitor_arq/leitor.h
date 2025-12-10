/**
 * @file leitor.h
 * @brief Módulo de Leitura Genérica de Arquivos
 * 
 * Este módulo implementa um leitor genérico de arquivos de texto que armazena
 * todas as linhas do arquivo em uma lista encadeada. Fornece uma camada de
 * abstração para leitura de arquivos .geo e .qry.
 * 
 * @note Cada linha é armazenada como string (char*) na lista.
 * @note O módulo gerencia a memória das linhas lidas automaticamente.
 * @note Suporta extração do nome do arquivo sem caminho.
 * 
 * @author João Costa
 */

#ifndef LEITOR_H
#define LEITOR_H

#include "lista.h"

/**
 * @brief Tipo opaco para dados do arquivo lido.
 */
typedef void *DadosArquivo;

/**
 * @brief Abre e lê um arquivo, armazenando suas linhas.
 * 
 * Abre o arquivo especificado, lê todas as suas linhas e armazena
 * cada uma como string em uma lista encadeada interna.
 * 
 * @param caminho_arquivo Caminho completo do arquivo a ser lido
 * @return Estrutura DadosArquivo contendo as linhas, ou NULL em caso de erro
 */
DadosArquivo dados_arquivo_criar(const char *caminho_arquivo);

/**
 * @brief Libera a memória da estrutura de dados do arquivo.
 * 
 * Destroi a estrutura e libera a memória de todas as linhas
 * lidas do arquivo.
 * 
 * @param dados Estrutura DadosArquivo a ser destruída
 */
void dados_arquivo_destruir(DadosArquivo dados);

/**
 * @brief Obtém apenas o nome do arquivo sem o caminho.
 * 
 * @param dados Estrutura DadosArquivo do arquivo
 * @return Ponteiro para string com o nome do arquivo (sem diretório)
 */
const char *obter_nome_arquivo(DadosArquivo dados);

/**
 * @brief Obtém a lista de linhas do arquivo.
 * 
 * @param dados Estrutura DadosArquivo do arquivo
 * @return Lista contendo as linhas (strings char*) do arquivo
 */
Lista leitor_obter_linhas(DadosArquivo dados);

#endif