CFLAGS = -g -Wall -Wextra -I./src -I.
CC = gcc

all: main.exe

test_interp.exe: test_interp.c src/prog1.c src/slp.c src/util.c src/interp.c
	$(CC) $(CFLAGS) $^ -o $@

main.exe: main.c src/token.c src/slp.c src/util.c src/parser.c src/interp.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm test_interp.exe* main.exe* *.bak

test: main.exe
	@echo test: main.exe
	./main.exe
