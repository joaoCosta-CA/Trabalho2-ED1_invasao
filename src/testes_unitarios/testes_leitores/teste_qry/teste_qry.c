#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lista.h"
#include "leitor_qry.h"
#include "forma.h"
#include "circulo.h"
#include "retangulo.h"

#define GEO_NAME "cidade_teste"
#define QRY_FILE "comandos.qry"
#define OUTPUT_DIR "."

void setup_cenario(Lista l) {
    // Adiciona formas para serem alvo dos comandos
    // ID 1: Círculo (Será transformado em anteparo pelo comando 'a')
    void *c = CriarCirc(1, 100, 100, 50, "red", "red");
    insert(l, create_forma(CIRCULO, c));

    // ID 2: Retângulo (Será alvo da bomba 'd')
    void *r = criarRec(2, 200, 200, 50, 50, "blue", "blue");
    insert(l, create_forma(RETANGULO, r));
}

void criar_arquivo_qry() {
    FILE *f = fopen(QRY_FILE, "w");
    // Comando 'a': Transforma ID 1 em anteparo
    fprintf(f, "a 1 1 v\n"); 
    // Comando 'd': Bomba perto do ID 2
    fprintf(f, "d 200 200 bomb1\n");
    fclose(f);
}

int arquivo_existe_e_contem(const char *path, const char *texto) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    char buffer[1024];
    int achou = 0;
    while (fgets(buffer, 1024, f)) {
        if (strstr(buffer, texto)) {
            achou = 1; 
            break;
        }
    }
    fclose(f);
    return achou;
}

// Callback de limpeza
void destroy_forma_wrapper(void* f) { destroy_forma((Forma)f); }

int main() {
    printf("[QRY] 1. Preparando cenário (Formas + Arquivo QRY)...\n");
    Lista formas = createList();
    setup_cenario(formas);
    criar_arquivo_qry();

    printf("[QRY] 2. Processando QRY...\n");
    processar_qry(QRY_FILE, OUTPUT_DIR, GEO_NAME, formas, 'q', 10);

    printf("[QRY] 3. Verificando relatórios...\n");
    
    // O nome do relatório gerado deve ser "cidade_teste-comandos.txt"
    char nome_relatorio[256];
    sprintf(nome_relatorio, "%s-%s.txt", GEO_NAME, "comandos"); // Remove .qry da string se sua logica faz isso

    // Verifica se o relatório relatou a transformação
    if (arquivo_existe_e_contem(nome_relatorio, "Transformando IDs")) {
        printf("   > Relatório confirmou comando 'a'.\n");
    } else {
        printf("   [ERRO] Relatório não contém log do comando 'a'!\n");
        exit(1);
    }

    // Verifica se relatou a bomba
    if (arquivo_existe_e_contem(nome_relatorio, "Bomba em")) {
        printf("   > Relatório confirmou comando 'd'.\n");
    } else {
        printf("   [ERRO] Relatório não contém log do comando 'd'!\n");
        exit(1);
    }

    printf("[QRY] 4. Limpeza...\n");
    killList(formas, destroy_forma_wrapper);
    remove(QRY_FILE);
    remove(nome_relatorio);
    // remove("debug_bomba_bomb1.svg"); // Opcional

    printf(">>> SUCESSO: Leitor QRY integrou e rodou corretamente!\n");
    return 0;
}