/* testes_leitores/teste_geo.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "lista.h"
#include "leitor_geo.h"
#include "forma.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

#define ARQUIVO_TESTE "temp_teste.geo"

/* Função auxiliar para criar o arquivo .geo dinamicamente */
void criar_cenario_teste() {
    FILE *f = fopen(ARQUIVO_TESTE, "w");
    if (!f) {
        perror("Nao foi possivel criar arquivo de teste");
        exit(1);
    }
    
    // Formato: c id x y r corb corp
    fprintf(f, "c 1 10.5 20.5 5.0 red blue\n");
    
    // Formato: r id x y w h corb corp
    fprintf(f, "r 2 100.0 200.0 50.0 30.0 green yellow\n");
    
    // Formato: l id x1 y1 x2 y2 cor
    fprintf(f, "l 3 10.0 10.0 50.0 50.0 black\n");
    
    // Formato: t id x y corb corp ancora conteudo
    fprintf(f, "t 4 500.0 500.0 white black i Ola Mundo\n");
    
    fclose(f);
}

/* Callback para limpar memória */
void destruir_forma_wrapper(void *item) {
    if (item) destroy_forma((Forma)item);
}

int main() {
    printf("[GEO] 1. Criando ambiente de teste (arquivo '%s')...\n", ARQUIVO_TESTE);
    criar_cenario_teste();

    printf("[GEO] 2. Executando Leitor GEO...\n");
    Lista l = createList();
    int max_id = 0;

    // Passa "." como diretório base e o nome do arquivo
    ler_arquivo_geo(".", ARQUIVO_TESTE, l, &max_id);

    printf("[GEO] 3. Verificando integridade dos dados...\n");

    // 3.1 Verificações Gerais
    assert(l != NULL);
    assert(length(l) == 4); // Deve ter lido exatamente 4 formas
    assert(max_id == 4);    // O maior ID escrito foi 4

    // 3.2 Verificação Detalhada dos Nós
    Posic p = getFirst(l);
    
    // --- Nó 1: Círculo ---
    assert(p != NULL);
    Forma f1 = (Forma)get(l, p);
    assert(get_tipo_forma(f1) == CIRCULO);
    
    void *dados1 = get_dados_forma(f1);

    assert(circulo_get_id(dados1) == 1); 
    assert(circulo_get_x(dados1) == 10.5);
    assert(strcmp(circulo_get_corp(dados1), "blue") == 0);

    // --- Nó 2: Retângulo ---
    p = getNext(l, p);
    assert(p != NULL);
    Forma f2 = (Forma)get(l, p);
    assert(get_tipo_forma(f2) == RETANGULO);
    
    void *dados2 = get_dados_forma(f2);
    assert(retangulo_get_id(dados2) == 2);
    assert(retangulo_get_larg(dados2) == 50.0);

    // --- Nó 3: Linha ---
    p = getNext(l, p);
    assert(p != NULL);
    Forma f3 = (Forma)get(l, p);
    assert(get_tipo_forma(f3) == LINHA);
    
    void *dados3 = get_dados_forma(f3);
    assert(linha_get_id(dados3) == 3);
    assert(linha_get_x2(dados3) == 50.0);

    // --- Nó 4: Texto ---
    p = getNext(l, p);
    assert(p != NULL);
    Forma f4 = (Forma)get(l, p);
    assert(get_tipo_forma(f4) == TEXTO);
    
    void *dados4 = get_dados_forma(f4);
    assert(texto_get_id(dados4) == 4);
    assert(strcmp(texto_get_conteudo(dados4), "Ola Mundo") == 0);
    assert(texto_get_ancora(dados4) == 'i');

    // Verifica se acabou
    assert(getNext(l, p) == NULL);

    printf("[GEO] 4. Limpando sujeira e memória...\n");
    
    // Remove o arquivo temporário do disco
    remove(ARQUIVO_TESTE);

    // Limpa a lista usando a função de destruir forma
    killList(l, destruir_forma_wrapper);

    printf("\n>>> SUCESSO: O Leitor GEO passou em todos os testes! <<<\n");
    
    return 0;
}