TARGET = ted

CC = gcc

CFLAGS = -std=c99 -fstack-protector-all -Wall -Wextra -O0 -g -ggdb

# Diretórios
SRC_DIR = .
LIB_DIR = ./lib

# Bibliotecas matemáticas
LIBS = -lm

SRCS = $(shell find $(SRC_DIR) -name "*.c")

# Cria a lista de objetos (.o) baseada nos nomes dos .c
OBJS = $(SRCS:.c=.o)

INC_DIRS = $(shell find $(SRC_DIR) -type d)
INC_FLAGS = $(addprefix -I,$(INC_DIRS))

# Regra padrão
all: $(TARGET)

# Regra de compilação do executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# Regra de compilação dos arquivos de objeto
# O $< é o arquivo .c e $@ é o arquivo .o
%.o: %.c
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# Limpar arquivos
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean all