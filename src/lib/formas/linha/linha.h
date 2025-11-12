#ifndef LINHA_H
#define LINHA_H

/**
 * @file linha.h
 * @brief Módulo de implementação da estrutura de dados Linha
 * 
 * Este módulo fornece uma implementação completa para manipulação de linhas
 * geométricas em um espaço bidimensional. Cada linha é definida por dois pontos
 * (x1, y1) e (x2, y2) que determinam sua posição e orientação, além de possuir
 * um identificador único e uma cor associada.
 */

typedef void* Linha;

/**
 * @brief cria uma linha com os pontos (x1,y1) e (x2,y2)
 * @param 'id' será um valor int que identifica a linha criada
 * @param 'x1' será um valor que indica a posição horizontal do primeiro ponto da linha
 * @param 'y1' será um valor que indica a posição vertical do primeiro ponto da linha
 * @param 'x2' será um valor que indica a posição horizontal do segundo ponto da linha
 * @param 'y2' será um valor que indica a posição vertical do segundo ponto da linha
 * @param 'corp' será a cor que a linha terá no seu interior
 * @param 'corb' será a cor que a borda da linha deverá ter
 */
Linha criarLinha(int id, float x1, float y1, float x2, float y2, const char* cor);

/**
 * @brief função que libera os recursos associados a uma Linha.
 *
 * @param 'l' será o handle/ponteiro para a Linha a ser destruída. Se 'l' for NULL, a função não realiza nenhuma ação.
 */
 void destruirLinha(Linha l);

/**
 * @brief função que retorna o identificador da linha.
 *
 * @param 'l' será a instância da Linha cujo identificador será obtido. Deve representar uma linha válida.
 * @return retorna o identificador (ID) associado à linha.
 */
int linha_get_id(const Linha l);
 
/**
 * @brief função que obtém a coordenada X do primeiro ponto da linha.
 *
 * @param 'l' será a instância da Linha da qual se deseja a coordenada X1.
 * @return retorna o valor da coordenada X do primeiro ponto.
 */
float linha_get_x1(const Linha l);
 
/**
 * @brief função que obtém a coordenada Y do primeiro ponto da linha.
 *
 * @param 'l' será a instância da Linha da qual se deseja a coordenada Y1.
 * @return retorna o valor da coordenada Y do primeiro ponto.
 */
float linha_get_y1(const Linha l);
 
/**
 * @brief função que obtém a coordenada X do segundo ponto da linha.
 *
 * @param 'l' será a instância da Linha da qual se deseja a coordenada X2.
 * @return retorna o valor da coordenada X do segundo ponto.
 */
float linha_get_x2(const Linha l);
 
/**
 * @brief função que obtém a coordenada Y do segundo ponto da linha.
 *
 * @param 'l' será a instância da Linha da qual se deseja a coordenada Y2.
 * @return retorna o valor da coordenada Y do segundo ponto.
 */
float linha_get_y2(const Linha l);
 
/**
 * @brief função que calcula e retorna a "área" associada à linha.
 *
 * @param 'l' será a instância da Linha para a qual a área será calculada.
 * @return retorna o valor da área associada à linha (conforme definição usada pela aplicação; por exemplo pode ser 0 para uma linha pura ou outra métrica definida pelo domínio).
 */
double linha_get_area(const Linha l);
 
/**
 * @brief função que retorna a cor associada à linha.
 *
 * @param 'l' será a instância da Linha da qual se obtém a cor.
 * @return retorna um ponteiro para uma string imutável contendo a cor (por exemplo nome ou código).
 */
const char* linha_get_cor(const Linha l);

/**
 * @brief função que define a coordenada X do primeiro ponto da linha.
 *
 * @param 'l' será a instância da Linha cujo X1 será atualizado.
 * @param 'x1' será o novo valor da coordenada X1.
 */
void linha_set_x1(Linha l, float x1);

/**
 * @brief função que define a coordenada Y do primeiro ponto da linha.
 *
 * @param 'l' será a instância da Linha cujo Y1 será atualizado.
 * @param 'y1' será o novo valor da coordenada Y1.
 */
void linha_set_y1(Linha l, float y1);

/**
 * @brief função que define a coordenada X do segundo ponto da linha.
 *
 * @param 'l' será a instância da Linha cujo X2 será atualizado.
 * @param 'x2' será o novo valor da coordenada X2.
 */
void linha_set_x2(Linha l, float x2);

/**
 * @brief função que define a coordenada Y do segundo ponto da linha.
 *
 * @param 'l' será a instância da Linha cujo Y2 será atualizado.
 * @param 'y2' será o novo valor da coordenada Y2.
 */
void linha_set_y2(Linha l, float y2);

/**
 * @brief função que define a cor associada à linha.
 *
 * @param 'l' será a instância da Linha cuja cor será atualizada.
 * @param 'cor' será a string representando a cor (por exemplo nome ou código).
 */
void linha_set_cor(Linha l, const char* cor);

#endif