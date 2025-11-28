#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "leitor_geo.h"
#include "forma.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
// #include "estilo_texto.h"

/* Funções auxiliares privadas */
static void tratar_circulo(char *params, Lista l, int *max_id);
static void tratar_retangulo(char *params, Lista l, int *max_id);
static void tratar_linha(char *params, Lista l, int *max_id);
static void tratar_texto(char *params, Lista l, int *max_id);

/* Função Principal */
void ler_arquivo_geo(const char *diretorio_base, const char *nome_arquivo, Lista formas, int *max_id) {
    char caminho_completo[512];
    
    if (diretorio_base && strlen(diretorio_base) > 0) {
        snprintf(caminho_completo, sizeof(caminho_completo), "%s/%s", diretorio_base, nome_arquivo);
    } else {
        strncpy(caminho_completo, nome_arquivo, sizeof(caminho_completo));
    }

    FILE *arquivo = fopen(caminho_completo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo .geo");
        return;
    }

    char linha[1024];
    while (fgets(linha, sizeof(linha), arquivo)) {
        /* Remove quebra de linha do final se existir */
        linha[strcspn(linha, "\r\n")] = 0;

        /* Separa o comando do restante dos parâmetros */
        char *comando = strtok(linha, " \t");
        char *params = strtok(NULL, ""); // Pega o resto da linha inalterado para processamento interno

        if (!comando) continue;

        if (strcmp(comando, "c") == 0) {
            tratar_circulo(params, formas, max_id);
        } else if (strcmp(comando, "r") == 0) {
            tratar_retangulo(params, formas, max_id);
        } else if (strcmp(comando, "l") == 0) {
            tratar_linha(params, formas, max_id);
        } else if (strcmp(comando, "t") == 0) {
            tratar_texto(params, formas, max_id);
        }
        // else if (strcmp(comando, "ts") == 0) { ... }
    }

    fclose(arquivo);
}

/* * Implementação dos Trata Comandos 
 * Nota: O strtok aqui continua a partir do 'params' passado.
 * Como 'params' é uma string nova (ponteiro), reiniciamos o strtok nela.
 */

static void tratar_circulo(char *params, Lista l, int *max_id) {
    if (!params) return;
    
    // Reinicia tokenizer na string de parametros
    char *token = strtok(params, " \t");
    if (!token) return;
    int id = atoi(token);
    if (id > *max_id) *max_id = id;

    token = strtok(NULL, " \t"); if (!token) return;
    double x = atof(token);
    
    token = strtok(NULL, " \t"); if (!token) return;
    double y = atof(token);
    
    token = strtok(NULL, " \t"); if (!token) return;
    double r = atof(token);
    
    token = strtok(NULL, " \t"); if (!token) return;
    char *corb = token;
    
    token = strtok(NULL, " \t"); if (!token) return;
    char *corp = token;

    // Cria o TAD específico
    void *c = CriarCirc(id, x, y, r, corb, corp);
    
    // Cria o envelope e insere na lista
    // create_forma deve ser implementada em forma.c
    Forma *f = create_forma(CIRCULO, c); 
    insert(l, f);
}

static void tratar_retangulo(char *params, Lista l, int *max_id) {
    if (!params) return;
    char *token = strtok(params, " \t");
    if (!token) return;
    int id = atoi(token);
    if (id > *max_id) *max_id = id;

    token = strtok(NULL, " \t"); double x = atof(token);
    token = strtok(NULL, " \t"); double y = atof(token);
    token = strtok(NULL, " \t"); double w = atof(token);
    token = strtok(NULL, " \t"); double h = atof(token);
    token = strtok(NULL, " \t"); char *corb = token;
    token = strtok(NULL, " \t"); char *corp = token;

    void *rect = criarRec(id, x, y, w, h, corb, corp);
    Forma *f = create_forma(RETANGULO, rect);
    insert(l, f);
}

static void tratar_linha(char *params, Lista l, int *max_id) {
    if (!params) return;
    char *token = strtok(params, " \t");
    int id = atoi(token);
    if (id > *max_id) *max_id = id;

    token = strtok(NULL, " \t"); double x1 = atof(token);
    token = strtok(NULL, " \t"); double y1 = atof(token);
    token = strtok(NULL, " \t"); double x2 = atof(token);
    token = strtok(NULL, " \t"); double y2 = atof(token);
    token = strtok(NULL, " \t"); char *cor = token;

    void *lin = criarLinha(id, x1, y1, x2, y2, cor);
    Forma *f = create_forma(LINHA, lin);
    insert(l, f);
}

static void tratar_texto(char *params, Lista l, int *max_id) {
    if (!params) return;
    char *token = strtok(params, " \t");
    int id = atoi(token);
    if (id > *max_id) *max_id = id;

    token = strtok(NULL, " \t"); double x = atof(token);
    token = strtok(NULL, " \t"); double y = atof(token);
    token = strtok(NULL, " \t"); char *corb = token;
    token = strtok(NULL, " \t"); char *corp = token;
    token = strtok(NULL, " \t"); char ancora = token[0];

    // O conteúdo do texto pega o resto da linha, incluindo espaços
    char *conteudo = strtok(NULL, "\r\n");
    
    // Pula espaços em branco iniciais no conteúdo (lógica do seu código original)
    if (conteudo) {
        while (*conteudo == ' ' || *conteudo == '\t') {
            conteudo++;
        }
    } else {
        conteudo = "";
    }

    void *txt = criar_texto(id, x, y, corb, corp, ancora, conteudo);
    Forma *f = create_forma(TEXTO, txt);
    insert(l, f);
}