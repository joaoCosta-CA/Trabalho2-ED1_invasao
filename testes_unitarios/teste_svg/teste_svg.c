#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Includes do projeto
#include "lista.h"
#include "svg.h"
#include "forma.h"

// Includes das formas para poder criá-las
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

#define ARQUIVO_SAIDA "teste_output.svg"

/* Helper: Verifica se uma string existe dentro de um arquivo */
int arquivo_contem_texto(const char *nome_arquivo, const char *texto_procurado) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return 0; // Arquivo não existe

    char linha[1024];
    int encontrou = 0;

    while (fgets(linha, sizeof(linha), f)) {
        if (strstr(linha, texto_procurado) != NULL) {
            encontrou = 1;
            break;
        }
    }

    fclose(f);
    return encontrou;
}

/* Helper para limpar memória (callback da lista) */
void destruir_forma_wrapper(void *item) {
    if (item) destroy_forma((Forma)item);
}

int main() {
    printf("[SVG] 1. Preparando dados para o teste...\n");
    
    Lista l = createList();
    
    // --- Criação de Formas Dummy ---
    // Ajuste os nomes das funções (createCirculo/CriarCirc) conforme seu .h exato
    
    // 1. Círculo (Vermelho/Azul)
    void *c = CriarCirc(1, 100.0, 100.0, 50.0, "red", "blue");
    insert(l, create_forma(CIRCULO, c));

    // 2. Retângulo (Verde/Amarelo)
    void *r = criarRec(2, 200.0, 200.0, 80.0, 40.0, "green", "yellow");
    insert(l, create_forma(RETANGULO, r));

    // 3. Linha (Preta)
    void *li = criarLinha(3, 10.0, 10.0, 50.0, 50.0, "black");
    insert(l, create_forma(LINHA, li));

    // 4. Texto
    void *t = criar_texto(4, 300.0, 50.0, "black", "white", 'i', "TesteSVG");
    insert(l, create_forma(TEXTO, t));

    printf("[SVG] 2. Gerando arquivo '%s'...\n", ARQUIVO_SAIDA);
    gerar_svg(l, ARQUIVO_SAIDA);

    printf("[SVG] 3. Verificando conteúdo do arquivo...\n");

    // Verifica se o cabeçalho foi escrito
    assert(arquivo_contem_texto(ARQUIVO_SAIDA, "<svg xmlns="));
    
    // Verifica se o Círculo está lá com a cor certa
    assert(arquivo_contem_texto(ARQUIVO_SAIDA, "<circle"));
    assert(arquivo_contem_texto(ARQUIVO_SAIDA, "fill=\"blue\""));
    
    // Verifica se o Retângulo está lá
    assert(arquivo_contem_texto(ARQUIVO_SAIDA, "<rect"));
    assert(arquivo_contem_texto(ARQUIVO_SAIDA, "width=\"80.00\""));
    
    // Verifica Linha e Texto
    assert(arquivo_contem_texto(ARQUIVO_SAIDA, "<line"));
    assert(arquivo_contem_texto(ARQUIVO_SAIDA, "<text"));
    assert(arquivo_contem_texto(ARQUIVO_SAIDA, ">TesteSVG</text>"));

    printf("[SVG] 4. Limpeza...\n");
    killList(l, destruir_forma_wrapper);
    
    // Opcional: remover o arquivo gerado (comente se quiser ver o SVG visualmente)
    // remove(ARQUIVO_SAIDA);

    printf("\n>>> SUCESSO: O Módulo SVG Writer funciona corretamente! <<<\n");
    return 0;
}