# Makefile para Batalha Naval
# Compilador
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Nome do executável
TARGET = batalha_naval

# Diretórios
SRC_DIR = src
OBJ_DIR = obj

# Arquivos fonte
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/board.c \
       $(SRC_DIR)/fleet.c \
       $(SRC_DIR)/game.c \
       $(SRC_DIR)/io.c \
       $(SRC_DIR)/rnd.c

# Arquivos objeto
OBJS = $(OBJ_DIR)/main.o \
       $(OBJ_DIR)/board.o \
       $(OBJ_DIR)/fleet.o \
       $(OBJ_DIR)/game.o \
       $(OBJ_DIR)/io.o \
       $(OBJ_DIR)/rnd.o

# Regra principal
all: $(TARGET)

# Criar executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilar main.c
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(SRC_DIR)/board.h $(SRC_DIR)/fleet.h $(SRC_DIR)/game.h $(SRC_DIR)/io.h $(SRC_DIR)/rnd.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o

# Compilar board.c
$(OBJ_DIR)/board.o: $(SRC_DIR)/board.c $(SRC_DIR)/board.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/board.c -o $(OBJ_DIR)/board.o

# Compilar fleet.c
$(OBJ_DIR)/fleet.o: $(SRC_DIR)/fleet.c $(SRC_DIR)/fleet.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/fleet.c -o $(OBJ_DIR)/fleet.o

# Compilar game.c
$(OBJ_DIR)/game.o: $(SRC_DIR)/game.c $(SRC_DIR)/game.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/game.c -o $(OBJ_DIR)/game.o

# Compilar io.c
$(OBJ_DIR)/io.o: $(SRC_DIR)/io.c $(SRC_DIR)/io.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/io.c -o $(OBJ_DIR)/io.o

# Compilar rnd.c
$(OBJ_DIR)/rnd.o: $(SRC_DIR)/rnd.c $(SRC_DIR)/rnd.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/rnd.c -o $(OBJ_DIR)/rnd.o

# Criar diretório de objetos
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpar arquivos compilados
clean:
	rm -f $(OBJS) $(TARGET)

# Limpar tudo
distclean: clean
	rm -rf $(OBJ_DIR)

# Regra para rodar
run: $(TARGET)
	./$(TARGET)

# Regra para debug
debug: CFLAGS += -g -O0
debug: clean all

.PHONY: all clean distclean run debug