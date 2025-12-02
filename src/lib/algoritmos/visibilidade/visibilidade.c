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

/* A definição da struct fica ESCONDIDA aqui */
typedef struct {
    double x, y;
    double angulo;
    double distancia;
    int tipo;          // 0=Inicio, 1=Fim
    void *segmento;    // Ponteiro para o segmento original
} VerticeStruct;

/* Variáveis estáticas */
static double BOMBA_X;
static double BOMBA_Y;

void set_contexto_bomba(double x, double y) {
    BOMBA_X = x;
    BOMBA_Y = y;
}

/* --- Implementação dos Getters --- */
double get_vertice_x(Vertice v) { return ((VerticeStruct*)v)->x; }
double get_vertice_y(Vertice v) { return ((VerticeStruct*)v)->y; }
double get_vertice_angulo(Vertice v) { return ((VerticeStruct*)v)->angulo; }
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

static int pontos_iguais(double x1, double y1, double x2, double y2) {
    return dist_sq(x1, y1, x2, y2) < 1e-7;
}

/* Comparador para o Sort (agora faz cast para VerticeStruct) */
int comparar_vertices(const void *a, const void *b) {
    VerticeStruct *v1 = *(VerticeStruct**)a;
    VerticeStruct *v2 = *(VerticeStruct**)b;

    // 1. Ângulo (Tolerância robusta)
    if (fabs(v1->angulo - v2->angulo) > 1e-9) {
        return (v1->angulo < v2->angulo) ? -1 : 1;
    }

    // 2. CRÍTICO: TIPO
    // Se os ângulos são iguais, processamos REMOÇÃO (Fim=1) antes de INSERÇÃO (Inicio=0).
    // Queremos que Fim venha ANTES.
    if (v1->tipo != v2->tipo) {
    // TIPO_INICIO (0) deve ser menor que TIPO_FIM (1)
        return (v1->tipo == TIPO_INICIO) ? -1 : 1; 
    }

    // 3. Distância (Opcional)
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

        /* * DETECÇÃO DE CRUZAMENTO DO EIXO ZERO 
         * Se a diferença angular for maior que PI (180 graus), significa que
         * o menor caminho cruza o ângulo 0/360.
         */
        if (fabs(ang1 - ang2) > M_PI) {
            // Precisamos calcular onde corta o eixo X positivo (y = cy, x > cx)
            // Intersecção da reta do segmento com a reta y = cy.
            // Fórmula da reta: P = P1 + t(P2-P1). Queremos P.y = cy.
            // cy = y1 + t(y2 - y1)  -->  t = (cy - y1) / (y2 - y1)
            
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

    // Ordenação
    // IMPORTANTE: Seu comparador deve tratar TIPO_FIM < TIPO_INICIO em caso de empate
    if (tipo_ord == 'm') merge_sort((void**)vetor, count, cutoff, comparar_vertices);
    else quick_sort((void**)vetor, count, comparar_vertices);

    return vetor;
}
void destroy_vertice(Vertice v) {
    if (v == NULL) return;
    
    // Libera apenas a estrutura do vértice criada no 'preparar_vertices'
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
 * Retorna -1 se S1 < S2 (S1 mais perto)
 * Retorna  1 se S1 > S2 (S1 mais longe)
 */
int comparar_segmentos_arvore(const void* a, const void* b) {
    void* s1 = (void*)a;
    void* s2 = (void*)b;

    if (s1 == s2) return 0;

    double s1x1 = get_segmento_x1(s1), s1y1 = get_segmento_y1(s1);
    double s1x2 = get_segmento_x2(s1), s1y2 = get_segmento_y2(s1);
    double s2x1 = get_segmento_x1(s2), s2y1 = get_segmento_y1(s2);
    double s2x2 = get_segmento_x2(s2), s2y2 = get_segmento_y2(s2);

    // --- 1. CASO CRÍTICO: VÉRTICES COMPARTILHADOS (Elimina a Diagonal) ---
    // Se S1 e S2 nascem no mesmo ponto (o canto do retângulo)
    if (pontos_iguais(s1x1, s1y1, s2x1, s2y1)) {
        // Usamos a orientação (determinante) do outro extremo para desempatar
        // Se o fim de S2 está à esquerda de S1, S2 está na frente.
        double o = orientacao(BOMBA_X, BOMBA_Y, s1x2, s1y2, s2x2, s2y2);
        return (o > 0) ? -1 : 1; 
    }
    
    // Se um é continuação do outro (S1 começa onde S2 termina)
    if (pontos_iguais(s1x1, s1y1, s2x2, s2y2)) {
        double o = orientacao(BOMBA_X, BOMBA_Y, s1x2, s1y2, s2x1, s2y1);
        return (o > 0) ? -1 : 1;
    }
    
    if (pontos_iguais(s2x1, s2y1, s1x2, s1y2)) {
        double o = orientacao(BOMBA_X, BOMBA_Y, s2x2, s2y2, s1x1, s1y1);
        return (o > 0) ? 1 : -1;
    }

    // --- 2. ESTRATÉGIA DO PONTO MÉDIO (Para casos gerais) ---
    double mid1_x = (s1x1 + s1x2) / 2.0;
    double mid1_y = (s1y1 + s1y2) / 2.0;
    
    double dist_s1 = dist_sq(BOMBA_X, BOMBA_Y, mid1_x, mid1_y);
    double dist_s2_no_raio_s1 = distancia_interseccao_raio(s2, mid1_x, mid1_y);

    if (dist_s2_no_raio_s1 >= 0) {
        if (dist_s2_no_raio_s1 < dist_s1 - 1e-7) return 1;  // S2 ganha
        if (dist_s2_no_raio_s1 > dist_s1 + 1e-7) return -1; // S1 ganha
    }

    // Prova real (Inverso)
    double mid2_x = (s2x1 + s2x2) / 2.0;
    double mid2_y = (s2y1 + s2y2) / 2.0;
    
    double dist_s2 = dist_sq(BOMBA_X, BOMBA_Y, mid2_x, mid2_y);
    double dist_s1_no_raio_s2 = distancia_interseccao_raio(s1, mid2_x, mid2_y);

    if (dist_s1_no_raio_s2 >= 0) {
        if (dist_s1_no_raio_s2 < dist_s2 - 1e-7) return -1; // S1 ganha
        if (dist_s1_no_raio_s2 > dist_s2 + 1e-7) return 1;  // S2 ganha
    }

    // Desempate por ID
    int id1 = get_segmento_id(s1);
    int id2 = get_segmento_id(s2);
    return (id1 < id2) ? -1 : 1;
}

static void adicionar_aresta_visivel(Lista poligono, double x1, double y1, double x2, double y2) {

    static int id_gen = 90000; 
    insert(poligono, create_segmento(id_gen++, x1, y1, x2, y2));
}


static void obter_ponto_interseccao(double bx, double by, Vertice v, void* seg, double *out_x, double *out_y) {
    double vx = get_vertice_x(v);
    double vy = get_vertice_y(v);
    
    // Coordenadas do Segmento
    double sx1 = get_segmento_x1(seg);
    double sy1 = get_segmento_y1(seg);
    double sx2 = get_segmento_x2(seg);
    double sy2 = get_segmento_y2(seg);

    // Calcula intersecção da Reta(Bomba, Vertice) com o Segmento
    calcular_interseccao(bx, by, vx, vy, sx1, sy1, sx2, sy2, out_x, out_y);
}

Lista calcular_visibilidade(double bx, double by, Lista anteparos) {
    
    // 1. Configura o contexto geométrico (para o comparador da árvore funcionar)
    set_contexto_bomba(bx, by);

    // 2. Prepara os Eventos (Extrai vértices e ordena angularmente)
    int qtd_eventos = 0;
    // O PDF pede MergeSort ('m') e cutoff 10 por padrão, mas pode ser configurável
    Vertice *eventos = preparar_vertices_ordenados(bx, by, anteparos, &qtd_eventos, 'm', 10);

    // 3. Inicializa Estruturas
    Lista poligono_visivel = createList();
    Arvore ativos = tree_create(comparar_segmentos_arvore);
    
    // O "Biombo" é o segmento que está bloqueando a visão atualmente
    void* biombo_atual = NULL;
    double ponto_ant_x = bx; // Ponto anterior do polígono (começa no angulo 0 ou no primeiro vertice)
    double ponto_ant_y = by; // (Simplificação: idealmente calculamos interseção com eixo X positivo)

    /*
     * PRÉ-PROCESSAMENTO (Opcional mas recomendado pelo PDF [302]):
     * Verificar quais segmentos já começam interceptando o eixo X positivo (angulo 0).
     * Como simplificação inicial, vamos assumir que a varredura começa no primeiro vértice ordenado.
     */

    // 4. Loop de Varredura (Sweep Line) 
    for (int i = 0; i < qtd_eventos; i++) {
        Vertice v = eventos[i];
        void* seg_v = get_vertice_segmento(v);
        int tipo = get_vertice_tipo(v); // 0=INICIO, 1=FIM

        // Recupera coordenadas do vértice atual
        double vx = get_vertice_x(v);
        double vy = get_vertice_y(v);

        if (tipo == TIPO_INICIO) {
            
            // Insere na árvore de ativos
            tree_insert(ativos, seg_v);

            // Verifica quem é o mais próximo AGORA
            void* seg_mais_proximo = tree_find_min(ativos);

            // Se o novo segmento passou a ser o mais próximo (bloqueou o biombo anterior)
            if (seg_mais_proximo == seg_v) {
                if (biombo_atual != NULL) {
                    // O raio bate no biombo antigo até este ângulo. Calculamos onde.
                    double ix, iy;
                    obter_ponto_interseccao(bx, by, v, biombo_atual, &ix, &iy);
                    
                    // Desenha aresta do ponto anterior até a intersecção no biombo antigo
                    // E depois conecta ao início do novo biombo (o vértice v)
                    adicionar_aresta_visivel(poligono_visivel, ponto_ant_x, ponto_ant_y, ix, iy);
                    adicionar_aresta_visivel(poligono_visivel, ix, iy, vx, vy);
                } else {
                    // Não havia biombo (estava vendo o infinito/borda), agora vê este vértice
                    // (Lógica depende se vc usa Bounding Box infinito ou não)
                    adicionar_aresta_visivel(poligono_visivel, ponto_ant_x, ponto_ant_y, vx, vy);
                }
                
                // Atualiza estado
                biombo_atual = seg_v;
                ponto_ant_x = vx;
                ponto_ant_y = vy;
            }
        } 
        else {
            
            // Se o segmento que está acabando ERA o biombo
            if (seg_v == biombo_atual) {
                // Remove ele primeiro para descobrir quem está atrás
                tree_remove(ativos, seg_v);
                
                // Quem assume o posto?
                void* proximo_biombo = tree_find_min(ativos);
                
                if (proximo_biombo != NULL) {
                    // O raio passava pelo vértice final (v) e agora bate no próximo biombo lá atrás
                    double ix, iy;
                    obter_ponto_interseccao(bx, by, v, proximo_biombo, &ix, &iy);
                    
                    // Aresta do ponto anterior até o vértice final
                    adicionar_aresta_visivel(poligono_visivel, ponto_ant_x, ponto_ant_y, vx, vy);
                    // Aresta do vértice final até a projeção no biombo de trás
                    adicionar_aresta_visivel(poligono_visivel, vx, vy, ix, iy);
                    
                    ponto_ant_x = ix;
                    ponto_ant_y = iy;
                } else {
                    // Ficou vazio (vê o infinito/borda)
                    adicionar_aresta_visivel(poligono_visivel, ponto_ant_x, ponto_ant_y, vx, vy);
                    ponto_ant_x = vx;
                    ponto_ant_y = vy;
                }
                
                biombo_atual = proximo_biombo;
            } else {
                // Era um segmento que estava escondido atrás do biombo. Só remove.
                tree_remove(ativos, seg_v);
            }
        }
        void* min = tree_find_min(ativos);
        int id_min = min ? get_segmento_id(min) : -1;
        printf("[ANG %.2f] Evento ID %d (%s). Biombo Atual: %d\n", 
               get_vertice_angulo(v) * 180 / M_PI, // Converte para graus para facilitar leitura
               get_segmento_id(seg_v),
               tipo == TIPO_INICIO ? "INICIO" : "FIM",
               id_min);
    }

    // 5. Fechamento do Polígono
    // Conecta o último ponto ao inicial (ou trata o cruzamento do eixo 0 se necessário)
    // adicionar_aresta_visivel(poligono_visivel, ponto_ant_x, ponto_ant_y, ...);

    // 6. Limpeza
    tree_destroy(ativos, NULL);
    destruir_vetor_vertices(eventos, qtd_eventos);

    return poligono_visivel;
}