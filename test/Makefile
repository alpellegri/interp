CFLAGS = -g -Wall -Wextra
CC = gcc

SRC_DIR := ../src
SRC := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
INCDIR := $(addprefix -I,$(SRC_DIR))

test_interp.exe: test_interp.c prog1.c $(SRC)
	$(CC) $(CFLAGS) $(INCDIR) $^ -o $@

all: test_interp.exe
	@echo test: test_interp.exe
	./test_interp.exe

clean:
	rm test_interp.exe*

test: test_interp.exe
	@echo test: test_interp.exe
	./test_interp.exe
