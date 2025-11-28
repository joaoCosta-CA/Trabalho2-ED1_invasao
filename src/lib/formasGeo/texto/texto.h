#ifndef TEXTO_H
#define TEXTO_H

/**
 * @brief define um tipo opaco 'Texto' usando um ponteiro void.
 */
typedef void *Texto;

/**
 * @brief função que cria uma nova instância de um objeto de texto. 
 *
 * @param 'id' será um identificador numérico para o texto.
 * @param 'x' será a coordenada X da âncora do texto.
 * @param 'y' será a coordenada Y da âncora do texto.
 * @param 'corb' será a string que representa a cor da borda do texto (ex: "#00FF00").
 * @param 'corp' será a string que representa a cor de preenchimento do texto (ex: "#00FF00").
 * @param 'ancora' será o caractere que representa a âncora do texto.
 * @param 'conteudo' será a string de conteúdo a ser exibida.
 * @return retorna um ponteiro opaco 'Texto' (void*) para o objeto criado, ou NULL se a alocação de memória falhar.
 */
Texto criar_texto(int id, float x, float y, const char* corb, const char* corp, char ancora, const char* conteudo);

/**
 * @brief função que calcula e retorna a "área" ocupada pelo texto.
 * A área é definida como: 20.0 * (número de caracteres no conteúdo).
 *
 * @param 't' será o ponteiro opaco 'Texto' para o qual a área será calculada.
 * @return retorna o valor da área calculada como um float. Retorna 0.0 se o ponteiro for nulo.
 */
float area_texto(const Texto t);

/**
 * @brief função que libera toda a memória alocada para um objeto de texto.
 *
 * @param 't' será o ponteiro opaco 'Texto' a ser liberado.
 */
void destruirTexto(Texto t);

/**
 * @brief função que retorna o identificador do objeto Texto.
 *
 * @param 't' será o Texto cujo identificador será obtido. Se 't' for inválido (por exemplo, NULL), o comportamento é indefinido.
 * @return retorna o identificador do Texto.
 */
int texto_get_id(const Texto t);
 
/**
 * @brief Retorna a coordenada X do objeto Texto.
 *
 * @param 't' será o Texto a partir do qual a coordenada X será obtida. Se 't' for inválido, o comportamento é indefinido.
 * @return retorna o valor da coordenada X.
 */
float texto_get_x(const Texto t);
 
/**
 * @brief função que retorna a coordenada Y do objeto Texto.
 *
 * @param 't' será o Texto a partir do qual a coordenada Y será obtida. Se 't' for inválido, o comportamento é indefinido.
 * @return retorna o valor da coordenada Y.
 */
float texto_get_y(const Texto t);
 
/**
 * @brief função que calcula ou retorna a área ocupada pelo Texto.
 *
 * @param 't' será o Texto cujo valor de área será obtido. Se 't' for inválido, o comportamento é indefinido.
 * @return retorna o valor da área ocupada pelo Texto.
 */
double texto_get_area(const Texto t);
 
/**
 * @brief função que retorna a âncora (alinhamento/posicionamento) do Texto.
 *
 * @param 't' será o Texto cujo atributo de âncora será obtido. Se 't' for inválido, o comportamento é indefinido.
 * @return retorna o caractere representando a âncora do Texto.
 */
char texto_get_ancora(const Texto t);
 
/**
 * @brief função que retorna a cor da borda do Texto.
 *
 * @param 't' será o Texto cujo atributo de cor de borda será obtido. Se 't' for inválido, o comportamento é indefinido.
 * @return retorna um ponteiro para string constante contendo a cor da borda. Pode ser NULL se não definida.
 */
const char *texto_get_corb(const Texto t);
 
/**
 * @brief função que retorna a cor de preenchimento do Texto.
 *
 * @param 't' será o Texto cujo atributo de cor de preenchimento será obtido. Se 't' for inválido, o comportamento é indefinido.
 * @return retorna um ponteiro para string constante contendo a cor de preenchimento. Pode ser NULL se não definida.
 */
const char *texto_get_corp(const Texto t);
 
/**
 * @brief função que retorna o conteúdo textual do objeto Texto.
 *
 * @param 't' será o Texto cujo conteúdo será retornado. Se 't' for inválido, o comportamento é indefinido.
 * @return retorna um ponteiro para string constante com o conteúdo do Texto. Pode ser NULL se vazio.
 */
const char *texto_get_conteudo(const Texto t);

/**
 * @brief função que define a coordenada X do objeto Texto.
 *
 * @param 't' será a instância do Texto a ser modificada. Deve ser não NULL.
 * @param 'x' será a nova posição no eixo X (unidades dependem do contexto da aplicação).
 */
void texto_set_x(Texto t, float x);
 
/**
 * @brief função que define a coordenada Y do objeto Texto.
 *
 * @param 't' será a instância do Texto a ser modificada. Deve ser não NULL.
 * @param 'y' será a nova posição no eixo Y (unidades dependem do contexto da aplicação).
 */
void texto_set_y(Texto t, float y);
 
/**
 * @brief função que define a cor de preenchimento (cor principal) do Texto.
 *
 * @param 't' será a instância do Texto a ser modificada. Deve ser não NULL.
 * @param 'corp' será a string representando a cor de preenchimento. Deve ser não NULL.
 */
void texto_set_corp(Texto t, const char* corp);
 
/**
 * @brief função que define a cor da borda do Texto.
 *
 * @param 't' será a instância do Texto a ser modificada. Deve ser não NULL.
 * @param 'corb' será a string representando a cor da borda. Deve ser não NULL.
 */
void texto_set_corb(Texto t, const char* corb);
 
/**
 * @brief função que define o conteúdo textual do objeto Texto.
 *
 * @param 't' será a instância do Texto a ser modificada. Deve ser não NULL.
 * @param 'conteudo' será a string com o novo conteúdo textual. Deve ser não NULL.
 */
void texto_set_conteudo(Texto t, const char* conteudo);

#endif