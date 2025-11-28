#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitor.h"
#include "lista.h"

/* Função auxiliar para duplicar string */
static char* duplicar_string(const char* s) {
    if (!s) return NULL;
    char* d = malloc(strlen(s) + 1);
    if (!d) return NULL;
    strcpy(d, s);
    return d;
}

typedef struct {
    char* caminho_completo;
    char* nome_arquivo;
    Lista lista_linhas;
} LeitorInfo;

DadosArquivo dados_arquivo_criar(const char *caminho_arquivo) {
    if (!caminho_arquivo) return NULL;

    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    LeitorInfo* info = (LeitorInfo*)malloc(sizeof(LeitorInfo));
    if (!info) {
        fclose(arquivo);
        return NULL;
    }

    // Copia o caminho
    info->caminho_completo = duplicar_string(caminho_arquivo);
    
    // Cria a lista genérica
    info->lista_linhas = createList();

    // Extrai apenas o nome do arquivo
    const char *barra = strrchr(caminho_arquivo, '/');
    if (barra) {
        info->nome_arquivo = duplicar_string(barra + 1);
    } else {
        info->nome_arquivo = duplicar_string(caminho_arquivo);
    }

    // Leitura linha a linha usando getline (POSIX)
    char *linha = NULL;
    size_t len = 0;
    ssize_t lidos;

    while ((lidos = getline(&linha, &len, arquivo)) != -1) {
        // Remove o \n do final, se existir
        linha[strcspn(linha, "\r\n")] = 0;
        
        // Insere uma cópia da linha na lista
        insert(info->lista_linhas, duplicar_string(linha));
    }

    free(linha); // Libera o buffer interno do getline
    fclose(arquivo);

    return (DadosArquivo)info;
}

void dados_arquivo_destruir(DadosArquivo dados) {
    if (!dados) return;

    LeitorInfo* info = (LeitorInfo*)dados;

    if (info->caminho_completo) free(info->caminho_completo);
    if (info->nome_arquivo) free(info->nome_arquivo);

    if (info->lista_linhas) {
        killList(info->lista_linhas, free);
    }

    free(info);
}

const char *obter_nome_arquivo(DadosArquivo dados) {
    if (!dados) return NULL;
    LeitorInfo* info = (LeitorInfo*)dados;
    return info->nome_arquivo;
}

Lista leitor_obter_linhas(DadosArquivo dados) {
    if (!dados) return NULL;
    LeitorInfo* info = (LeitorInfo*)dados;
    return info->lista_linhas;
}