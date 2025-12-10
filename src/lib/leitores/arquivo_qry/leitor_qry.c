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
static void tratar_d(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, Lista registros, Lista pontos_bombas, char tipo_ord, int cutoff);
static void tratar_p(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, Lista registros, Lista pontos_bombas, char tipo_ord, int cutoff);
static void tratar_cln(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, int *id_global, Lista registros, Lista pontos_bombas, char tipo_ord, int cutoff);
static void processar_destruicao(Lista formas, Lista poligono_visivel, FILE *txt);
static int forma_foi_atingida(Forma f, Lista poligono);
static void acumular_desenho(Lista acumulador, Lista poligono_atual);
static void extrair_nome_base(const char *path, char *dest);
static void destroy_segmento_void(void *p);
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
            tratar_d(params, lista_formas, lista_anteparos, txt, output_dir, registros_visuais, pontos_bombas, tipo_ord, cutoff);
        }
        else if (strcmp(comando, "p") == 0) {
            tratar_p(params, lista_formas, lista_anteparos, txt, output_dir, registros_visuais, pontos_bombas, tipo_ord, cutoff);
        }
        else if (strcmp(comando, "cln") == 0) {
            tratar_cln(params, lista_formas, lista_anteparos, txt, output_dir, &contador_id_segmento, registros_visuais, pontos_bombas, tipo_ord, cutoff);
        }
    }

    char nome_final_svg[1024];
    char nome_qry_sem_ext[256];
    extrair_nome_base(path_qry, nome_qry_sem_ext);
    
    // Nome: nomegeo-nomeqry.svg
    sprintf(nome_final_svg, "%s/%s-%s.svg", output_dir, nome_base_geo, nome_qry_sem_ext);
    


    
    gerar_svg(lista_formas, lista_anteparos, registros_visuais, pontos_bombas, nome_final_svg);

    // --- LIMPEZA ---
    fclose(qry);
    fclose(txt);
    
    killList(registros_visuais, destroy_segmento_void);
    
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
                     const char *output_dir, Lista registros_visuais, Lista pontos_bombas, 
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

    // 4. GERA SVG DE DEBUG
    if (strcmp(sfx, "-") != 0) {
        Lista temp_bomba = createList();
        insert(temp_bomba, pt);
        char nome_arq[1024];
        
        if (output_dir) sprintf(nome_arq, "%s/bomba_%s.svg", output_dir, sfx);
        else sprintf(nome_arq, "bomba_%s.svg", sfx);
        
        // Corrigido: passa 'lista_formas', 'lista_anteparos' e 'poligono'
        gerar_svg(NULL, NULL, poligono, temp_bomba, nome_arq);

        killList(temp_bomba, NULL);
    }

    killList(poligono, destroy_segmento_void);
}

/*=================================================
    IMPLEMENTAÇÃO DO COMANDO 'cln'
===================================================*/

static void tratar_cln(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, int *id_global, Lista registros_visuais, Lista pontos_bombas, char tipo_ord, int cutoff) {
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
        if (out_dir) sprintf(path, "%s/clonagem_%s.svg", out_dir, sfx);
        else sprintf(path, "clonagem_%s.svg", sfx);
        
        gerar_svg(NULL, NULL, poligono, temp_bomba, path);
    }
    
    killList(poligono, destroy_segmento_void);
}

/*=================================================
    IMPLEMENTAÇÃO DO COMANDO 'p'
===================================================*/
static void tratar_p(char *params, Lista formas, Lista anteparos, FILE *txt, const char *out_dir, Lista registros_visuais, Lista pontos_bombas, char tipo_ord, int cutoff) {
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
        if (out_dir) sprintf(path, "%s/pintura_%s.svg", out_dir, sfx);
        else sprintf(path, "pintura_%s.svg", sfx);
        
        gerar_svg(NULL, NULL, poligono, temp_bomba, path);
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
            
            // Testa intersecção com arestas do polígono (como projeto_exemplo)
            Posic p = getFirst(poligono);
            while (p) {
                void *seg = get(poligono, p);
                double sx1 = get_segmento_x1(seg);
                double sy1 = get_segmento_y1(seg);
                double sx2 = get_segmento_x2(seg);
                double sy2 = get_segmento_y2(seg);
                
                if (tem_interseccao(x1, y1, x2, y2, sx1, sy1, sx2, sy2)) {
                    return 1;
                }
                p = getNext(poligono, p);
            }
            break;
        }
        case TEXTO: {
            // Testa a âncora + converte para linha de colisão (como projeto_exemplo)
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
            Posic pt = getFirst(poligono);
            while (pt) {
                void *seg = get(poligono, pt);
                double sx1 = get_segmento_x1(seg);
                double sy1 = get_segmento_y1(seg);
                double sx2 = get_segmento_x2(seg);
                double sy2 = get_segmento_y2(seg);
                
                if (tem_interseccao(lx1, ty, lx2, ty, sx1, sy1, sx2, sy2)) {
                    return 1;
                }
                pt = getNext(poligono, pt);
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


static void acumular_desenho(Lista acumulador, Lista poligono_atual) {
    if (!poligono_atual) return;
    
    // Estrutura de ponto usada em visibilidade.c
    typedef struct { double x, y; } PontoPoligono;
    
    Posic p = getFirst(poligono_atual);
    while(p) {
        PontoPoligono *pt = (PontoPoligono*)get(poligono_atual, p);
        
        // Cria cópia do ponto
        PontoPoligono *copia = malloc(sizeof(PontoPoligono));
        copia->x = pt->x;
        copia->y = pt->y;
        insert(acumulador, copia);
        
        p = getNext(poligono_atual, p);
    }
}

static void destroy_segmento_void(void *p) {
    if (p) destroy_segmento((Segmento)p);
}


static int anteparo_foi_atingido(Segmento s, Lista poligono) {
    double x1 = get_segmento_x1(s);
    double y1 = get_segmento_y1(s);
    double x2 = get_segmento_x2(s);
    double y2 = get_segmento_y2(s);
    
    // Verifica se alguma ponta do anteparo está dentro do polígono
    if (ponto_dentro_poligono(x1, y1, poligono) || 
        ponto_dentro_poligono(x2, y2, poligono)) {
        return 1;
    }
    
    // Verifica se o anteparo intersecta ou TOCA qualquer aresta do polígono
    Posic p = getFirst(poligono);
    while (p) {
        Segmento seg_poly = get(poligono, p);
        double px1 = get_segmento_x1(seg_poly);
        double py1 = get_segmento_y1(seg_poly);
        double px2 = get_segmento_x2(seg_poly);
        double py2 = get_segmento_y2(seg_poly);
        
        // Verifica interseção
        if (tem_interseccao(x1, y1, x2, y2, px1, py1, px2, py2)) {
            return 1;
        }
        
        // NOVO: Verifica se ponta do anteparo está SOBRE a aresta do polígono
        // (ray-casting não detecta pontos exatamente na borda)
        double tol = 1.0;  // Tolerância de 1 pixel
        
        // Verifica se (x1,y1) está sobre o segmento (px1,py1)-(px2,py2)
        double seg_len = sqrt((px2-px1)*(px2-px1) + (py2-py1)*(py2-py1));
        if (seg_len > 0.001) {
            // Distância perpendicular do ponto ao segmento
            double dist1 = fabs((py2-py1)*x1 - (px2-px1)*y1 + px2*py1 - py2*px1) / seg_len;
            // Verifica se está dentro do "retângulo" do segmento
            double minx = (px1 < px2 ? px1 : px2) - tol;
            double maxx = (px1 > px2 ? px1 : px2) + tol;
            double miny = (py1 < py2 ? py1 : py2) - tol;
            double maxy = (py1 > py2 ? py1 : py2) + tol;
            
            if (dist1 < tol && x1 >= minx && x1 <= maxx && y1 >= miny && y1 <= maxy) {
                return 1;  // Ponta 1 do anteparo está sobre aresta do polígono
            }
            
            // Verifica se (x2,y2) está sobre o segmento
            double dist2 = fabs((py2-py1)*x2 - (px2-px1)*y2 + px2*py1 - py2*px1) / seg_len;
            if (dist2 < tol && x2 >= minx && x2 <= maxx && y2 >= miny && y2 <= maxy) {
                return 1;  // Ponta 2 do anteparo está sobre aresta do polígono
            }
        }
        
        p = getNext(poligono, p);
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