PROGRAM = bvm
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -ggdb -std=c11
C_FILES := $(wildcard src/*.c)
OBJS := $(patsubst %.c, %.o, $(C_FILES))
TESTE_FILE = teste.bbvm


all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o build/$(PROGRAM)

run: build/$(PROGRAM)
	./build/$(PROGRAM) $(TESTE_FILE)
