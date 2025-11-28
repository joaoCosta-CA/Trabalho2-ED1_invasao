#include <stdio.h>
#include <float.h>

#include "svg.h"
#include "forma.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"


typedef struct {
    double min_x, min_y;
    double max_x, max_y;
} BoundingBox;


static void escrever_circulo(FILE *svg, void *dados);
static void escrever_retangulo(FILE *svg, void *dados);
static void escrever_linha(FILE *svg, void *dados);
static void escrever_texto(FILE *svg, void *dados);
static BoundingBox calcular_bounding_box(Lista formas);

void gerar_svg(Lista formas, const char *caminho_arquivo) {
    FILE *svg = fopen(caminho_arquivo, "w");
    if (!svg) {
        perror("Erro ao criar arquivo SVG");
        return;
    }

    BoundingBox box = calcular_bounding_box(formas);

    // 2. Adiciona uma margem de segurança
    double margem = 50.0;
    box.min_x -= margem;
    box.min_y -= margem;
    box.max_x += margem;
    box.max_y += margem;

    double largura_total = box.max_x - box.min_x;
    double altura_total = box.max_y - box.min_y;

    /* Cabeçalho padrão do SVG */
    fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"%.2f %.2f %.2f %.2f\">\n",
            box.min_x, box.min_y, largura_total, altura_total);

    if (formas) {
        Posic p = getFirst(formas);
        while (p) {
            Forma f = (Forma) get(formas, p);
            void *dados = get_dados_forma(f);
            ShapeType tipo = get_tipo_forma(f);

            switch (tipo) {
                case CIRCULO:
                    escrever_circulo(svg, dados);
                    break;
                case RETANGULO:
                    escrever_retangulo(svg, dados);
                    break;
                case LINHA:
                    escrever_linha(svg, dados);
                    break;
                case TEXTO:
                    escrever_texto(svg, dados);
                    break;
                default:
                    // Ignora tipos desconhecidos ou ESTILO_TEXTO que não é desenhável diretamente
                    break;
            }
            p = getNext(formas, p);
        }
    }

    /* Rodapé do SVG */
    fprintf(svg, "</svg>");
    fclose(svg);
}

/* --- Implementações Específicas --- */

static void escrever_circulo(FILE *svg, void *dados) {
    /* Recupera os valores usando os getters do módulo circulo */
    double x = circulo_get_x(dados);
    double y = circulo_get_y(dados);
    double r = circulo_get_raio(dados);
    const char *corb = circulo_get_corb(dados); // stroke
    const char *corp = circulo_get_corp(dados); // fill

    fprintf(svg, "\t<circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" stroke=\"%s\" fill=\"%s\" stroke-width=\"1\" />\n",
            x, y, r, corb, corp);
}

static void escrever_retangulo(FILE *svg, void *dados) {
    double x = retangulo_get_x(dados);
    double y = retangulo_get_y(dados);
    double w = retangulo_get_larg(dados);
    double h = retangulo_get_altura(dados);
    const char *corb = retangulo_get_corb(dados);
    const char *corp = retangulo_get_corp(dados);

    // Nota: SVG desenha retângulo a partir do canto superior esquerdo.
    // O PDF [cite: 230] diz que a âncora é canto inferior esquerdo? 
    // SE FOR: você precisa ajustar o Y aqui: y_svg = y_lido - altura.
    // Assumindo comportamento padrão (x,y é topo-esquerda):
    fprintf(svg, "\t<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" stroke=\"%s\" fill=\"%s\" stroke-width=\"1\" />\n",
            x, y, w, h, corb, corp);
}

static void escrever_linha(FILE *svg, void *dados) {
    double x1 = linha_get_x1(dados);
    double y1 = linha_get_y1(dados);
    double x2 = linha_get_x2(dados);
    double y2 = linha_get_y2(dados);
    const char *cor = linha_get_cor(dados);

    fprintf(svg, "\t<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"%s\" stroke-width=\"1\" />\n",
            x1, y1, x2, y2, cor);
}

static void escrever_texto(FILE *svg, void *dados) {
    double x = texto_get_x(dados);
    double y = texto_get_y(dados);
    const char *corb = texto_get_corb(dados); // stroke
    const char *corp = texto_get_corp(dados); // fill
    const char *conteudo = texto_get_conteudo(dados);
    char ancora_char = texto_get_ancora(dados);

    /* Conversão da âncora do projeto ('i', 'm', 'f') para SVG [cite: 251] */
    const char *svg_anchor = "start";
    if (ancora_char == 'm') svg_anchor = "middle";
    if (ancora_char == 'f') svg_anchor = "end";

    // SVG Text usa fill para a cor da letra e stroke para contorno (opcional)
    fprintf(svg, "\t<text x=\"%.2f\" y=\"%.2f\" fill=\"%s\" stroke=\"%s\" text-anchor=\"%s\">%s</text>\n",
            x, y, corp, corb, svg_anchor, conteudo);
}


static BoundingBox calcular_bounding_box(Lista formas) {
    BoundingBox box = {DBL_MAX, DBL_MAX, -DBL_MAX, -DBL_MAX};
    
    // Se lista vazia, retorna padrão 0..1000
    if (!formas || getFirst(formas) == NULL) {
        box.min_x = 0; box.min_y = 0;
        box.max_x = 1000; box.max_y = 1000;
        return box;
    }

    Posic p = getFirst(formas);
    while (p) {
        Forma f = (Forma) get(formas, p);
        void *dados = get_dados_forma(f);
        ShapeType tipo = get_tipo_forma(f);

        double x = 0, y = 0, w = 0, h = 0;

        // Precisamos pegar os extremos (x+w, y+h)
        switch (tipo) {
            case RETANGULO:
                x = retangulo_get_x(dados);
                y = retangulo_get_y(dados);
                w = retangulo_get_larg(dados);
                h = retangulo_get_altura(dados);
                break;
            case CIRCULO:
                // Para círculo, o bounding box é (cx - r) até (cx + r)
                double cx = circulo_get_x(dados);
                double cy = circulo_get_y(dados);
                double r = circulo_get_raio(dados);
                x = cx - r;
                y = cy - r;
                w = 2 * r;
                h = 2 * r;
                break;
            case LINHA:
                // Para linha, precisamos testar os dois pontos
                // Simplificação: pegamos o min e max entre x1, x2, y1, y2
                double x1 = linha_get_x1(dados);
                double y1 = linha_get_y1(dados);
                double x2 = linha_get_x2(dados);
                double y2 = linha_get_y2(dados);
                
                if (x1 < box.min_x) box.min_x = x1;
                if (x2 < box.min_x) box.min_x = x2;
                if (x1 > box.max_x) box.max_x = x1;
                if (x2 > box.max_x) box.max_x = x2;
                
                if (y1 < box.min_y) box.min_y = y1;
                if (y2 < box.min_y) box.min_y = y2;
                if (y1 > box.max_y) box.max_y = y1;
                if (y2 > box.max_y) box.max_y = y2;
                
                // Pula o resto do loop para linha pois já tratamos
                p = getNext(formas, p);
                continue; 
            
            case TEXTO:
                 x = texto_get_x(dados);
                 y = texto_get_y(dados);
                 // Texto é difícil calcular w/h exato sem renderizar, 
                 // assuma um tamanho pequeno ou ignore o w/h
                 w = 10; h = 10; 
                 break;
            default:
                p = getNext(formas, p);
                continue;
        }

        // Atualiza Box Global
        if (x < box.min_x) box.min_x = x;
        if (y < box.min_y) box.min_y = y;
        if ((x + w) > box.max_x) box.max_x = x + w;
        if ((y + h) > box.max_y) box.max_y = y + h;

        p = getNext(formas, p);
    }
    return box;
}