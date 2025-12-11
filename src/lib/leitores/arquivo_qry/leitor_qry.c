#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
static Lista obter_poligono_explosao(double x, double y, Lista formas, Lista anteparos, char tipo_ord, int cutoff);
static void tratar_d(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, const char *nome_base_combo, Lista registros, Lista pontos_bombas, char tipo_ord, int cutoff);
static void tratar_p(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, const char *nome_base_combo, Lista registros, Lista pontos_bombas, char tipo_ord, int cutoff);
static void tratar_cln(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, const char *nome_base_combo, int *id_global, Lista registros, Lista pontos_bombas, char tipo_ord, int cutoff);
static void processar_destruicao(Lista formas, Lista poligono_visivel, FILE *txt);
static int forma_foi_atingida(Forma f, Lista poligono);
static void acumular_desenho(Lista acumulador, Lista poligono_atual);
static void extrair_nome_base(const char *path, char *dest);
static void destroy_segmento_void(void *p);
static void destroy_poligono_lista(void *p);
static int anteparo_foi_atingido(Segmento s, Lista poligono);
static void processar_efeito_em_anteparos(Lista anteparos, Lista poligono, FILE *txt, char tipo_efeito, char* cor_pintura);

void processar_qry(const char *path_qry, const char *output_dir, const char *nome_base_geo, Lista lista_formas, char tipo_ord, int cutoff) {
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

    // Extrai nomes base para geração de SVGs
    char nome_qry_sem_ext[256];
    extrair_nome_base(path_qry, nome_qry_sem_ext);
    char nome_base_combo[512];
    sprintf(nome_base_combo, "%s-%s", nome_base_geo, nome_qry_sem_ext);

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
            tratar_d(params, lista_formas, lista_anteparos, txt, output_dir, nome_base_combo, registros_visuais, pontos_bombas, tipo_ord, cutoff);
        }
        else if (strcmp(comando, "p") == 0) {
            tratar_p(params, lista_formas, lista_anteparos, txt, output_dir, nome_base_combo, registros_visuais, pontos_bombas, tipo_ord, cutoff);
        }
        else if (strcmp(comando, "cln") == 0) {
            tratar_cln(params, lista_formas, lista_anteparos, txt, output_dir, nome_base_combo, &contador_id_segmento, registros_visuais, pontos_bombas, tipo_ord, cutoff);
        }
    }

    char nome_final_svg[1024];
    
    // Nome: nomegeo-nomeqry.svg
    sprintf(nome_final_svg, "%s/%s.svg", output_dir, nome_base_combo);
    


    
    gerar_svg_multi(lista_formas, lista_anteparos, registros_visuais, pontos_bombas, nome_final_svg);

    // --- LIMPEZA ---
    fclose(qry);
    fclose(txt);
    
    killList(registros_visuais, destroy_poligono_lista);
    
    killList(lista_anteparos, destroy_segmento_void);

    killList(pontos_bombas, free);

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
    
    Posic p = getFirst(lista_formas);
    while (p) {
        Forma f = get(lista_formas, p);
        Posic p_next = getNext(lista_formas, p); // Salva o próximo
        
        int id_atual = get_forma_id_generico(f);

        if (id_atual >= i && id_atual <= j) {
            // 1. Gera lista de segmentos
            Lista novos_segs = transformar_em_anteparos(f, flag, id_seg);
            
            fprintf(txt, "  - Forma ID %d (%s) removida e transformada em %d segmento(s).\n", 
                    id_atual, get_tipo_forma_str(f), length(novos_segs));

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
static void tratar_d(char *params, Lista lista_formas, Lista lista_anteparos, FILE *txt, 
                     const char *output_dir, const char *nome_base_combo, Lista registros_visuais, Lista pontos_bombas, 
                     char tipo_ord, int cutoff) {
    double x, y;
    char sfx[100];
    
    sscanf(params, "%lf %lf %s", &x, &y, sfx);
    fprintf(txt, "\n[*] Comando 'd': Bomba em (%.2f, %.2f) Sfx: %s\n", x, y, sfx);

    // 1. Guarda ponto da bomba para o SVG final
    double *pt = malloc(2 * sizeof(double));
    pt[0] = x; pt[1] = y;
    insert(pontos_bombas, pt);

    Lista poligono = obter_poligono_explosao(x, y, lista_formas, lista_anteparos, tipo_ord, cutoff);

    // Acumula para o SVG final
    acumular_desenho(registros_visuais, poligono);
    
    fprintf(txt, "  (Região de visibilidade calculada: %d arestas)\n", length(poligono));

    // 3. APLICAÇÃO DOS EFEITOS
    processar_destruicao(lista_formas, poligono, txt);
    processar_efeito_em_anteparos(lista_anteparos, poligono, txt, 'd', NULL);

    // 4. GERA SVG DE DEBUG (formato: nomeGeo-nomeQry-sufixo.svg)
    if (strcmp(sfx, "-") != 0) {
        Lista temp_bomba = createList();
        insert(temp_bomba, pt);
        char nome_arq[1024];
        
        if (output_dir) sprintf(nome_arq, "%s/%s-%s.svg", output_dir, nome_base_combo, sfx);
        else sprintf(nome_arq, "%s-%s.svg", nome_base_combo, sfx);
        
        gerar_svg(NULL, NULL, poligono, temp_bomba, nome_arq);

        killList(temp_bomba, NULL);
    }

    killList(poligono, destroy_segmento_void);
}

/*=================================================
    IMPLEMENTAÇÃO DO COMANDO 'cln'
===================================================*/

static void tratar_cln(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, const char *nome_base_combo, int *id_global, Lista registros_visuais, Lista pontos_bombas, char tipo_ord, int cutoff) {
    double x, y, dx, dy;
    char sfx[100];
    
    sscanf(params, "%lf %lf %lf %lf %s", &x, &y, &dx, &dy, sfx);
    fprintf(txt, "\n[*] Comando 'cln': Clonar em (%.2f, %.2f) Off(%.2f, %.2f)\n", x, y, dx, dy);
    
    // Save bomb point for SVG
    double *pt = malloc(2 * sizeof(double));
    pt[0] = x;
    pt[1] = y;
    insert(pontos_bombas, pt);
    


    // 1. Calcula a área atingida
    Lista poligono = obter_poligono_explosao(x, y, formas, anteparos, tipo_ord, cutoff);
    


    acumular_desenho(registros_visuais, poligono);

    // 2. Lista temporária para guardar os clones
    Lista novos_clones = createList();

    // 2.1 Clona FORMAS atingidas
    Posic p = getFirst(formas);
    while (p) {
        Forma f = get(formas, p);
        if (forma_foi_atingida(f, poligono)) {
            // Cria o clone com novo ID e posição deslocada
            Forma clone = forma_clonar(f, (*id_global)++, dx, dy);
            if (clone) {
                insert(novos_clones, clone);
                fprintf(txt, "  -> Clonado: ID %d (%s) -> Novo ID %d\n", 
                        get_forma_id_generico(f), get_tipo_forma_str(f), get_forma_id_generico(clone));
            }
        }
        p = getNext(formas, p);
    }

    // 2.2 Clona ANTEPAROS atingidos (usa mesma lógica do comando 'd')
    double clone_params[3];
    clone_params[0] = dx;
    clone_params[1] = dy;
    clone_params[2] = (double)(*id_global);
    
    int id_antes = *id_global;
    processar_efeito_em_anteparos(anteparos, poligono, txt, 'c', (char*)clone_params);
    *id_global = (int)clone_params[2];
    

    
    int total_clones = (*id_global - id_antes);
    if (total_clones > 0) {
        fprintf(txt, "  (Total: %d anteparo(s) clonado(s))\n", total_clones);
    }

    // 3. Move os clones de formas para a lista principal
    while(length(novos_clones) > 0) {
        Posic p_tmp = getFirst(novos_clones);
        Forma c = removePosic(novos_clones, p_tmp);
        insert(formas, c);
    }
    killList(novos_clones, NULL);

    // 4. SVG
    if (strcmp(sfx, "-") != 0) {
        Lista temp_bomba = createList();
        insert(temp_bomba, pt);
        char path[1024];
        if (out_dir) sprintf(path, "%s/%s-%s.svg", out_dir, nome_base_combo, sfx);
        else sprintf(path, "%s-%s.svg", nome_base_combo, sfx);
        
        gerar_svg(NULL, NULL, poligono, temp_bomba, path);
        killList(temp_bomba, NULL);
    }
    
    killList(poligono, destroy_segmento_void);
}

/*=================================================
    IMPLEMENTAÇÃO DO COMANDO 'p'
===================================================*/
static void tratar_p(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, const char *nome_base_combo, Lista registros_visuais, Lista pontos_bombas, char tipo_ord, int cutoff) {
    double x, y;
    char cor[100], sfx[100];

    sscanf(params, "%lf %lf %s %s", &x, &y, cor, sfx);
    fprintf(txt, "\n[*] Comando 'p': Pintar em (%.2f, %.2f) cor: %s\n", x, y, cor);

    double *pt = malloc(2 * sizeof(double));
    pt[0] = x;
    pt[1] = y;
    insert(pontos_bombas, pt);

    // 1. Calcula a área atingida
    Lista poligono = obter_poligono_explosao(x, y, formas, anteparos, tipo_ord, cutoff);

    acumular_desenho(registros_visuais, poligono);

    // 2. Aplica a tinta nas FORMAS
    Posic p = getFirst(formas);
    while (p) {
        Forma f = get(formas, p);
        if (forma_foi_atingida(f, poligono)) {
            forma_atualizar_cor(f, cor);
            fprintf(txt, "  -> Pintado: ID %d (%s)\n", get_forma_id_generico(f), get_tipo_forma_str(f));
        }
        p = getNext(formas, p);
    }
    
    // 3. Aplica a tinta nos ANTEPAROS (não muda cor visual, apenas reporta)
    processar_efeito_em_anteparos(anteparos, poligono, txt, 'p', cor);

    // 3. SVG de Debug
    if (strcmp(sfx, "-") != 0) {
        Lista temp_bomba = createList();
        insert(temp_bomba, pt);
        char path[1024];
        if (out_dir) sprintf(path, "%s/%s-%s.svg", out_dir, nome_base_combo, sfx);
        else sprintf(path, "%s-%s.svg", nome_base_combo, sfx);
        
        gerar_svg(NULL, NULL, poligono, temp_bomba, path);
        killList(temp_bomba, NULL);
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
            // Testa os dois extremos e o ponto médio
            double x1 = linha_get_x1(dados);
            double y1 = linha_get_y1(dados);
            double x2 = linha_get_x2(dados);
            double y2 = linha_get_y2(dados);
            double mx = (x1 + x2) / 2.0;
            double my = (y1 + y2) / 2.0;
            
            // Testa pontos dentro do polígono
            if (ponto_dentro_poligono(x1, y1, poligono)) return 1;
            if (ponto_dentro_poligono(x2, y2, poligono)) return 1;
            if (ponto_dentro_poligono(mx, my, poligono)) return 1;
            
            // Testa intersecção com arestas do polígono
            // O polígono é uma lista de PontoPoligono (x, y), não Segmento
            typedef struct { double x, y; } PontoPoligono;
            Posic p = getFirst(poligono);
            Posic p_next = p ? getNext(poligono, p) : NULL;
            while (p && p_next) {
                PontoPoligono *pt1 = (PontoPoligono*)get(poligono, p);
                PontoPoligono *pt2 = (PontoPoligono*)get(poligono, p_next);
                
                if (tem_interseccao(x1, y1, x2, y2, pt1->x, pt1->y, pt2->x, pt2->y)) {
                    return 1;
                }
                p = p_next;
                p_next = getNext(poligono, p_next);
            }
            // Fecha o polígono (último ponto -> primeiro ponto)
            if (length(poligono) > 2) {
                PontoPoligono *pt_last = (PontoPoligono*)get(poligono, getLast(poligono));
                PontoPoligono *pt_first = (PontoPoligono*)get(poligono, getFirst(poligono));
                if (tem_interseccao(x1, y1, x2, y2, pt_last->x, pt_last->y, pt_first->x, pt_first->y)) {
                    return 1;
                }
            }
            break;
        }
        case TEXTO: {
            // Testa a âncora + converte para linha de colisão
            double tx = texto_get_x(dados);
            double ty = texto_get_y(dados);
            if (ponto_dentro_poligono(tx, ty, poligono)) return 1;
            
            // Cria linha de colisão a partir do texto (aproximação)
            const char *conteudo = texto_get_conteudo(dados);
            double comp = 10.0 * strlen(conteudo);
            char ancora = texto_get_ancora(dados);
            
            double lx1, lx2;
            if (ancora == 'i') {
                lx1 = tx; lx2 = tx + comp;
            } else if (ancora == 'f') {
                lx1 = tx - comp; lx2 = tx;
            } else { // meio
                lx1 = tx - comp/2.0; lx2 = tx + comp/2.0;
            }
            
            // Testa extremos da linha de colisão
            if (ponto_dentro_poligono(lx1, ty, poligono)) return 1;
            if (ponto_dentro_poligono(lx2, ty, poligono)) return 1;
            
            // Testa intersecção com arestas do polígono
            // O polígono é uma lista de PontoPoligono (x, y), não Segmento
            typedef struct { double x, y; } PontoPoligono2;
            Posic pt = getFirst(poligono);
            Posic pt_next = pt ? getNext(poligono, pt) : NULL;
            while (pt && pt_next) {
                PontoPoligono2 *ppt1 = (PontoPoligono2*)get(poligono, pt);
                PontoPoligono2 *ppt2 = (PontoPoligono2*)get(poligono, pt_next);
                
                if (tem_interseccao(lx1, ty, lx2, ty, ppt1->x, ppt1->y, ppt2->x, ppt2->y)) {
                    return 1;
                }
                pt = pt_next;
                pt_next = getNext(poligono, pt_next);
            }
            // Fecha o polígono
            if (length(poligono) > 2) {
                PontoPoligono2 *ppt_last = (PontoPoligono2*)get(poligono, getLast(poligono));
                PontoPoligono2 *ppt_first = (PontoPoligono2*)get(poligono, getFirst(poligono));
                if (tem_interseccao(lx1, ty, lx2, ty, ppt_last->x, ppt_last->y, ppt_first->x, ppt_first->y)) {
                    return 1;
                }
            }
            break;
        }
        default:
            return 0;
    }
    return 0;
}

static Lista obter_poligono_explosao(double x, double y, Lista formas, Lista anteparos, char tipo_ord, int cutoff) {

    // Calcula bounding box baseado nos conteúdos reais
    Limites box = calcular_limites_mundo(formas);
    
    // Expande com ponto da bomba e anteparos
    limites_expandir_ponto(box, x, y);
    limites_expandir_segmentos(box, anteparos);

    double bx = x + 0.00001;
    double by = y + 0.00001;

    Lista poligono = calcular_visibilidade(bx, by, anteparos, box, tipo_ord, cutoff);

    destruir_limites(box);
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
            
            fprintf(txt, "  -> Destruido: ID %d (%s)\n", id, get_tipo_forma_str(f));
            
            removePosic(formas, p);
            destroy_forma(f);
        }
        p = p_next;
    }
}


// Destructor para listas de polígonos (cada elemento é uma Lista de pontos)
static void destroy_poligono_lista(void *p) {
    Lista poligono = (Lista)p;
    if (poligono) {
        killList(poligono, free); // Libera os pontos com free
    }
}

static void acumular_desenho(Lista acumulador, Lista poligono_atual) {
    if (!poligono_atual || length(poligono_atual) < 3) return;
    
    // Estrutura de ponto usada em visibilidade.c
    typedef struct { double x, y; } PontoPoligono;
    
    // Cria uma cópia do polígono para armazenar
    Lista copia_poligono = createList();
    
    Posic p = getFirst(poligono_atual);
    while(p) {
        PontoPoligono *pt = (PontoPoligono*)get(poligono_atual, p);
        
        // Cria cópia do ponto
        PontoPoligono *copia = malloc(sizeof(PontoPoligono));
        copia->x = pt->x;
        copia->y = pt->y;
        insert(copia_poligono, copia);
        
        p = getNext(poligono_atual, p);
    }
    
    // Insere o POLÍGONO INTEIRO como um elemento do acumulador (lista de listas)
    insert(acumulador, copia_poligono);
}

static void destroy_segmento_void(void *p) {
    if (p) destroy_segmento((Segmento)p);
}


// Verifica se um ponto está próximo a algum vértice do polígono (na borda)
static int ponto_na_borda_poligono(double px, double py, Lista poligono) {
    typedef struct { double x, y; } PontoPoligono;
    const double TOL = 1.0; // Tolerância de 1 pixel
    
    Posic p = getFirst(poligono);
    while (p) {
        PontoPoligono *pt = (PontoPoligono*)get(poligono, p);
        double dx = px - pt->x;
        double dy = py - pt->y;
        if (dx*dx + dy*dy < TOL*TOL) {
            return 1; // Ponto está na borda
        }
        p = getNext(poligono, p);
    }
    return 0;
}

static int anteparo_foi_atingido(Segmento s, Lista poligono) {
    double x1 = get_segmento_x1(s);
    double y1 = get_segmento_y1(s);
    double x2 = get_segmento_x2(s);
    double y2 = get_segmento_y2(s);
    
    // Verifica se os pontos estão na borda (são vértices do polígono)
    int p1_na_borda = ponto_na_borda_poligono(x1, y1, poligono);
    int p2_na_borda = ponto_na_borda_poligono(x2, y2, poligono);
    
    // Se QUALQUER ponta está na borda, o anteparo foi TOCADO → ATINGIDO
    if (p1_na_borda || p2_na_borda) {
        return 1;
    }
    
    // Verifica se alguma ponta está DENTRO do polígono
    if (ponto_dentro_poligono(x1, y1, poligono) || 
        ponto_dentro_poligono(x2, y2, poligono)) {
        return 1;
    }
    
    // Verifica ponto médio
    double mx = (x1 + x2) / 2.0;
    double my = (y1 + y2) / 2.0;
    if (ponto_dentro_poligono(mx, my, poligono) || ponto_na_borda_poligono(mx, my, poligono)) {
        return 1;
    }
    
    return 0;
}

static void processar_efeito_em_anteparos(Lista anteparos, Lista poligono, FILE *txt, char tipo_efeito, char* param_extra) {
    Posic p = getFirst(anteparos);
    
    // Para clonagem, precisamos dos parâmetros de offset
    double dx = 0, dy = 0;
    int id_base = 90000;
    
    if (tipo_efeito == 'c' && param_extra) {
        double *params = (double*)param_extra;
        dx = params[0];
        dy = params[1];
        id_base = (int)params[2];
    }
    
    Lista novos_anteparos = createList();

    while (p) {
        Segmento s = get(anteparos, p);
        Posic p_next = getNext(anteparos, p);
        
        if (anteparo_foi_atingido(s, poligono)) {
            int id = get_segmento_id(s);
            
            if (tipo_efeito == 'd') {
                fprintf(txt, "  -> Anteparo ID %d destruído (Deixa de bloquear).\n", id);
                removePosic(anteparos, p);
                destroy_segmento(s);
            }
            else if (tipo_efeito == 'p') {
                if (param_extra) {
                    set_segmento_cor(s, param_extra);
                }
                fprintf(txt, "  -> Anteparo ID %d atingido pela tinta.\n", id);
            }
            else if (tipo_efeito == 'c') {
                // Clone: cria novo segmento deslocado
                double sx1 = get_segmento_x1(s);
                double sy1 = get_segmento_y1(s);
                double sx2 = get_segmento_x2(s);
                double sy2 = get_segmento_y2(s);
                
                int novo_id = id_base++;
                Segmento clone = create_segmento(novo_id,
                                                 sx1 + dx, sy1 + dy,
                                                 sx2 + dx, sy2 + dy);
                insert(novos_anteparos, clone);
                fprintf(txt, "  -> Clonado Anteparo ID %d (Novo ID %d)\n", id, novo_id);
                
                // Atualiza o parâmetro para refletir o próximo ID
                if (param_extra) {
                    double *params = (double*)param_extra;
                    params[2] = (double)id_base;
                }
            }
        }
        p = p_next;
    }

    // Mescla os novos anteparos na lista principal
    while(length(novos_anteparos) > 0) {
        Posic p_tmp = getFirst(novos_anteparos);
        Segmento c = removePosic(novos_anteparos, p_tmp);
        insert(anteparos, c);
    }
    killList(novos_anteparos, NULL);
}