#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 

#include "argumentos.h" 
#include "leitor.h"
#include "leitor_geo.h"
#include "leitor_qry.h"
#include "lista.h"
#include "forma.h"
#include "svg.h"

#define MAX_FULL_PATH 1024

void destruir_forma_void(void *item) {
    if (item) {
        destroy_forma((Forma)item);
    }
}

int main(int argc, char *argv[]) {

    if (argc < 5) {
        printf("Uso: %s [-e <dir_base>] -f <arq.geo> -o <dir_saida> [-q <arq.qry>]\n", argv[0]);
        return 1;
    }
    
    const char *base_input_path = get_option_value(argc, argv, "e"); 
    const char *geo_filename = get_option_value(argc, argv, "f");
    const char *output_path = get_option_value(argc, argv, "o");
    const char *qry_filename = get_option_value(argc, argv, "q");
    const char *arg_to = get_option_value(argc, argv, "to");
    const char *arg_i = get_option_value(argc, argv, "i");

    char tipo_ordenacao = 'q'; 
    int insertion_cutoff = 10;

    if (geo_filename == NULL || output_path == NULL) {
        printf("Erro: Os parâmetros -f e -o são obrigatórios.\n");
        exit(EXIT_FAILURE);
    }

    if (!diretorio_valido(output_path)) {
        printf("Erro: O diretório de saída '%s' não existe ou não é válido.\n", output_path);
        exit(EXIT_FAILURE);
    }

    if (arg_to != NULL) {
        if (strcmp(arg_to, "m") == 0 || strcmp(arg_to, "q") == 0) {
            tipo_ordenacao = arg_to[0];
        } else {
            fprintf(stderr, "Aviso: Tipo de ordenação '%s' desconhecido. Usando padrão 'q'.\n", arg_to);
        }
    }

    if (arg_i != NULL) {
        int val = atoi(arg_i);
        if (val > 0) insertion_cutoff = val;
    }

    printf("Config: Geo='%s', Sort='%c', Cutoff (-i)=%d\n", geo_filename, tipo_ordenacao, insertion_cutoff);

    Lista lista_formas = createList();
    int max_id = 0;


    ler_arquivo_geo(base_input_path, geo_filename, lista_formas, &max_id);

    char svg_output_path[MAX_FULL_PATH];
    char nome_base[256];
    strncpy(nome_base, geo_filename, sizeof(nome_base)-1);
    char *ponto = strrchr(nome_base, '.');
    if (ponto) *ponto = '\0'; 

    snprintf(svg_output_path, MAX_FULL_PATH, "%s/%s.svg", output_path, nome_base);


    gerar_svg(lista_formas, NULL, NULL, NULL, svg_output_path);

    if (qry_filename != NULL) {

        
        char qry_full_path[MAX_FULL_PATH];
        if (base_input_path) {
            snprintf(qry_full_path, MAX_FULL_PATH, "%s/%s", base_input_path, qry_filename);
        } else {
            strncpy(qry_full_path, qry_filename, MAX_FULL_PATH - 1);
        }

        processar_qry(qry_full_path, output_path, nome_base, lista_formas, tipo_ordenacao, insertion_cutoff);
    }

    killList(lista_formas, destruir_forma_void);

    printf("Processamento concluido\n");

    return 0;
}