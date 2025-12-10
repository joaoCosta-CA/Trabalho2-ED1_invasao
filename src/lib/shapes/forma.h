/**
 * @file forma.h
 * @brief Módulo de Envelope Genérico para Formas Geométricas
 * 
 * Este módulo implementa um "envelope" (wrapper) genérico que encapsula
 * diferentes tipos de formas geométricas (círculo, retângulo, linha, texto).
 * Permite tratamento uniforme de formas heterogêneas em listas e operações.
 * 
 * @note O envelope armazena o tipo da forma e um ponteiro para os dados
 *       específicos, permitindo polimorfismo em C.
 * @note Suporta operações de clonagem com deslocamento para comandos DSP/CLN.
 * 
 * @author João Costa
 */

#ifndef FORMA_H
#define FORMA_H

/**
 * @brief Enumeração dos tipos de formas geométricas suportadas.
 */
typedef enum {
    CIRCULO,      /**< Círculo definido por centro (x,y) e raio */
    RETANGULO,    /**< Retângulo definido por posição (x,y), largura e altura */
    LINHA,        /**< Linha definida por dois pontos (x1,y1) e (x2,y2) */
    TEXTO,        /**< Texto definido por posição (x,y) e conteúdo */
    ESTILO_TEXTO  /**< Estilo de texto (fonte, tamanho, peso) */
} ShapeType;

/**
 * @brief Tipo opaco para envelope de forma geométrica.
 */
typedef void *Forma;

/**
 * @brief Cria um envelope para uma forma geométrica.
 * @param tipo Tipo da forma (CIRCULO, RETANGULO, etc.)
 * @param dados_figura Ponteiro para os dados específicos da forma
 * @return Ponteiro para o envelope criado
 */
Forma create_forma(ShapeType tipo, void *dados_figura);

/**
 * @brief Obtém o tipo da forma encapsulada.
 * @param f Forma a consultar
 * @return Tipo da forma (enum ShapeType)
 */
ShapeType get_tipo_forma(Forma f);

/**
 * @brief Obtém os dados internos da forma.
 * @param f Forma a consultar
 * @return Ponteiro para os dados específicos (Circulo, Retangulo, etc.)
 */
void* get_dados_forma(Forma f);

/**
 * @brief Libera memória da forma e seus dados internos.
 * @param f Forma a destruir
 */
void destroy_forma(Forma f);

/**
 * @brief Obtém o ID genérico da forma (qualquer tipo).
 * @param f Forma a consultar
 * @return ID da forma encapsulada
 */
int get_forma_id_generico(Forma f);

/**
 * @brief Atualiza a cor de preenchimento da forma.
 * @param f Forma a modificar
 * @param cor Nova cor de preenchimento
 */
void forma_atualizar_cor(Forma f, const char *cor);

/**
 * @brief Clona uma forma com deslocamento.
 * @param f Forma original a clonar
 * @param novo_id ID para a nova forma
 * @param dx Deslocamento em X
 * @param dy Deslocamento em Y
 * @return Nova forma clonada e deslocada
 */
Forma forma_clonar(Forma f, int novo_id, double dx, double dy);

/**
 * @brief Obtém o nome do tipo da forma como string.
 * @param f Forma a consultar
 * @return String com o nome do tipo ("circulo", "retangulo", etc.)
 */
const char* get_tipo_forma_str(Forma f);

#endif