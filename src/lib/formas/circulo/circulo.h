#ifndef CIRCULO_H
#define CIRCULO_H

/**
 * @brief Define um tipo opaco 'Circulo' usando um ponteiro void.
 * 
 * A estrutura interna é mantida oculta no arquivo .c, e os usuários
 * da biblioteca interagem apenas com este ponteiro.
 */

typedef void* Circulo;

/** 
 * @brief função que cria um círculo com os parâmetros fornecidos
 * @param 'id' será o identificador do círculo que será criado.
 * @param 'x' será a coordenada X do centro do círculo.
 * @param 'y' será a coordenada Y do centro do círculo.
 * @param 'r' será o raio do círculo.
 * @param 'corb' será a cor da borda do círculo.
 * @param 'corp' será a cor de preenchimento do círculo.
*/
Circulo CriarCirc(int id, float x, float y, double r, const char* corb, const char* corp);

/** 
 * @brief função que destrói um círculo e libera os recursos associados.
 *
 * @param 'c' será o círculo que será destruído.
 *
 * @pre o parâmetro 'c' deve referenciar um círculo válido.
 * @note a função não retorna erro; o chamador não deve assumir que o círculo foi destruído caso a memória não tenha sido liberada.
 */
void DestruirCirc(Circulo c); 

/**
 * @brief função que obtém o identificador do círculo.
 *
 * @param 'c' será o círculo cujo identificador será retornado.
 * @return retorna o identificador do círculo.
 */
 int circulo_get_id(const Circulo c);

/**
 * @brief função que obtém a coordenada X do centro do círculo.
 *
 * @param 'c' será o círculo cuja coordenada X será retornada.
 * @return retorna a coordenada X (em unidades de posição) do centro do círculo.
 */
 float circulo_get_x(const Circulo c);

/**
 * @brief função que obtém a coordenada Y do centro do círculo.
 *
 * @param 'c' será o círculo cuja coordenada Y será retornada.
 * @return retorna a coordenada Y (em unidades de posição) do centro do círculo.
 */
 float circulo_get_y(const Circulo c);

/**
 * @brief função que obtém o raio do círculo.
 *
 * @param 'c' será o círculo cujo raio será retornado.
 * @return retorna o raio do círculo (em unidades de comprimento).
 */
 double circulo_get_raio(const Circulo c);

/**
 * @brief função que calcula e retorna a área do círculo.
 *
 * @param 'c' será o círculo cuja área será calculada.
 * @return retorna a área do círculo (em unidades de área).
 */
 double circulo_get_area(const Circulo c);

/**
 * @brief função que obtém a cor da borda (stroke) do círculo.
 *
 * @param 'c' será o círculo cuja cor da borda será retornada.
 * @return retorna um ponteiro para uma string terminada em NUL que representa a cor da borda.
 */
const char* circulo_get_corb(const Circulo c);

/**
 * @brief função que obtém a cor de preenchimento (fill) do círculo.
 *
 * @param 'c' será o círculo cuja cor de preenchimento será retornada.
 * @return retorna um ponteiro para uma string terminada em NUL que representa a cor de preenchimento.
 */
 const char* circulo_get_corp(const Circulo c);

/**
 * @brief função que define a coordenada X do círculo.
 *
 * @param 'c' será o círculo cuja coordenada X será modificada.
 * @param 'x' será a nova coordenada X (em unidades dependem do sistema de coordenadas da aplicação).
 *
 * @note o parâmetro 'c' deve referenciar um objeto válido. Se 'c' for NULL, o comportamento é indefinido.
 */
 void circulo_set_x(Circulo c, float x);
 
/**
 * @brief função que define a coordenada Y do círculo.
 *
 * @param 'c' será o círculo cuja coordenada Y será modificada.
 * @param 'y' será a nova coordenada Y (em unidades dependem do sistema de coordenadas da aplicação).
 *
 * @note o parâmetro 'c' deve referenciar um objeto válido. Se 'c' for NULL, o comportamento é indefinido.
 */
 void circulo_set_y(Circulo c, float y);
 
/**
 * @brief função que define a cor de preenchimento do círculo.
 *
 * @param 'c' será o círculo cuja cor de preenchimento será modificada.
 * @param 'corp' será a nova cor de preenchimento (em unidades dependem do sistema de coordenadas da aplicação).
 *
 * @note o parâmetro 'c' deve referenciar um objeto válido. Se 'c' for NULL, o comportamento é indefinido.
 */
 void circulo_set_corp(Circulo c, const char* corp);
 
/**
 * @brief função que define a cor da borda (stroke) do círculo.
 *
 * @param 'c' será o círculo cuja cor da borda será modificada.
 * @param 'corb' será a nova cor da borda (em unidades dependem do sistema de coordenadas da aplicação).
 *
 * @note o parâmetro 'c' deve referenciar um objeto válido. Se 'c' for NULL, o comportamento é indefinido.
 */
 void circulo_set_corb(Circulo c, const char* corb);
#endif