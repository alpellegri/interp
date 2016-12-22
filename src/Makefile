CFLAGS = -g -Wall -Wextra
CC = gcc

all: main.exe test_interp.exe

test_interp.exe: test_interp.c prog1.c slp.c util.c interp.c
	$(CC) $(CFLAGS) $^ -o $@

main.exe: main.c slp.c util.c interp.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm test_interp.exe* diy.exe* *.bak

test: main.exe test_interp.exe
	@echo test: diy.exe
	./diy.exe
