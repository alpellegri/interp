CFLAGS = -g -Wall -Wextra
CC = gcc

SRC_DIR := ../src
SRC := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
INCDIR := $(addprefix -I,$(SRC_DIR))

main.exe: main.c $(SRC)
	$(CC) $(CFLAGS) $(INCDIR) $^ -o $@

all: main.exe
	@echo test: main.exe
	./main.exe

clean:
	rm main.exe*

test: main.exe
	@echo test: main.exe
	./main.exe
