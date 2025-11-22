#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/argumentos/argumentos.h"
#include "lib/leitores/leitor_arq/leitor.h"
#include "lib/leitores/arquivo_geo/leitor_geo.h"
#include "lib/estruturas_dados/lista/lista.h"
#include "lib/formas/enumFormas/formas.h"

#define MAX_FULL_PATH 1024

void destruir_forma_void(void *item) {
    if (item) {
        destroy_forma((Forma)item); // Chama a função definida em forma.h
    }
}

int main(int argc, char *argv[]) {

    /* 1. Validação de Argumentos Básicos */
    if (argc < 5) {
        printf("Uso: %s [-e <dir_base>] -f <arq.geo> -o <dir_saida>\n", argv[0]);
        return 1;
    }
    
    const char *base_input_path = get_option_value(argc, argv, "-e"); 
    const char *geo_filename = get_option_value(argc, argv, "-f");
    const char *output_path = get_option_value(argc, argv, "-o");
    

    /* 3. Verificações de Segurança */
    if (geo_filename == NULL || output_path == NULL) {
        printf("Erro: Os parâmetros -f e -o são obrigatórios.\n");
        exit(EXIT_FAILURE);
    }

    if (!diretorio_valido(output_path)) {
        printf("Erro: O diretório de saída '%s' não existe ou não é válido.\n", output_path);
        exit(EXIT_FAILURE);
    }

    /* 4. Inicialização das Estruturas */
    Lista lista_formas = createList();
    int max_id = 0;

    /* 5. Processamento do Arquivo GEO */
    printf("Lendo arquivo GEO: %s ...\n", geo_filename);
    
    // A função ler_arquivo_geo (refatorada no passo anterior) popula a lista
    ler_arquivo_geo(base_input_path, geo_filename, lista_formas, &max_id);

    /* 6. Geração da Saída (SVG) */
    // Monta o nome do arquivo de saída
     char svg_output_path[MAX_FULL_PATH];
    
    // Extrai apenas o nome do arquivo sem extensão para compor a saída
    char nome_base[256];
    strncpy(nome_base, geo_filename, sizeof(nome_base)-1);
    char *ponto = strrchr(nome_base, '.');
    if (ponto) *ponto = '\0'; // Remove .geo

    snprintf(svg_output_path, MAX_FULL_PATH, "%s/%s.svg", output_path, nome_base);

    printf("Gerando SVG em: %s ...\n", svg_output_path);
    
    // ATENÇÃO: Você precisa implementar essa função em um módulo separado (svg_writer.c)
    // gerar_svg_do_geo(lista_formas, svg_output_path); 

    /* 7. Limpeza de Memória */
    // Libera a lista e todas as formas dentro dela usando o callback
    killList(lista_formas, destruir_forma_void);

    printf("Concluído. Max ID encontrado: %d\n", max_id);

    return 0;
}