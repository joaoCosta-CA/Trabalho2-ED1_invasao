# Nome do executável
TARGET = ted

# Nome do aluno
ALUNO = João Henrique

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -std=c99 -fstack-protector-all -Wall -Wextra -O0 -g -ggdb

# Diretório de código-fonte
SRC_DIR = .

# Diretório de cabeçalhos
INC_DIR = .
LIB_DIR = ./lib

# Bibliotecas adicionais
LIBS = -lm

# Lista de arquivos de origem (SRC raiz + libs)
SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(LIB_DIR)/*/*.c)

# Lista de arquivos de objeto
OBJS = $(SRCS:.c=.o)

# Incluir diretórios (SRC raiz + libs)
INC_FLAGS = -I$(INC_DIR) -I$(LIB_DIR)

# Diretórios de bibliotecas
LIB_FLAGS = -L$(LIB_DIR)

# Regra de compilação dos arquivos de objeto
%.o: %.c
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# Regra de compilação do executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB_FLAGS) $(LIBS) -o $(TARGET)

# Limpar arquivos intermediários e o executável
clean:
	rm -f $(SRC_DIR)/*.o $(LIB_DIR)/*/*.o $(TARGET)

# Falso alvo para evitar conflitos com arquivos chamados "clean"
.PHONY: clean

pack: $(TARGET)
	rm -f ../$(ALUNO).zip
	echo $(ALUNO)
	date >> .entrega
	cd ..; zip $(ALUNO).zip -r src/*.c src/*.h src/Makefile LEIA-ME.txt .entrega