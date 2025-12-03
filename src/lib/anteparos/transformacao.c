#include <string.h>
#include <stdio.h>
#include "lista.h"
#include "forma.h"
#include "segmento.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

/* * Transforma uma forma geométrica em uma lista de segmentos (anteparos).
 * flag_circulo: 'h' ou 'v' (para círculos)
 * id_seg_counter: ponteiro para contador de IDs de segmentos (devem ser únicos)
 */
Lista transformar_em_anteparos(Forma f, char flag_circulo, int *id_seg_counter) {
    Lista segmentos = createList();
    void *dados = get_dados_forma(f);
    ShapeType tipo = get_tipo_forma(f);

    switch(tipo) {
        case CIRCULO: {
            double x = circulo_get_x(dados);
            double y = circulo_get_y(dados);
            double r = circulo_get_raio(dados);
            
            double x1, y1, x2, y2;
            if (flag_circulo == 'v') { // Vertical
                x1 = x2 = x;
                y1 = y - r;
                y2 = y + r;
            } else { // Horizontal (padrão 'h')
                x1 = x - r;
                x2 = x + r;
                y1 = y2 = y;
            }
            insert(segmentos, create_segmento((*id_seg_counter)++, x1, y1, x2, y2));
            break;
        }

        case RETANGULO: {
            double x = retangulo_get_x(dados);
            double y = retangulo_get_y(dados);
            double w = retangulo_get_larg(dados);
            double h = retangulo_get_altura(dados);
            
            insert(segmentos, create_segmento((*id_seg_counter)++, x, y, x + w, y));
            // Direita
            insert(segmentos, create_segmento((*id_seg_counter)++, x + w, y, x + w, y + h));
            // Baixo
            insert(segmentos, create_segmento((*id_seg_counter)++, x + w, y + h, x, y + h));
            // Esquerda
            insert(segmentos, create_segmento((*id_seg_counter)++, x, y + h, x, y));
            break;
}

        case LINHA: {
            //  Linha vira ela mesma
            double x1 = linha_get_x1(dados);
            double y1 = linha_get_y1(dados);
            double x2 = linha_get_x2(dados);
            double y2 = linha_get_y2(dados);
            insert(segmentos, create_segmento((*id_seg_counter)++, x1, y1, x2, y2));
            break;
        }

        case TEXTO: {
            double x = texto_get_x(dados);
            double y = texto_get_y(dados);
            char ancora = texto_get_ancora(dados);
            const char* conteudo = texto_get_conteudo(dados);
            
            double len = strlen(conteudo);
            double comp_linha = 10.0 * len;
            
            double x1, x2;
            
            if (ancora == 'i') {
                x1 = x;
                x2 = x + comp_linha;
            } else if (ancora == 'f') {
                x1 = x - comp_linha;
                x2 = x;
            } else { // Meio ('m')
                x1 = x - comp_linha / 2.0;
                x2 = x + comp_linha / 2.0;
            }
            
            insert(segmentos, create_segmento((*id_seg_counter)++, x1, y, x2, y));
            break;
        }
        case ESTILO_TEXTO: {
            break;
        }
    }
    return segmentos;
}