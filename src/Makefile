# ==========================================
# MAKEFILE PRINCIPAL (src/Makefile)
# ==========================================

TARGET = ted
CC = gcc
# Flags base (Note que não listamos os -I aqui manualmente ainda)
CFLAGS = -std=c99 -fstack-protector-all -Wall -Wextra -O0 -g -ggdb

# Diretórios Raiz
SRC_DIR = .
LIB_DIR = ./lib

# Biblioteca Matemática
LIBS = -lm

# ---------------------------------------------------------
# INCLUDES
# ---------------------------------------------------------
# 1. Encontra TODAS as subpastas dentro de src e lib
ALL_DIRS := $(shell find $(SRC_DIR) -type d)
# 2. Adiciona o prefixo -I para cada diretório encontrado
INC_FLAGS := $(addprefix -I,$(ALL_DIRS))

# ---------------------------------------------------------
# FONTES E OBJETOS
# ---------------------------------------------------------
# Encontra todos os arquivos .c
SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(SRCS:.c=.o)

# ---------------------------------------------------------
# REGRAS DE COMPILAÇÃO
# ---------------------------------------------------------

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linkando executável..."
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# Regra para transformar .c em .o
%.o: %.c
	@echo "Compilando $<..."
	# AQUI ESTÁ O TRUQUE: $(INC_FLAGS) diz ao GCC onde procurar os .h
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean all