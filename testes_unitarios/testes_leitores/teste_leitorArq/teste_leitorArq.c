#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "lista.h"
#include "leitor.h" // Seu módulo genérico (dados_arquivo_criar, etc)

#define ARQ_TESTE "temp_leitor.txt"

void criar_arquivo_generico() {
    FILE *f = fopen(ARQ_TESTE, "w");
    if (!f) exit(1);
    fprintf(f, "Linha Um\n");
    fprintf(f, "Linha Dois\n");
    fprintf(f, "   Linha Tres com Espacos   \n");
    fclose(f);
}

int main() {
    printf("[LEITOR] 1. Criando arquivo temporário...\n");
    criar_arquivo_generico();

    printf("[LEITOR] 2. Lendo arquivo...\n");
    DadosArquivo dados = dados_arquivo_criar(ARQ_TESTE);
    
    // Verifica se abriu
    assert(dados != NULL);
    
    // Verifica nome do arquivo
    const char* nome = obter_nome_arquivo(dados);
    printf("   > Nome detectado: %s\n", nome);
    assert(strcmp(nome, ARQ_TESTE) == 0);

    // Verifica conteúdo das linhas
    Lista linhas = leitor_obter_linhas(dados); // Ou obter_fila_linhas, dependendo da sua versao
    assert(length(linhas) == 3);

    Posic p = getFirst(linhas);
    char *l1 = (char*)get(linhas, p);
    assert(strcmp(l1, "Linha Um") == 0);

    p = getNext(linhas, p);
    char *l2 = (char*)get(linhas, p);
    assert(strcmp(l2, "Linha Dois") == 0);

    printf("[LEITOR] 3. Limpeza...\n");
    dados_arquivo_destruir(dados);
    remove(ARQ_TESTE);

    printf(">>> SUCESSO: Leitor Genérico OK!\n");
    return 0;
}