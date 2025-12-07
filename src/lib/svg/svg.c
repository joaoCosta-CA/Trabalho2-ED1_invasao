#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>

#include "svg.h"
#include "forma.h"
#include "limites.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "estilo_texto.h"

/*temporário*/
#include "segmento.h"


typedef struct {
    double min_x, min_y;
    double max_x, max_y;
} BoundingBox;

//# prototipo das funções 
static void escrever_circulo(FILE *svg, void *dados);
static void escrever_retangulo(FILE *svg, void *dados);
static void escrever_linha(FILE *svg, void *dados);
static void escrever_texto(FILE *svg, void *dados, const char *family, const char *weight, double size);

void gerar_svg(Lista formas, Lista anteparos, Lista poligono, 
               Lista pontos_bombas, const char *caminho_arquivo) {
               
    FILE *svg = fopen(caminho_arquivo, "w");
    if (!svg) {
        perror("Erro ao criar arquivo SVG");
        return;
    }

    // 1. CÁLCULO DA VIEWBOX (Expandir a Câmera para incluir TUDO)
    double min_x = 0, min_y = 0, max_x = 1000, max_y = 1000;
    int inicializado = 0; 

    // 1.1 Limites das Formas
    if (formas && length(formas) > 0) {
        Limites box = calcular_limites_mundo(formas);
        min_x = get_limites_min_x(box);
        min_y = get_limites_min_y(box);
        max_x = get_limites_max_x(box);
        max_y = get_limites_max_y(box);
        destruir_limites(box);
        inicializado = 1;
    } 

    // 1.2 Expandir com o Polígono de Visibilidade
    if (poligono) {
        Posic p = getFirst(poligono);
        while (p) {
            void *s = get(poligono, p);
            double x1 = get_segmento_x1(s); double y1 = get_segmento_y1(s);
            double x2 = get_segmento_x2(s); double y2 = get_segmento_y2(s);
            
            if (!inicializado) {
                min_x = x1; max_x = x1; min_y = y1; max_y = y1;
                inicializado = 1;
            }
            if (x1 < min_x) min_x = x1; 
            if (x1 > max_x) max_x = x1;
            if (y1 < min_y) min_y = y1;
            if (y1 > max_y) max_y = y1;
            if (x2 < min_x) min_x = x2; 
            if (x2 > max_x) max_x = x2;
            if (y2 < min_y) min_y = y2; 
            if (y2 > max_y) max_y = y2;
            
            p = getNext(poligono, p);
        }
    }

    // 1.3 Expandir com as Bombas
    if (pontos_bombas) {
        Posic p = getFirst(pontos_bombas);
        while (p) {
            double *pt = (double*) get(pontos_bombas, p);
            double bx = pt[0];
            double by = pt[1];

            if (!inicializado) {
                min_x = bx; max_x = bx; min_y = by; max_y = by;
                inicializado = 1;
            }
            if (bx < min_x) min_x = bx;
            if (bx > max_x) max_x = bx;
            if (by < min_y) min_y = by; 
            if (by > max_y) max_y = by;

            p = getNext(pontos_bombas, p);
        }
    }

    // 1.4 Expandir com os ANTEPAROS (para incluir clones!)
    if (anteparos) {
        Posic p = getFirst(anteparos);
        while (p) {
            void *s = get(anteparos, p);
            double x1 = get_segmento_x1(s); double y1 = get_segmento_y1(s);
            double x2 = get_segmento_x2(s); double y2 = get_segmento_y2(s);
            
            if (!inicializado) {
                min_x = x1; max_x = x1; min_y = y1; max_y = y1;
                inicializado = 1;
            }
            if (x1 < min_x) min_x = x1; 
            if (x1 > max_x) max_x = x1;
            if (y1 < min_y) min_y = y1;
            if (y1 > max_y) max_y = y1;
            if (x2 < min_x) min_x = x2; 
            if (x2 > max_x) max_x = x2;
            if (y2 < min_y) min_y = y2; 
            if (y2 > max_y) max_y = y2;
            
            p = getNext(anteparos, p);
        }
    }

    // Margem e Header
    double margem = 50.0;
    double vb_x = min_x - margem;
    double vb_y = min_y - margem;
    double vb_w = (max_x - min_x) + (2 * margem);
    double vb_h = (max_y - min_y) + (2 * margem);

    fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"%.2f %.2f %.2f %.2f\">\n",
            vb_x, vb_y, vb_w, vb_h);

    char font_family[100] = "sans-serif";
    char font_weight[20]  = "normal";
    double font_size      = 12.0;

    // 2. CAMADA 1: FORMAS
    if (formas) {
        Posic p = getFirst(formas);
        while (p) {
            Forma f = get(formas, p);
            void *dados = get_dados_forma(f);
            ShapeType tipo = get_tipo_forma(f);

            switch (tipo) {
                case CIRCULO:   escrever_circulo(svg, dados); break;
                case RETANGULO: escrever_retangulo(svg, dados); break;
                case LINHA:     escrever_linha(svg, dados); break;
                case ESTILO_TEXTO: {
                    const char *fam = get_estilo_familia(dados);
                    const char *w   = get_estilo_peso(dados);
                    if (fam) strcpy(font_family, fam);
                    if (w) {
                        if (strcmp(w, "b") == 0) strcpy(font_weight, "bold");
                        else strcpy(font_weight, "normal");
                    }
                    font_size = get_estilo_tamanho(dados);
                    break;
                }
                case TEXTO:
                    escrever_texto(svg, dados, font_family, font_weight, font_size);
                    break;
            }
            p = getNext(formas, p);
        }
    }

    // 3. CAMADA 2: ANTEPAROS
    if (anteparos) {
        fprintf(svg, "\t\n");
        Posic p = getFirst(anteparos);
        while (p) {
            void *s = get(anteparos, p);
            fprintf(svg, "\t<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\" />\n",
                    get_segmento_x1(s), get_segmento_y1(s), get_segmento_x2(s), get_segmento_y2(s));
            p = getNext(anteparos, p);
        }
    }

    // 4. CAMADA 3: POLÍGONO DE VISIBILIDADE
    if (poligono && length(poligono) > 0) {
        fprintf(svg, "\t\n");

        fprintf(svg, "\t<path d=\"");
        
        Posic p = getFirst(poligono);
        int primeiro = 1;
        while (p) {
            void *s = get(poligono, p);
            double x1 = get_segmento_x1(s);
            double y1 = get_segmento_y1(s);
            double x2 = get_segmento_x2(s);
            double y2 = get_segmento_y2(s);

            if (primeiro) {
                fprintf(svg, "M %.2f %.2f L %.2f %.2f", x1, y1, x2, y2);
                primeiro = 0;
            } else {
                fprintf(svg, " L %.2f %.2f", x2, y2);
            }
            p = getNext(poligono, p);
        }
        // Fecha o caminho (Z) e define cor (sem borda aqui)
        fprintf(svg, " Z\" fill=\"red\" fill-opacity=\"0.3\" stroke=\"none\" />\n");

        p = getFirst(poligono);
        while (p) {
            void *s = get(poligono, p);
            double x1 = get_segmento_x1(s); double y1 = get_segmento_y1(s);
            double x2 = get_segmento_x2(s); double y2 = get_segmento_y2(s);

            // Filtro da Linha Fantasma (Igual ao anterior)
            int eh_linha_fantasma = 0;
            if (pontos_bombas) {
                Posic pb = getFirst(pontos_bombas);
                while (pb) {
                    double *pt = (double*) get(pontos_bombas, pb);
                    double bx = pt[0]; double by = pt[1];

                    if (fabs(y1 - y2) < 0.1 && fabs(y1 - by) < 0.1) {
                        if (x1 > bx + 0.1 || x2 > bx + 0.1) {
                            eh_linha_fantasma = 1;
                            break; 
                        }
                    }
                    pb = getNext(pontos_bombas, pb);
                }
            }
            
            // Só desenha a borda se NÃO for a linha fantasma
            if (!eh_linha_fantasma) {
                fprintf(svg, "\t<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" "
                             "stroke=\"red\" stroke-width=\"1\" stroke-opacity=\"0.4\" stroke-linecap=\"round\" />\n",
                        x1, y1, x2, y2);
            }
            
            p = getNext(poligono, p);
        }
    }

    // 5. CAMADA 4: BOMBAS
    if (pontos_bombas) {
        fprintf(svg, "\t\n");
        Posic p = getFirst(pontos_bombas);
        while (p) {
            double *pt = (double*) get(pontos_bombas, p);
            fprintf(svg, "\t<circle cx=\"%.2f\" cy=\"%.2f\" r=\"5\" fill=\"yellow\" stroke=\"red\" stroke-width=\"2\" />\n", 
                    pt[0], pt[1]);
            p = getNext(pontos_bombas, p);
        }
    }

    fprintf(svg, "</svg>");
    fclose(svg);
}

/* --- Implementações Específicas --- */

static void escrever_circulo(FILE *svg, void *dados) {
    double x = circulo_get_x(dados);
    double y = circulo_get_y(dados);
    double r = circulo_get_raio(dados);
    const char *corb = circulo_get_corb(dados);
    const char *corp = circulo_get_corp(dados);

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

static void escrever_texto(FILE *svg, void *dados, const char *family, const char *weight, double size) {
    double x = texto_get_x(dados);
    double y = texto_get_y(dados);
    char ancora = texto_get_ancora(dados);
    const char* conteudo = texto_get_conteudo(dados);
    const char* corb = texto_get_corb(dados);
    const char* corp = texto_get_corp(dados);

    const char *svg_anchor = "start";
    if (ancora == 'm') svg_anchor = "middle";
    else if (ancora == 'f') svg_anchor = "end";

    fprintf(svg, "\t<text x=\"%.2f\" y=\"%.2f\" fill=\"%s\" stroke=\"%s\" "
                 "text-anchor=\"%s\" font-family=\"%s\" font-weight=\"%s\" font-size=\"%.2f\">%s</text>\n",
            x, y, corp, corb, svg_anchor, family, weight, size, conteudo);
}
