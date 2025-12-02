#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "leitor_qry.h"
#include "forma.h"
#include "lista.h"
#include "segmento.h"
#include "transformacao.h"
#include "limites.h"
#include "svg.h"
#include "visibilidade.h"
#include "geometria.h"

// Includes específicos para transformação
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

// Protótipos das funções auxiliares
static void tratar_a(char *params, Lista lista_formas, Lista lista_anteparos, FILE *txt, int *id_seg);
static void tratar_d(char *params, Lista lista_formas, Lista lista_anteparos, FILE *txt, const char *output_dir, Lista registros_visuais, Lista pontos_bombas);
static void tratar_p(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, Lista registros_visuais, Lista pontos_bombas  );
static void tratar_cln(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, int *id_global, Lista registros_visuais, Lista pontos_bombas);
static void remover_segmento_da_lista(Lista l, Segmento s);
static void processar_destruicao(Lista formas, Lista poligono_visivel, FILE *txt);
static Lista obter_poligono_explosao(double x, double y, Lista formas, Lista anteparos, int *id_counter);
static int forma_foi_atingida(Forma f, Lista poligono);
static void acumular_desenho(Lista acumulador, Lista poligono_atual);
static void extrair_nome_base(const char *path, char *dest);
static void destroy_segmento_void(void *p);

void processar_qry(const char *path_qry, const char *output_dir, const char *nome_base_geo, Lista lista_formas) {
    FILE *qry = fopen(path_qry, "r");
    if (!qry) {
        perror("Erro ao abrir .qry");
        return;
    }

    // 1. Prepara o arquivo de relatório TXT
    char nome_base_qry[256];
    extrair_nome_base(path_qry, nome_base_qry);

    char path_txt[1024];
    sprintf(path_txt, "%s/%s-%s.txt", output_dir, nome_base_geo, nome_base_qry);

    FILE *txt = fopen(path_txt, "w");
    if (!txt) {
        perror("Erro ao criar relatório TXT");
        fclose(qry);
        return;
    }

    // Lista de anteparos (segmentos) que serão usados na visibilidade
    // Inicialmente vazia, populada pelo comando 'a'
    Lista lista_anteparos = createList();
    Lista registros_visuais = createList();
    Lista pontos_bombas = createList();
    int contador_id_segmento = 1; // IDs únicos para novos segmentos

    char linha[1024];
    while (fgets(linha, sizeof(linha), qry)) {
        linha[strcspn(linha, "\r\n")] = 0; // Remove newline
        if (strlen(linha) == 0) continue;

        char *comando = strtok(linha, " \t");
        char *params = strtok(NULL, ""); // Resto da linha

        if (strcmp(comando, "a") == 0) {
            tratar_a(params, lista_formas, lista_anteparos, txt, &contador_id_segmento);
        } 
        else if (strcmp(comando, "d") == 0) {
            tratar_d(params, lista_formas, lista_anteparos, txt, output_dir, registros_visuais, pontos_bombas);
        }
        else if (strcmp(comando, "p") == 0) {
            tratar_p(params, lista_formas, lista_anteparos, txt, output_dir, registros_visuais, pontos_bombas);
        }
        else if (strcmp(comando, "cln") == 0) {
            tratar_cln(params, lista_formas, lista_anteparos, txt, output_dir, &contador_id_segmento, registros_visuais, pontos_bombas);
        }
    }

    char nome_final_svg[1024];
    char nome_qry_sem_ext[256];
    extrair_nome_base(path_qry, nome_qry_sem_ext);
    
    // Nome: nomegeo-nomeqry.svg
    sprintf(nome_final_svg, "%s/%s-%s.svg", output_dir, nome_base_geo, nome_qry_sem_ext);
    
    printf("--> Gerando SVG Final: %s\n", nome_final_svg);
    
    // Aqui usamos 'registros_visuais' como o argumento de polígono para desenhar tudo
    // Passamos NULL no anteparo pois no final talvez não queiramos ver as paredes pretas, 
    // ou passamos lista_anteparos se quiser ver o que sobrou de parede.
    gerar_svg(lista_formas, lista_anteparos, registros_visuais, pontos_bombas, nome_final_svg);

    // --- LIMPEZA ---
    fclose(qry);
    fclose(txt);
    
    // Limpa a memória acumulada
    killList(registros_visuais, destroy_segmento_void);
    
    // Os anteparos são temporários para o QRY ou devem persistir?
    // Geralmente limpamos aqui se não forem usados depois.
    // Use um callback que chame destroy_segmento
    // killList(lista_anteparos, destroy_segmento_wrapper); 
}

/* =========================================================
   IMPLEMENTAÇÃO DO COMANDO 'a'
   ========================================================= */

static void tratar_a(char *params, Lista lista_formas, Lista lista_anteparos, FILE *txt, int *id_seg) {
    int i, j;
    char flag = 'h';
    char flag_buf[10];

    // Tenta ler i, j e opcionalmente a flag
    int lidos = sscanf(params, "%d %d %s", &i, &j, flag_buf);
    if (lidos == 3) flag = flag_buf[0];

    fprintf(txt, "\n[*] Comando 'a': Transformando IDs [%d, %d] em anteparos (%c)\n", i, j, flag);

    // Percorrer a lista de formas
    // CUIDADO: Vamos remover elementos enquanto percorremos.
    // O jeito seguro é sempre pegar o 'next' antes de remover o atual.
    
    Posic p = getFirst(lista_formas);
    while (p) {
        Forma f = get(lista_formas, p);
        Posic p_next = getNext(lista_formas, p); // Salva o próximo
        
        int id_atual = get_forma_id_generico(f);

        if (id_atual >= i && id_atual <= j) {
            // 1. Gera lista de segmentos
            Lista novos_segs = transformar_em_anteparos(f, flag, id_seg);
            
            fprintf(txt, "  - Forma ID %d removida e transformada em %d segmento(s).\n", 
                    id_atual, length(novos_segs));

            // 2. Move segmentos para a lista de anteparos
            while (length(novos_segs) > 0) {
                Posic p_seg = getFirst(novos_segs);
                Segmento seg = removePosic(novos_segs, p_seg);
                
                fprintf(txt, "    > Seg ID:%d (%.1f,%.1f)-(%.1f,%.1f)\n", 
                        get_segmento_id(seg), 
                        get_segmento_x1(seg), get_segmento_y1(seg),
                        get_segmento_x2(seg), get_segmento_y2(seg));
                
                insert(lista_anteparos, seg);
            }
            // Limpa a lista temporária (apenas estrutura)
            killList(novos_segs, NULL); 

            removePosic(lista_formas, p);
            destroy_forma(f);
        }

        p = p_next; // Avança
    }
}

/* =========================================================
   STUB DO COMANDO 'd'
   ========================================================= */
static void tratar_d(char *params, Lista lista_formas, Lista lista_anteparos, FILE *txt, const char *output_dir, Lista registros_visuais, Lista pontos_bombas) {
    double x, y;
    char sfx[100];
    
    // ID temporário para as paredes (alto para não colidir com IDs do geo)
    int id_temp = 90000; 

    sscanf(params, "%lf %lf %s", &x, &y, sfx);
    
    // Move a bomba um milionésimo para evitar alinhamentos perfeitos com vértices
    double x_calc = x + 0.00001;
    double y_calc = y + 0.00001;

    fprintf(txt, "\n[*] Comando 'd': Bomba em (%.2f, %.2f) Sfx: %s\n", x, y, sfx);

    Limites box = calcular_limites_mundo(lista_formas);
    
    double min_x = get_limites_min_x(box) - 50.0;
    double max_x = get_limites_max_x(box) + 50.0;
    double min_y = get_limites_min_y(box) - 50.0;
    double max_y = get_limites_max_y(box) + 50.0;
    
    destruir_limites(box); 

    // 2. Insere as 4 Paredes na lista de anteparos
    Segmento p1 = create_segmento(id_temp++, min_x, min_y, max_x, min_y);
    Segmento p2 = create_segmento(id_temp++, max_x, min_y, max_x, max_y);
    Segmento p3 = create_segmento(id_temp++, max_x, max_y, min_x, max_y);
    Segmento p4 = create_segmento(id_temp++, min_x, max_y, min_x, min_y);

    insert(lista_anteparos, p1);
    insert(lista_anteparos, p2);
    insert(lista_anteparos, p3);
    insert(lista_anteparos, p4);

    fprintf(txt, "  (Paredes do mundo inseridas: %.1f, %.1f a %.1f, %.1f)\n", min_x, min_y, max_x, max_y);

    double *pt = malloc(2 * sizeof(double));
    pt[0] = x;
    pt[1] = y;
    insert(pontos_bombas, pt);

    // 3. EXECUTA O ALGORITMO DE VISIBILIDADE
    // Usa as coordenadas perturbadas (x_calc, y_calc) para estabilidade numérica
    Lista poligono_luz = calcular_visibilidade(x_calc, y_calc, lista_anteparos);

    acumular_desenho(registros_visuais, poligono_luz);
    
    fprintf(txt, "  (Região de visibilidade calculada: %d arestas)\n", length(poligono_luz));

    // 4. Verifica e Remove formas atingidas (Implemente sua colisão aqui)
    processar_destruicao(lista_formas, poligono_luz, txt);

    // 5. Gera SVG de Debug
    if (strcmp(sfx, "-") != 0) {
        Lista temp_bomba = createList();
        insert(temp_bomba, pt);
        char nome_arq_debug[1024];
        
        // Salva no diretório correto recebido da main
        if (output_dir)
            sprintf(nome_arq_debug, "%s/debug_bomba_%s.svg", output_dir, sfx);
        else
            sprintf(nome_arq_debug, "debug_bomba_%s.svg", sfx);
        
        gerar_svg(lista_formas, lista_anteparos, poligono_luz, temp_bomba, nome_arq_debug);

        killList(temp_bomba, NULL);
    }

    // 6. LIMPEZA
    // Remove as paredes da lista para não poluir a próxima bomba
    remover_segmento_da_lista(lista_anteparos, p1);
    remover_segmento_da_lista(lista_anteparos, p2);
    remover_segmento_da_lista(lista_anteparos, p3);
    remover_segmento_da_lista(lista_anteparos, p4);
    
    // Libera a memória das paredes
    destroy_segmento(p1); destroy_segmento(p2);
    destroy_segmento(p3); destroy_segmento(p4);

    // Libera a memória do polígono de visibilidade
    killList(poligono_luz, destroy_segmento_void);
}

/*=================================================
    IMPLEMENTAÇÃO DO COMANDO 'cln'
===================================================*/

static void tratar_cln(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, int *id_global, Lista registros_visuais, Lista pontos_bombas) {
    double x, y, dx, dy;
    char sfx[100];
    
    sscanf(params, "%lf %lf %lf %lf %s", &x, &y, &dx, &dy, sfx);
    fprintf(txt, "\n[*] Comando 'cln': Clonar em (%.2f, %.2f) Off(%.2f, %.2f)\n", x, y, dx, dy);

    double x_calc = x + 0.00001;
    double y_calc = y + 0.00001;

    double *pt = malloc(2 * sizeof(double));
    pt[0] = x;
    pt[1] = y;
    insert(pontos_bombas, pt);

    // 1. Calcula a área atingida
    Lista poligono = obter_poligono_explosao(x_calc, y_calc, formas, anteparos, id_global);

    acumular_desenho(registros_visuais, poligono);

    // 2. Lista temporária para guardar os clones (CRUCIAL!)
    // Se inserirmos direto na lista 'formas' enquanto percorremos ela,
    // podemos clonar o clone infinitamente.
    Lista novos_clones = createList();

    Posic p = getFirst(formas);
    while (p) {
        Forma f = get(formas, p);
        if (forma_foi_atingida(f, poligono)) {
            // Cria o clone com novo ID e posição deslocada
            Forma clone = forma_clonar(f, (*id_global)++, dx, dy);
            if (clone) {
                insert(novos_clones, clone);
                fprintf(txt, "  -> Clonado ID %d (Novo ID %d)\n", 
                        get_forma_id_generico(f), get_forma_id_generico(clone));
            }
        }
        p = getNext(formas, p);
    }

    // 3. Move os clones para a lista principal
    while(length(novos_clones) > 0) {
        Posic p_tmp = getFirst(novos_clones);
        Forma c = removePosic(novos_clones, p_tmp); // Remove da temp
        insert(formas, c); // Insere na principal
    }
    killList(novos_clones, NULL); // Destrói apenas a estrutura da lista temp

    // 4. SVG de Debug
    if (strcmp(sfx, "-") != 0) {
        Lista temp_bomba = createList();
        insert(temp_bomba, pt);
        char path[1024];
        if (out_dir) sprintf(path, "%s/debug_clone_%s.svg", out_dir, sfx);
        else sprintf(path, "debug_clone_%s.svg", sfx);
        
        gerar_svg(formas, anteparos, poligono, temp_bomba, path);
    }
    
    killList(poligono, destroy_segmento_void);
}

/*=================================================
    IMPLEMENTAÇÃO DO COMANDO 'p'
===================================================*/
static void tratar_p(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, Lista registros_visuais, Lista pontos_bombas) {
    double x, y;
    char cor[100], sfx[100];
    int id_dummy = 80000;

    sscanf(params, "%lf %lf %s %s", &x, &y, cor, sfx);
    fprintf(txt, "\n[*] Comando 'p': Pintar em (%.2f, %.2f) cor: %s\n", x, y, cor);

    double x_calc = x + 0.00001;
    double y_calc = y + 0.00001;

    double *pt = malloc(2 * sizeof(double));
    pt[0] = x;
    pt[1] = y;
    insert(pontos_bombas, pt);

    // 1. Calcula a área atingida
    Lista poligono = obter_poligono_explosao(x_calc, y_calc, formas, anteparos, &id_dummy);

    acumular_desenho(registros_visuais, poligono);

    // 2. Aplica a tinta
    Posic p = getFirst(formas);
    while (p) {
        Forma f = get(formas, p);
        if (forma_foi_atingida(f, poligono)) {
            forma_atualizar_cor(f, cor); // Função do forma.c
            fprintf(txt, "  -> Pintado ID %d\n", get_forma_id_generico(f));
        }
        p = getNext(formas, p);
    }

    // 3. SVG de Debug
    if (strcmp(sfx, "-") != 0) {
        Lista temp_bomba = createList();
        insert(temp_bomba, pt);
        char path[1024];
        if (out_dir) sprintf(path, "%s/debug_pintura_%s.svg", out_dir, sfx);
        else sprintf(path, "debug_pintura_%s.svg", sfx);
        
        gerar_svg(formas, anteparos, poligono, temp_bomba, path);
    }
    
    // Limpeza do polígono
    killList(poligono, destroy_segmento_void); 
}



/*=================================================
    FUNÇÕES AUXILIARES
===================================================*/

static void extrair_nome_base(const char *path, char *dest) {
    const char *slash = strrchr(path, '/');
    const char *base = slash ? slash + 1 : path;
    strcpy(dest, base);
    char *dot = strrchr(dest, '.');
    if (dot) *dot = '\0';
}

static void remover_segmento_da_lista(Lista l, Segmento s) {
    Posic p = getFirst(l);
    while (p) {
        if (get(l, p) == s) {
            removePosic(l, p);
            return;
        }
        p = getNext(l, p);
    }
}

static int forma_foi_atingida(Forma f, Lista poligono) {
    void *dados = get_dados_forma(f);
    ShapeType tipo = get_tipo_forma(f);

    switch (tipo) {
        case CIRCULO: {
            double cx = circulo_get_x(dados);
            double cy = circulo_get_y(dados);
            double r = circulo_get_raio(dados);
            
            // Testa o centro e 4 extremos para garantir colisão parcial
            if (ponto_dentro_poligono(cx, cy, poligono)) return 1;
            if (ponto_dentro_poligono(cx + r, cy, poligono)) return 1;
            if (ponto_dentro_poligono(cx - r, cy, poligono)) return 1;
            if (ponto_dentro_poligono(cx, cy + r, poligono)) return 1;
            if (ponto_dentro_poligono(cx, cy - r, poligono)) return 1;
            break;
        }
        case RETANGULO: {
            double x = retangulo_get_x(dados);
            double y = retangulo_get_y(dados);
            double w = retangulo_get_larg(dados);
            double h = retangulo_get_altura(dados);
            
            // Testa centro e cantos
            if (ponto_dentro_poligono(x, y, poligono)) return 1;
            if (ponto_dentro_poligono(x + w, y, poligono)) return 1;
            if (ponto_dentro_poligono(x, y + h, poligono)) return 1;
            if (ponto_dentro_poligono(x + w, y + h, poligono)) return 1;
            if (ponto_dentro_poligono(x + w/2.0, y + h/2.0, poligono)) return 1;
            break;
        }
        case LINHA: {
            // Testa o ponto médio
            double mx = (linha_get_x1(dados) + linha_get_x2(dados)) / 2.0;
            double my = (linha_get_y1(dados) + linha_get_y2(dados)) / 2.0;
            if (ponto_dentro_poligono(mx, my, poligono)) return 1;
            break;
        }
        case TEXTO: {
            // Testa a âncora
            double tx = texto_get_x(dados);
            double ty = texto_get_y(dados);
            if (ponto_dentro_poligono(tx, ty, poligono)) return 1;
            break;
        }
        default:
            return 0;
    }
    return 0;
}

static Lista obter_poligono_explosao(double x, double y, Lista formas, Lista anteparos, int *id_counter) {
    // 1. Calcula Limites
    Limites box = calcular_limites_mundo(formas);
    double min_x = get_limites_min_x(box) - 50.0;
    double max_x = get_limites_max_x(box) + 50.0;
    double min_y = get_limites_min_y(box) - 50.0;
    double max_y = get_limites_max_y(box) + 50.0;
    destruir_limites(box);

    // 2. Cria Paredes
    Segmento p1 = create_segmento((*id_counter)++, min_x, min_y, max_x, min_y);
    Segmento p2 = create_segmento((*id_counter)++, max_x, min_y, max_x, max_y);
    Segmento p3 = create_segmento((*id_counter)++, max_x, max_y, min_x, max_y);
    Segmento p4 = create_segmento((*id_counter)++, min_x, max_y, min_x, min_y);

    insert(anteparos, p1); insert(anteparos, p2);
    insert(anteparos, p3); insert(anteparos, p4);

    // 3. Roda Algoritmo (com perturbação)
    Lista poligono = calcular_visibilidade(x + 0.00001, y + 0.00001, anteparos);

    // 4. Remove Paredes
    // Nota: Dependendo da sua lista, remover pelo ponteiro pode ser lento ou exigir função específica.
    // Se sua 'remove_segmento_da_lista' já existe (usada no tratar_d), use-a.
    // Caso contrário, implemente um loop simples de remoção.
    
    // Assumindo que você tem a função helper que usamos no tratar_d:
    remover_segmento_da_lista(anteparos, p1);
    remover_segmento_da_lista(anteparos, p2);
    remover_segmento_da_lista(anteparos, p3);
    remover_segmento_da_lista(anteparos, p4);

    destroy_segmento(p1); destroy_segmento(p2);
    destroy_segmento(p3); destroy_segmento(p4);

    return poligono;
}

static void processar_destruicao(Lista formas, Lista poligono_visivel, FILE *txt) {
    Posic p = getFirst(formas);
    while (p) {
        Forma f = get(formas, p);
        Posic p_next = getNext(formas, p); // Salva o próximo

        // Chama a função corrigida acima
        if (forma_foi_atingida(f, poligono_visivel)) {
            int id = get_forma_id_generico(f);
            
            fprintf(txt, "  -> Destruido: ID %d\n", id);
            
            removePosic(formas, p);
            destroy_forma(f);
        }
        p = p_next;
    }
}


static void acumular_desenho(Lista acumulador, Lista poligono_atual) {
    if (!poligono_atual) return;
    
    Posic p = getFirst(poligono_atual);
    while(p) {
        Segmento s = get(poligono_atual, p);

        Segmento copia = create_segmento(0, 
                                         get_segmento_x1(s), get_segmento_y1(s),
                                         get_segmento_x2(s), get_segmento_y2(s));
        insert(acumulador, copia);
        p = getNext(poligono_atual, p);
    }
}

static void destroy_segmento_void(void *p) {
    if (p) destroy_segmento((Segmento)p);
}