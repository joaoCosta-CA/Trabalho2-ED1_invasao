#ifndef RETANGULO_H
#define RETANGULO_H

/**
 * @file retangulo.h
 * @brief módulo de implementação da estrutura de dados Retângulo
 * 
 * este módulo fornece uma implementação completa para manipulação de retângulos geométricos em um espaço bidimensional. Cada retângulo é definido por uma posição (x, y), dimensões (largura e altura), cores de preenchimento e borda, além de possuir um identificador único.
 * 
 * @note a estrutura utiliza encapsulamento através de ponteiro opaco (void*), garantindo abstração completa da implementação interna.
 * @note todas as funções getter são seguras para uso e retornam valores válidos quando recebem uma instância de Retangulo válida.
 */

typedef void* Retangulo;

/**
 * @brief cria um retangulo de área WxH 
 * @param 'id' será um valor int que identifica o retangulo criado
 * @param 'w' será um valor que indica a largura da base do retangulo
 * @param 'h' será um valor que indica a altura que o retangulo tem que ter
 * @param 'corp' será a cor que o retangulo terá no seu interior
 * @param 'corb' será a cor que a borda do retangula deverá ter
 */
Retangulo criarRec(int id, float x, float y, double w, double h, const char* corb, const char* corp);

/**
 * @brief libera a memória alocada para o retangulo
 * @param 'r' será o ponteiro para o retangulo a ser destruído
 */
void destruirRec(Retangulo r);

/**
 * @brief função que retorna o identificador inteiro do retângulo.
 *
 * @param 'r' será a estrutura Retangulo (passada por valor) da qual se deseja obter o id.
 * @return retorna o identificador associado ao retângulo.
 */
int retangulo_get_id(const Retangulo r);

/**
 * @brief função que retorna a coordenada X (posição horizontal) do retângulo.
 *
 * @param 'r' será a estrutura Retangulo (passada por valor) da qual se deseja obter a coordenada X.
 * @return retorna o valor da coordenada X.
 */
float retangulo_get_x(const Retangulo r);

/**
 * @brief função que retorna a coordenada Y (posição vertical) do retângulo.
 *
 * @param 'r' será a estrutura Retangulo (passada por valor) da qual se deseja obter a coordenada Y.
 * @return retorna o valor da coordenada Y.
 */
float retangulo_get_y(const Retangulo r);

/**
 * @brief função que retorna a largura do retângulo.
 *
 * @param 'r' será a estrutura Retangulo (passada por valor) da qual se deseja obter a largura.
 * @return retorna o valor da largura do retângulo.
 */
double retangulo_get_larg(const Retangulo r);

/**
 * @brief função que retorna a altura do retângulo.
 *
 * @param 'r' será a estrutura Retangulo (passada por valor) da qual se deseja obter a altura.
 * @return retorna o valor da altura do retângulo.
 */
double retangulo_get_altura(const Retangulo r);

/**
 * @brief função que retorna a cor de preenchimento (fill) do retângulo como string.
 *
 * @param 'r' será a estrutura Retangulo (passada por valor) da qual se deseja obter a cor de preenchimento.
 * @return retorna um ponteiro para uma string que representa a cor de preenchimento.
 */
const char* retangulo_get_corp(const Retangulo r);

/**
 * @brief função que retorna a cor da borda (stroke) do retângulo como string.
 *
 * @param 'r' será a estrutura Retangulo (passada por valor) da qual se deseja obter a cor da borda.
 * @return retorna um ponteiro para uma string que representa a cor da borda.
 */
const char* retangulo_get_corb(const Retangulo r);

/**
 * @brief função que calcula e retorna a área do retângulo.
 *
 * @param 'r' será a estrutura Retangulo (passada por valor) cujas dimensões serão usadas no cálculo.
 * @return retorna o valor da área calculada (largura * altura).
 */
double retangulo_get_area(const Retangulo r);

/**
 * @brief função que define a coordenada X do retângulo.
 *
 * @param 'r' será a instância do retângulo a ser modificada.
 * @param 'x' será a nova coordenada X (unidades do sistema de coordenadas utilizado).
 */
void retangulo_set_x(Retangulo r, float x);

/**
 * @brief função que define a coordenada Y do retângulo.
 *
 * @param 'r' será a instância do retângulo a ser modificada.
 * @param 'y' será a nova coordenada Y (unidades do sistema de coordenadas utilizado).
 */
void retangulo_set_y(Retangulo r, float y);

/**
 * @brief função que define a cor de preenchimento do retângulo.
 *
 * @param 'r' será a instância do retângulo a ser modificada.
 * @param 'corp' será a string que descreve a cor de preenchimento.
 */
void retangulo_set_corp(Retangulo r, const char* corp);

/**
 * @brief função que define a cor da borda do retângulo.
 *
 * @param 'r' será a instância do retângulo a ser modificada.
 * @param 'corb' será a string que descreve a cor da borda.
 */
void retangulo_set_corb(Retangulo r, const char* corb);

#endif