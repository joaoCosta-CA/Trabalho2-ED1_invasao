#ifndef ARGUMENTOS_H
#define ARGUMENTOS_H

/**
 * @brief função que obtém o valor associado a uma opção específica dos argumentos da linha de comando.
 *
 * Esta função procura na lista de argumentos fornecida por uma opção que corresponda a `opt_name`
 * e retorna o valor que a segue. Se a opção não for encontrada ou não tiver um valor associado,
 * a função retorna NULL.
 *
 * @param 'argc' será o número de argumentos da linha de comando.
 * @param 'argv' será um vetor de strings contendo os argumentos da linha de comando.
 * @param 'opt_name' será o nome da opção a ser procurada (ex: "--file").
 * @return retorna um ponteiro para a string do valor associado à opção ou NULL caso a opção não seja encontrada ou não tenha um valor associado
 */
char *get_option_value(int argc, char *argv[], char *opt_name);

/**
 * @brief função que obtém o sufixo do comando da linha de comando.
 *
 * Esta função procura na lista de argumentos fornecida por uma opção que corresponda a `opt_name`
 * e retorna o valor que a segue. Se a opção não for encontrada ou não tiver um valor associado,
 * a função retorna NULL.
 *
 * @param 'argc' será o número de argumentos da linha de comando.
 * @param 'argv' será um vetor de strings contendo os argumentos da linha de comando.
 * @return retorna um ponteiro para a string do valor associado à opção ou NULL caso a opção não seja encontrada ou não tenha um valor associado
 */
char *get_command_suffix(int argc, char *argv[]);

/**
 * @brief função que verifica se um diretório existe e é válido.
 *
 * Esta função verifica se o caminho fornecido corresponde a um diretório existente no sistema de arquivos.
 *
 * @param 'caminho' será o caminho do diretório a ser verificado.
 * @return retorna 1 se o diretório existir e for válido, ou 0 caso contrário.
 */
int diretorio_valido(const char *caminho);
#endif