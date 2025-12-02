# =========================================================
# MAKEFILE AUTOMÁTICO (FIND RECURSIVO) - VERSÃO COMPLETA
# Localização: pasta /testes_unitarios
# =========================================================

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -fstack-protector-all -g
LIBS = -lm

# --- 1. DEFINIÇÃO DA RAIZ ---
# Onde está o código fonte real?
SRC_LIB = ../src/lib

# --- 2. A MÁGICA DOS INCLUDES (AUTOMÁTICO) ---
# O comando 'find' lista TODAS as subpastas dentro de ../src/lib
# A função 'addprefix' coloca um "-I" na frente de cada uma.
# Isso garante que qualquer #include "arquivo.h" funcione.
ALL_DIRS  := $(shell find $(SRC_LIB) -type d)
INC_FLAGS := $(addprefix -I,$(ALL_DIRS))

# --- 3. A MÁGICA DOS FONTES (.c) (AUTOMÁTICO) ---
# Encontra TODOS os arquivos .c dentro de ../src/lib
# Isso inclui: lista, arvore, circulo, svg, visibilidade, sort, etc.
# Compilamos tudo junto para garantir que não faltem dependências.
ALL_LIB_SRCS := $(shell find $(SRC_LIB) -name "*.c")

# --- 4. DIRETÓRIOS DOS TESTES (Para organizar as regras abaixo) ---
T_ESTRUT = testes_dados
T_LEITOR = testes_leitores
T_SVG    = teste_svg
T_FORMAS = testes_formas
T_ALGO   = testes_algoritmos
T_ORD    = teste_ordenadores

# =========================================================
# REGRAS PRINCIPAIS
# =========================================================

# Roda TUDO
all: shapes structs algos modules integration
	@echo "\n>>> TODOS OS TESTES PASSARAM COM SUCESSO! <<<"

# Grupos de Testes
shapes: t_circ t_ret t_lin t_txt
structs: t_lista t_arvore
algos: t_sort t_visib
modules: t_leitor t_geo t_transf t_svg
integration: t_qry

# ==========================================
# 1. TESTES DE FORMAS BÁSICAS
# ==========================================

t_circ:
	@echo "\n[AUTO] Teste Circulo..."
	@$(CC) $(CFLAGS) -o bin_circ \
		$(T_FORMAS)/teste_circulo/teste_circulo.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_circ
	@rm -f bin_circ

t_ret:
	@echo "\n[AUTO] Teste Retangulo..."
	@$(CC) $(CFLAGS) -o bin_ret \
		$(T_FORMAS)/teste_retangulo/teste_retangulo.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_ret
	@rm -f bin_ret

t_lin:
	@echo "\n[AUTO] Teste Linha..."
	@$(CC) $(CFLAGS) -o bin_lin \
		$(T_FORMAS)/teste_linha/teste_linha.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_lin
	@rm -f bin_lin

t_txt:
	@echo "\n[AUTO] Teste Texto..."
	@$(CC) $(CFLAGS) -o bin_txt \
		$(T_FORMAS)/teste_texto/teste_texto.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_txt
	@rm -f bin_txt

# ==========================================
# 2. TESTES DE ESTRUTURAS DE DADOS
# ==========================================

t_lista:
	@echo "\n[AUTO] Teste Lista..."
	@$(CC) $(CFLAGS) -o bin_lista \
		$(T_ESTRUT)/teste_lista/teste_lista.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_lista
	@rm -f bin_lista

t_arvore:
	@echo "\n[AUTO] Teste Arvore..."
	@$(CC) $(CFLAGS) -o bin_arvore \
		$(T_ESTRUT)/teste_arvore/teste_arvore.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_arvore
	@rm -f bin_arvore

# ==========================================
# 3. TESTES DE ALGORITMOS (ORDENAÇÃO E VISIBILIDADE)
# ==========================================

t_sort: t_insert t_merge t_quick

t_insert:
	@echo "\n[AUTO] Teste InsertionSort..."
	@$(CC) $(CFLAGS) -o bin_insert \
		$(T_ORD)/teste_insertion/teste_insertsort.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_insert
	@rm -f bin_insert

t_merge:
	@echo "\n[AUTO] Teste MergeSort..."
	@$(CC) $(CFLAGS) -o bin_merge \
		$(T_ORD)/teste_murgesort/teste_msort.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_merge
	@rm -f bin_merge

t_quick:
	@echo "\n[AUTO] Teste QuickSort..."
	@$(CC) $(CFLAGS) -o bin_quick \
		$(T_ORD)/teste_quicksort/teste_qsort.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_quick
	@rm -f bin_quick

t_visib:
	@echo "\n[AUTO] Teste Visibilidade..."
	@$(CC) $(CFLAGS) -o bin_visib \
		$(T_ALGO)/teste_visibilidade/teste_visibilidade.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_visib
	@rm -f bin_visib

# ==========================================
# 4. TESTES DE LEITURA, TRANSFORMAÇÃO E SVG
# ==========================================

t_leitor:
	@echo "\n[AUTO] Teste Leitor Genérico..."
	@$(CC) $(CFLAGS) -o bin_leitor \
		$(T_LEITOR)/teste_leitorArq/teste_leitorArq.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_leitor
	@rm -f bin_leitor

t_geo:
	@echo "\n[AUTO] Teste Leitor GEO..."
	@$(CC) $(CFLAGS) -o bin_geo \
		$(T_LEITOR)/teste_geo/teste_geo.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_geo
	@rm -f bin_geo

t_transf:
	@echo "\n[AUTO] Teste Transformação em Anteparos..."
	@$(CC) $(CFLAGS) -o bin_transf \
		./teste_anteparos/teste_anteparos.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_transf
	@rm -f bin_transf

t_svg:
	@echo "\n[AUTO] Teste SVG Writer..."
	@$(CC) $(CFLAGS) -o bin_svg \
		$(T_SVG)/teste_svg.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_svg
	@rm -f bin_svg

# ==========================================
# 5. TESTE DE INTEGRAÇÃO (QRY)
# ==========================================

t_qry:
	@echo "\n[AUTO] Teste Leitor QRY (Sistema Completo)..."
	@$(CC) $(CFLAGS) -o bin_qry \
		$(T_LEITOR)/teste_qry/teste_qry.c \
		$(ALL_LIB_SRCS) $(INC_FLAGS) $(LIBS)
	@./bin_qry
	@rm -f bin_qry

# Limpeza
clean:
	rm -f bin_* t_* *.svg *.txt