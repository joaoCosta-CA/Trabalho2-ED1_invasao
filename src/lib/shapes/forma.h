#ifndef FORMA_H
#define FORMA_H

typedef enum {
    CIRCULO,
    RETANGULO,
    LINHA,
    TEXTO,
    ESTILO_TEXTO
} ShapeType;

typedef void *Forma;

/* Cria o envelope */
Forma create_forma(ShapeType tipo, void *dados_figura);

/* Getters para recuperar os dados depois */
ShapeType get_tipo_forma(Forma f);
void* get_dados_forma(Forma f);

/* Destruidor */
void destroy_forma(Forma f);

int get_forma_id_generico(Forma f);

void forma_atualizar_cor(Forma f, const char *cor);

Forma forma_clonar(Forma f, int novo_id, double dx, double dy);

#endif