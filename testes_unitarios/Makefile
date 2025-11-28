# ==========================================
# MAKEFILE DE TESTES UNITÁRIOS
# Localização: pasta /testes_unitarios
# ==========================================

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -fstack-protector-all -g
LIBS = -lm


# --- DIRETÓRIOS ---
SRC_ROOT = ../src
LIB_DIR  = $(SRC_ROOT)/lib

# Pastas das Bibliotecas
DIR_LISTA  = $(LIB_DIR)/estruturas_dados/lista
DIR_ARVORE = $(LIB_DIR)/estruturas_dados/arvore_bin
DIR_GEO    = $(LIB_DIR)/leitores/arquivo_geo
DIR_SVG    = $(LIB_DIR)/svg
DIR_FORMAS = $(LIB_DIR)/formasGeo
DIR_SHAPES = $(LIB_DIR)/shapes

# Subpastas das Formas
DIR_CIRC = $(DIR_FORMAS)/circulo
DIR_RET  = $(DIR_FORMAS)/retangulo
DIR_LIN  = $(DIR_FORMAS)/linha
DIR_TXT  = $(DIR_FORMAS)/texto

# Pastas dos Testes (Organização Local)
T_ESTRUT = testes_estruturas
T_LEITOR = testes_leitores
T_SVG    = teste_svg
T_FORMAS = testes_formas

# --- GRUPOS DE DEPENDÊNCIAS ---

# 1. Todas as Formas (Necessário para Geo e SVG)
SRC_ALL_FORMAS = $(DIR_CIRC)/circulo.c \
                 $(DIR_RET)/retangulo.c \
                 $(DIR_LIN)/linha.c \
                 $(DIR_TXT)/texto.c \
                 $(DIR_SHAPES)/forma.c

# 2. Includes de todas as formas (Para o compilador achar os .h)
INC_ALL_FORMAS = -I $(DIR_CIRC) \
                 -I $(DIR_RET) \
                 -I $(DIR_LIN) \
                 -I $(DIR_TXT) \
                 -I $(DIR_FORMAS)\
				 -I $(DIR_SHAPES)

# ==========================================
# REGRAS PRINCIPAIS
# ==========================================

# Roda TUDO
all: shapes structs modules
	@echo "\n>>> TODOS OS TESTES PASSARAM COM SUCESSO! <<<"

# Grupos Específicos
shapes: t_circ t_ret t_lin t_txt
structs: t_lista t_arvore
modules: t_geo t_svg

# ==========================================
# 1. TESTES DE FORMAS BÁSICAS
# ==========================================

t_circ:
	@echo "\n[TESTE] Circulo..."
	@$(CC) $(CFLAGS) -o bin_circ \
		$(T_FORMAS)/teste_circulo/teste_circulo.c \
		$(DIR_CIRC)/circulo.c \
		-I $(DIR_CIRC)
	@./bin_circ
	@rm -f bin_circ

t_ret:
	@echo "\n[TESTE] Retangulo..."
	@$(CC) $(CFLAGS) -o bin_ret \
		$(T_FORMAS)/teste_retangulo/teste_retangulo.c \
		$(DIR_RET)/retangulo.c \
		-I $(DIR_RET)
	@./bin_ret
	@rm -f bin_ret

t_lin:
	@echo "\n[TESTE] Linha..."
	@$(CC) $(CFLAGS) \
		$(T_FORMAS)/teste_linha/teste_linha.c \
		$(DIR_LIN)/linha.c \
		-I $(DIR_LIN) \
		-o bin_lin \
		$(LIBS)
	@./bin_lin
	@rm -f bin_lin

t_txt:
	@echo "\n[TESTE] Texto..."
	@$(CC) $(CFLAGS) \
		$(T_FORMAS)/teste_texto/teste_texto.c \
		$(DIR_TXT)/texto.c \
		-I $(DIR_TXT) \
		 -o bin_txt \
		$(LIBS)
	@./bin_txt
	@rm -f bin_txt

# ==========================================
# 2. TESTES DE ESTRUTURAS DE DADOS
# ==========================================

t_lista:
	@echo "\n[TESTE] Lista..."
	@$(CC) $(CFLAGS) -o bin_lista \
		./testes_dados/teste_lista/teste_lista.c \
		$(DIR_LISTA)/lista.c \
		-I $(DIR_LISTA)
	@./bin_lista
	@rm -f bin_lista

t_arvore:
	@echo "\n[TESTE] Arvore..."
	@$(CC) $(CFLAGS) -o bin_arvore \
		./testes_dados/teste_arvore/teste_arvore.c \
		$(DIR_ARVORE)/arvore.c \
		-I $(DIR_ARVORE)
	@./bin_arvore
	@rm -f bin_arvore

# ==========================================
# 3. TESTES DE MÓDULOS INTEGRADOS (GEO/SVG)
# ==========================================

t_geo:
	@echo "\n[TESTE] Leitor GEO..."
	@$(CC) $(CFLAGS) -o bin_geo \
		./testes_leitores/teste_geo/teste_geo.c \
		$(DIR_GEO)/leitor_geo.c \
		$(DIR_LISTA)/lista.c \
		$(SRC_ALL_FORMAS) \
		-I $(DIR_GEO) \
		-I $(DIR_LISTA) \
		$(LIBS) \
		$(INC_ALL_FORMAS)
	@./bin_geo
	@rm -f bin_geo

t_svg:
	@echo "\n[TESTE] SVG..."
	@$(CC) $(CFLAGS) -o bin_svg \
		$(T_SVG)/teste_svg.c \
		$(DIR_SVG)/svg.c \
		$(DIR_LISTA)/lista.c \
		$(SRC_ALL_FORMAS) \
		-I $(DIR_SVG) \
		-I $(DIR_LISTA) \
		$(LIBS) \
		$(INC_ALL_FORMAS)
	@./bin_svg
	@rm -f bin_svg

# Limpeza
clean:
	rm -f bin_* *.svg