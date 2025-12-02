#ifndef LEITOR_QRY_H
#define LEITOR_QRY_H

#include "lista.h"

/*
 * Processa o arquivo de consultas .qry.
 * - path_qry: Caminho completo do arquivo .qry
 * - output_dir: Diretório onde salvar o .txt e .svgs
 * - nome_base_geo: Nome do arquivo geo (sem extensão) para compor o nome do relatório
 * - lista_formas: Lista contendo as formas lidas do .geo (será modificada!)
 */
/* Assinatura Atualizada */
void processar_qry(const char *path_qry, const char *output_dir, const char *nome_base_geo, 
                   Lista lista_formas, char tipo_ord, int cutoff);

#endif