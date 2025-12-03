#ifndef VISIBILIDADE_H
#define VISIBILIDADE_H

#include "lista.h"
#include "limites.h" 

/* Tipo Opaco */
typedef void* Vertice;

// Enum para facilitar leitura
typedef enum {
    TIPO_INICIO = 0,
    TIPO_FIM = 1
} TipoVertice;

/*
 * Função principal:
 * Recebe o centro da bomba e a lista de anteparos.
 * Retorna um VETOR DE PONTEIROS (Vertice*) ordenado angularmente.
 * O tamanho do vetor é retornado em *qtd_out.
 */
Vertice* preparar_vertices_ordenados(double centro_x, double centro_y, 
                                     Lista lista_segmentos, int *qtd_out,
                                     char tipo_ord, int cutoff);

/*
 * Calcula a visibilidade.
 * Agora recebe 'box_mundo' para criar suas próprias paredes internas temporárias.
 */
Lista calcular_visibilidade(double x, double y, Lista anteparos, 
                            Limites box_mundo, char tipo_ord, int cutoff);

/* Getters */
double get_vertice_x(Vertice v);
double get_vertice_y(Vertice v);
double get_vertice_angulo(Vertice v);
void* get_vertice_segmento(Vertice v); // Retorna o segmento associado
int get_vertice_tipo(Vertice v);       // Retorna 0 (INICIO) ou 1 (FIM)

/* Destrutor para um único vértice */
void destroy_vertice(Vertice v);

void destruir_vetor_vertices(Vertice *vetor, int qtd);

void set_contexto_bomba(double x, double y);

int comparar_segmentos_arvore(const void* a, const void* b);

#endif