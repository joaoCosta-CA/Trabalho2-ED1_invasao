# ==========================================
# MAKEFILE PRINCIPAL (src/Makefile)
# ==========================================

TARGET = ted
CC = gcc
CFLAGS = -std=c99 -fstack-protector-all -Wall -Wextra -O0 -g -ggdb

SRC_DIR = .
LIB_DIR = ./lib
TEST_DIR = ./testes_unitarios

LIBS = -lm

# ---------------------------------------------------------
# INCLUDES (todas as pastas)
# ---------------------------------------------------------
ALL_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(ALL_DIRS))

# ---------------------------------------------------------
# FONTES DA BIBLIOTECA (exclui main.c e testes)
# ---------------------------------------------------------
LIB_SRCS := $(shell find $(LIB_DIR) -name "*.c")
LIB_OBJS := $(LIB_SRCS:.c=.o)

# ---------------------------------------------------------
# FONTE PRINCIPAL
# ---------------------------------------------------------
MAIN_SRC = ./main.c
MAIN_OBJ = $(MAIN_SRC:.c=.o)

# ---------------------------------------------------------
# FONTES DE TESTE
# ---------------------------------------------------------
TEST_SRCS := $(shell find $(TEST_DIR) -name "*.c")
TEST_OBJS := $(TEST_SRCS:.c=.o)
TEST_BINS := $(TEST_SRCS:.c=)

# ---------------------------------------------------------
# REGRAS DE COMPILAÇÃO
# ---------------------------------------------------------

all: $(TARGET)

# Programa principal
$(TARGET): $(LIB_OBJS) $(MAIN_OBJ)
	@echo "Linkando executavel principal..."
	$(CC) $(CFLAGS) $(LIB_OBJS) $(MAIN_OBJ) -o $(TARGET) $(LIBS)

# Compilar testes como executaveis
tests: $(LIB_OBJS) $(TEST_BINS)
	@echo "Testes compilados com sucesso!"

# Regra para cada executavel de teste
$(TEST_DIR)/%: $(TEST_DIR)/%.o $(LIB_OBJS)
	@echo "Linkando teste $@..."
	$(CC) $(CFLAGS) $(LIB_OBJS) $< -o $@ $(LIBS)

# Rodar todos os testes
run-tests: tests
	@echo ""
	@echo "========================================="
	@echo "       EXECUTANDO TESTES UNITARIOS      "
	@echo "========================================="
	@for test in $(TEST_BINS); do \
		echo ""; \
		echo "--- Rodando $$test ---"; \
		./$$test || exit 1; \
	done
	@echo ""
	@echo "========================================="
	@echo "    TODOS OS TESTES PASSARAM!           "
	@echo "========================================="

# Regra generica .c -> .o
%.o: %.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(LIB_OBJS) $(MAIN_OBJ) $(TARGET)
	rm -f $(TEST_OBJS) $(TEST_BINS)
	rm -f *.txt *.svg

.PHONY: clean all tests run-tests