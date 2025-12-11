#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "visibilidade.h"
#include "segmento.h"
#include "qsort.h"
#include "merge.h"
#include "geometria.h"
#include "arvore.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    double x, y;
    double angulo;
    double distancia;
    int tipo;         
    void *segmento;    
} VerticeStruct;

/* Variáveis estáticas */
static double BOMBA_X;
static double BOMBA_Y;
static Vertice VERTICE_ATUAL;

void set_contexto_bomba(double x, double y) {
    BOMBA_X = x;
    BOMBA_Y = y;
}

void set_vertice_atual(Vertice v) {
    VERTICE_ATUAL = v;
}

/* --- Implementação dos Getters --- */
double get_vertice_x(Vertice v) { return ((VerticeStruct*)v)->x; }
double get_vertice_y(Vertice v) { return ((VerticeStruct*)v)->y; }
double get_vertice_angulo(Vertice v) { return ((VerticeStruct*)v)->angulo; }
double get_vertice_distancia(Vertice v) { return ((VerticeStruct*)v)->distancia; }
void* get_vertice_segmento(Vertice v) { return ((VerticeStruct*)v)->segmento; }
int get_vertice_tipo(Vertice v) { return ((VerticeStruct*)v)->tipo; }

/* --- Funções Auxiliares Internas --- */

double dist_sq(double x1, double y1, double x2, double y2) {
    return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
}

double calcular_angulo(double cy, double cx, double py, double px) {
    double ang = atan2(py - cy, px - cx);
    if (ang < 0) ang += 2 * M_PI;
    return ang;
}

/**
 * Calcula a distância do raio até um segmento para um ângulo específico.
 */
static double distancia_raio_segmento(double ox, double oy, double angulo, void* seg) {
    const double EPSILON = 1e-12;
    
    double x1 = get_segmento_x1(seg);
    double y1 = get_segmento_y1(seg);
    double x2 = get_segmento_x2(seg);
    double y2 = get_segmento_y2(seg);
    
    // Direção do raio
    double rdx = cos(angulo);
    double rdy = sin(angulo);
    
    // Direção do segmento
    double sdx = x2 - x1;
    double sdy = y2 - y1;
    
    // Vetor origem -> ponto inicial do segmento
    double qpx = x1 - ox;
    double qpy = y1 - oy;
    
    // Determinante (produto vetorial 2D)
    double rxs = rdx * sdy - rdy * sdx;
    
    // Se paralelos, não há intersecção
    if (fabs(rxs) < EPSILON) return INFINITY;
    
    // Parâmetro t (distância ao longo do raio)
    double t = (qpx * sdy - qpy * sdx) / rxs;
    
    // Parâmetro u (posição ao longo do segmento, 0-1)
    double u = (qpx * rdy - qpy * rdx) / rxs;
    
    // Ajusta para tolerância numérica
    if (fabs(t) < EPSILON) t = 0.0;
    if (fabs(u) < EPSILON) u = 0.0;
    if (fabs(u - 1.0) < EPSILON) u = 1.0;
    
    // Intersecção válida: t >= 0 (à frente) e u in [0,1] (dentro do segmento)
    if (t >= 0.0 && u >= 0.0 && u <= 1.0) return t;
    
    return INFINITY;
}

/**
 * Calcula a distância do raio até a bounding box.
 */
static double distancia_raio_bbox(double ox, double oy, double angulo, 
                                  double min_x, double min_y, double max_x, double max_y) {
    const double EPSILON = 1e-12;
    
    double dx = cos(angulo);
    double dy = sin(angulo);
    double t_min = INFINITY;
    
    // Testa intersecção com cada lado da bbox
    if (fabs(dx) > EPSILON) {
        double t = (min_x - ox) / dx;
        if (t >= 0) t_min = fmin(t_min, t);
        t = (max_x - ox) / dx;
        if (t >= 0) t_min = fmin(t_min, t);
    }
    
    if (fabs(dy) > EPSILON) {
        double t = (min_y - oy) / dy;
        if (t >= 0) t_min = fmin(t_min, t);
        t = (max_y - oy) / dy;
        if (t >= 0) t_min = fmin(t_min, t);
    }
    
    return t_min;
}

/* Construtor interno auxiliar */
static Vertice criar_vertice_interno(double x, double y, double ang, double dist, int tipo, void* seg) {
    VerticeStruct *v = malloc(sizeof(VerticeStruct));
    v->x = x; v->y = y;
    v->angulo = ang;
    v->distancia = dist;
    v->tipo = tipo;
    v->segmento = seg;
    return (Vertice)v;
}

/* Comparador para o Sort (agora faz cast para VerticeStruct) */
int comparar_vertices(const void *a, const void *b) {
    VerticeStruct *v1 = *(VerticeStruct**)a;
    VerticeStruct *v2 = *(VerticeStruct**)b;

    // 1. Ângulo (Tolerância robusta)
    if (fabs(v1->angulo - v2->angulo) > 1e-9) {
        return (v1->angulo < v2->angulo) ? -1 : 1;
    }

    if (v1->tipo != v2->tipo) {
    // TIPO_INICIO (0) deve ser menor que TIPO_FIM (1)
        return (v1->tipo == TIPO_INICIO) ? -1 : 1; 
    }

    // 3. Distância
    // Se ambos são Inicio ou ambos são Fim, processa o mais perto primeiro?
    if (fabs(v1->distancia - v2->distancia) > 1e-9) {
        return (v1->distancia < v2->distancia) ? -1 : 1;
    }

    return 0;
}

/* --- Função Principal --- */
/* Helper: Normaliza ângulo para [0, 2PI) */
double normalizar_angulo(double ang) {
    while (ang < 0) ang += 2 * M_PI;
    while (ang >= 2 * M_PI) ang -= 2 * M_PI;
    return ang;
}

/* Helper: Verifica se cruza o eixo X positivo (Leste) */
int cruza_eixo_zero(double ang1, double ang2) {
    // Se a diferença absoluta for grande (ex: > 180 graus), cruzou o zero
    return fabs(ang1 - ang2) > M_PI;
}

Vertice* preparar_vertices_ordenados(double cx, double cy, Lista lista_segs, int *qtd_out, char tipo_ord, int cutoff) {
    // Aloca com margem de segurança (3x) para os cortes
    Vertice *vetor = malloc(length(lista_segs) * 3 * sizeof(Vertice)); 
    int count = 0;

    Posic p = getFirst(lista_segs);
    while (p) {
        void* s = get(lista_segs, p);
        
        double x1 = get_segmento_x1(s);
        double y1 = get_segmento_y1(s);
        double x2 = get_segmento_x2(s);
        double y2 = get_segmento_y2(s);

        double ang1 = normalizar_angulo(calcular_angulo(cy, cx, y1, x1));
        double ang2 = normalizar_angulo(calcular_angulo(cy, cx, y2, x2));
        
        double d1 = dist_sq(cx, cy, x1, y1);
        double d2 = dist_sq(cx, cy, x2, y2);

        /* DETECÇÃO DE CRUZAMENTO DO EIXO ZERO */
        if (fabs(ang1 - ang2) > M_PI) {
            
            double t = (cy - y1) / (y2 - y1);
            double x_cut = x1 + t * (x2 - x1);
            double y_cut = cy; // Por definição
            double d_cut = dist_sq(cx, cy, x_cut, y_cut);

            // Determina qual ângulo é o "Grande" (Start original) e qual é o "Pequeno" (End original)
            // No sistema 0..360, o Start é o valor maior (ex: 350) e End é o menor (ex: 10)
            double ang_start, ang_end;
            double x_start, y_start, d_start;
            double x_end, y_end, d_end;

            if (ang1 > ang2) {
                // Caso normal do cruzeiro (350 -> 10)
                ang_start = ang1; x_start = x1; y_start = y1; d_start = d1;
                ang_end = ang2;   x_end = x2;   y_end = y2;   d_end = d2;
            } else {
                // Caso inverso (10 -> 350)
                ang_start = ang2; x_start = x2; y_start = y2; d_start = d2;
                ang_end = ang1;   x_end = x1;   y_end = y1;   d_end = d1;
            }

            // --- CRIAÇÃO DOS DOIS PEDAÇOS ---

            // Pedaço 1: Do Início Original até 360 (2PI)
            // Início: (x_start, y_start) @ ang_start
            // Fim:    (x_cut, y_cut)     @ 2*PI (usamos um epsilon menor para ficar no fim da lista)
            vetor[count++] = criar_vertice_interno(x_start, y_start, ang_start, d_start, TIPO_INICIO, s);
            vetor[count++] = criar_vertice_interno(x_cut, y_cut, 2 * M_PI - 1e-9, d_cut, TIPO_FIM, s);

            // Pedaço 2: De 0 até o Fim Original
            // Início: (x_cut, y_cut)   @ 0.0
            // Fim:    (x_end, y_end)   @ ang_end
            vetor[count++] = criar_vertice_interno(x_cut, y_cut, 0.0, d_cut, TIPO_INICIO, s);
            vetor[count++] = criar_vertice_interno(x_end, y_end, ang_end, d_end, TIPO_FIM, s);

        } else {
            // --- CASO NORMAL (Não cruza o zero) ---
            // O menor ângulo é o início, o maior é o fim.
            
            if (ang1 < ang2) {
                vetor[count++] = criar_vertice_interno(x1, y1, ang1, d1, TIPO_INICIO, s);
                vetor[count++] = criar_vertice_interno(x2, y2, ang2, d2, TIPO_FIM, s);
            } else {
                vetor[count++] = criar_vertice_interno(x2, y2, ang2, d2, TIPO_INICIO, s);
                vetor[count++] = criar_vertice_interno(x1, y1, ang1, d1, TIPO_FIM, s);
            }
        }

        p = getNext(lista_segs, p);
    }

    *qtd_out = count;

    if (tipo_ord == 'm') merge_sort((void**)vetor, count, cutoff, comparar_vertices);
    else quick_sort((void**)vetor, count, comparar_vertices);

    return vetor;
}

void destroy_vertice(Vertice v) {
    if (v == NULL) return;
    free(v);
}

void destruir_vetor_vertices(Vertice *vetor, int qtd) {
    for (int i = 0; i < qtd; i++) {
        destroy_vertice(vetor[i]); // Libera a struct interna
    }
    free(vetor); // Libera o array de ponteiros
}

static double distancia_interseccao_raio(void *seg_void, double dest_x, double dest_y) {
    // Recupera coordenadas do segmento usando os getters
    double sx1 = get_segmento_x1(seg_void);
    double sy1 = get_segmento_y1(seg_void);
    double sx2 = get_segmento_x2(seg_void);
    double sy2 = get_segmento_y2(seg_void);

    // Verifica se os pontos do segmento estão em lados opostos do raio
    double o1 = orientacao(BOMBA_X, BOMBA_Y, dest_x, dest_y, sx1, sy1);
    double o2 = orientacao(BOMBA_X, BOMBA_Y, dest_x, dest_y, sx2, sy2);

    // Se sinais iguais (e não zero), o segmento está todo de um lado do raio (não cruza)
    if ((o1 >= 1e-9 && o2 >= 1e-9) || (o1 <= -1e-9 && o2 <= -1e-9)) {
        return -1.0;
    }

    // Calcula ponto de intersecção usando álgebra linear (Cramer)
    // Reta do Raio: A1x + B1y = C1
    double A1 = BOMBA_Y - dest_y;
    double B1 = dest_x - BOMBA_X;
    double C1 = A1*BOMBA_X + B1*BOMBA_Y;

    // Reta do Segmento: A2x + B2y = C2
    double A2 = sy1 - sy2;
    double B2 = sx2 - sx1;
    double C2 = A2*sx1 + B2*sy1;

    double det = A1*B2 - A2*B1;
    
    if (fabs(det) < 1e-9) return -1.0; // Paralelos

    double x_int = (B2*C1 - B1*C2) / det;
    double y_int = (A1*C2 - A2*C1) / det;

    // Retorna distância ao quadrado da bomba até a intersecção
    return dist_sq(BOMBA_X, BOMBA_Y, x_int, y_int);
}

/*
 * Comparador Baseado em Ponto Médio (Evita problemas nos vértices)
 */
int comparar_segmentos_arvore(const void* a, const void* b) {
    void* s1 = (void*)a;
    void* s2 = (void*)b;
    
    if (s1 == s2) return 0;

    // 1. Recupera o raio atual da varredura (Bomba -> VerticeAtual)
    double vx = get_vertice_x(VERTICE_ATUAL);
    double vy = get_vertice_y(VERTICE_ATUAL);

    // Calcula a distância da Bomba até a intersecção deste raio com S1 e S2
    double dist1 = distancia_interseccao_raio(s1, vx, vy);
    double dist2 = distancia_interseccao_raio(s2, vx, vy);

    // Se o raio não acertar o segmento (ex: erro de precisão ou segmento terminando), 
    // tentamos usar o "Início" do próprio segmento como fallback, 
    // pois segmentos ativos DEVEM ser interceptados pelo raio de varredura.
    if (dist1 < 0) dist1 = dist_sq(BOMBA_X, BOMBA_Y, get_segmento_x1(s1), get_segmento_y1(s1));
    if (dist2 < 0) dist2 = dist_sq(BOMBA_X, BOMBA_Y, get_segmento_x1(s2), get_segmento_y1(s2));

    // 2. Comparação Direta de Distância
    if (fabs(dist1 - dist2) > 1e-7) {
        return (dist1 < dist2) ? -1 : 1; // Menor distância ganha (vem antes)
    }

    // 3. Empate (Vértices Compartilhados)
    // Se as distâncias são iguais, usa a orientação do outro extremo para desempatar
    double s1x1 = get_segmento_x1(s1), s1y1 = get_segmento_y1(s1);
    double s1x2 = get_segmento_x2(s1), s1y2 = get_segmento_y2(s1);
    double s2x1 = get_segmento_x1(s2), s2y1 = get_segmento_y1(s2);
    double s2x2 = get_segmento_x2(s2), s2y2 = get_segmento_y2(s2);

    // Se S1 e S2 compartilham inicio (distâncias iguais)
    if (dist_sq(s1x1, s1y1, s2x1, s2y1) < 1e-7) {
        double o = orientacao(BOMBA_X, BOMBA_Y, s1x2, s1y2, s2x2, s2y2);
        return (o > 0) ? -1 : 1; 
    }

    // Fallback ID
    return (get_segmento_id(s1) < get_segmento_id(s2)) ? -1 : 1;
}

// Variáveis estáticas para bbox (usadas pelo raycast)
static double BBOX_MIN_X, BBOX_MIN_Y, BBOX_MAX_X, BBOX_MAX_Y;

static void set_bbox_limits(double min_x, double min_y, double max_x, double max_y) {
    BBOX_MIN_X = min_x;
    BBOX_MIN_Y = min_y;
    BBOX_MAX_X = max_x;
    BBOX_MAX_Y = max_y;
}

/**
 * Lança um raio na direção do ângulo e retorna o ponto de intersecção mais próximo.
 */
static void raycast(double ox, double oy, double angulo, Arvore ativos,
                    double *out_x, double *out_y) {
    double dist_min = INFINITY;
    
    // 1. Verifica intersecção com todos os segmentos ativos na árvore
    Posic p = tree_get_first(ativos);
    while (p) {
        void* seg = tree_get_value(ativos, p);
        if (seg) {
            double d = distancia_raio_segmento(ox, oy, angulo, seg);
            if (d < dist_min) dist_min = d;
        }
        p = tree_get_next(ativos, p);
    }
    
    
    // Fallback para a bbox
    double dist_bbox = distancia_raio_bbox(ox, oy, angulo, BBOX_MIN_X, BBOX_MIN_Y, BBOX_MAX_X, BBOX_MAX_Y);
    if (dist_bbox < dist_min) dist_min = dist_bbox;
    
    // Calcula ponto de intersecção
    *out_x = ox + dist_min * cos(angulo);
    *out_y = oy + dist_min * sin(angulo);
}

// Estrutura simples para ponto (x,y) - usada no polígono de visibilidade
typedef struct {
    double x, y;
} PontoPoligono;

static PontoPoligono* criar_ponto_poligono(double x, double y) {
    PontoPoligono *p = malloc(sizeof(PontoPoligono));
    p->x = x;
    p->y = y;
    return p;
}

// Adiciona um ponto ao polígono (lista de pontos)
static void adicionar_ponto_poligono(Lista poligono, double x, double y) {
    // Evita pontos duplicados consecutivos
    Posic ultimo = getLast(poligono);
    if (ultimo) {
        PontoPoligono *p_ultimo = (PontoPoligono*)get(poligono, ultimo);
        if (fabs(p_ultimo->x - x) < 1e-6 && fabs(p_ultimo->y - y) < 1e-6) {
            return; // Ponto duplicado, não adiciona
        }
    }
    insert(poligono, criar_ponto_poligono(x, y));
}


/* =========================================================
 * ALGORITMO PRINCIPAL DE VISIBILIDADE
 * ========================================================= */


Lista calcular_visibilidade(double bx, double by, Lista anteparos, Limites box_mundo, char tipo_ord, int cutoff) {
    
    // 1. CONFIGURA CONTEXTO
    set_contexto_bomba(bx, by);

    // 2. CRIA PAREDES DO MUNDO - BBOX DINÂMICA baseada no conteúdo
    // Usa o bounding box passado como parâmetro, com uma margem de segurança
    double margin = 50.0;
    double min_x = get_limites_min_x(box_mundo) - margin;
    double max_x = get_limites_max_x(box_mundo) + margin;
    double min_y = get_limites_min_y(box_mundo) - margin;
    double max_y = get_limites_max_y(box_mundo) + margin;

    // ID temporário seguro
    int id_temp = 99000;

    // Cria os 4 segmentos de borda
    Segmento p1 = create_segmento(id_temp++, min_x, min_y, max_x, min_y); // Topo
    Segmento p2 = create_segmento(id_temp++, max_x, min_y, max_x, max_y); // Direita
    Segmento p3 = create_segmento(id_temp++, min_x, max_y, max_x, max_y); // Baixo
    Segmento p4 = create_segmento(id_temp++, min_x, min_y, min_x, max_y); // Esquerda

    // Insere e guarda posição para remoção rápida (O(1))
    Posic no_p1 = insert(anteparos, p1);
    Posic no_p2 = insert(anteparos, p2);
    Posic no_p3 = insert(anteparos, p3);
    Posic no_p4 = insert(anteparos, p4);
    
    // Define os limites da bbox para a função raycast
    set_bbox_limits(min_x, min_y, max_x, max_y);

    // 3. PREPARAÇÃO (Vértices e Árvore)
    int qtd_eventos = 0;
    Vertice *eventos = preparar_vertices_ordenados(bx, by, anteparos, &qtd_eventos, tipo_ord, cutoff);

    Lista poligono_visivel = createList();
    Arvore ativos = tree_create(comparar_segmentos_arvore);
    
    // 4. PRÉ-CARREGAMENTO DE BARREIRAS NO ÂNGULO 0
    const double EPSILON_ANGLE = 1e-9;
    for (int i = 0; i < qtd_eventos; i++) {
        Vertice v = eventos[i];
        double angulo = get_vertice_angulo(v);
        int tipo = get_vertice_tipo(v);
        
        if (tipo == TIPO_INICIO && angulo < EPSILON_ANGLE) {
            void* seg_v = get_vertice_segmento(v);
            double dist_key = get_vertice_distancia(v);
            tree_insert_with_key(&ativos, dist_key, seg_v);
        }
    }


    // 5. VARREDURA ANGULAR
    
    // Adiciona ponto inicial no ângulo 0
    double init_x, init_y;
    raycast(bx, by, 0.0, ativos, &init_x, &init_y);
    adicionar_ponto_poligono(poligono_visivel, init_x, init_y);

    for (int idx = 0; idx < qtd_eventos; idx++) {
        Vertice v = eventos[idx];
        int tipo = get_vertice_tipo(v);
        void* seg_v = get_vertice_segmento(v);
        double angulo = get_vertice_angulo(v);
        double dist_key = get_vertice_distancia(v);
        
        // Pula eventos de início no ângulo 0 (já foram pré-carregados)
        if (tipo == TIPO_INICIO && angulo < EPSILON_ANGLE) continue;
        
        // PRE-UPDATE: Faz raycast ANTES de atualizar a árvore
        double pre_x, pre_y;
        raycast(bx, by, angulo, ativos, &pre_x, &pre_y);
        adicionar_ponto_poligono(poligono_visivel, pre_x, pre_y);
        
        // ATUALIZA ÁRVORE
        if (tipo == TIPO_INICIO) {
            tree_insert_with_key(&ativos, dist_key, seg_v);
        } else {
            tree_remove_with_key(ativos, dist_key, seg_v);
        }
        
        // POST-UPDATE: Faz raycast APÓS atualizar a árvore
        double post_x, post_y;
        raycast(bx, by, angulo, ativos, &post_x, &post_y);
        adicionar_ponto_poligono(poligono_visivel, post_x, post_y);
    }

    // 6. LIMPEZA INTERNA
    tree_destroy(ativos, NULL);
    destruir_vetor_vertices(eventos, qtd_eventos);

    // 7. REMOVE PAREDES DO MUNDO (Limpa a sujeira que criou)
    removePosic(anteparos, no_p1);
    removePosic(anteparos, no_p2);
    removePosic(anteparos, no_p3);
    removePosic(anteparos, no_p4);

    destroy_segmento(p1); destroy_segmento(p2);
    destroy_segmento(p3); destroy_segmento(p4);

    return poligono_visivel;
}