exec = up_mobilites
sources = bus.c list.c sort.c utils.c
objects = $(sources:.c=.o)

CC ?= gcc
AR ?= ar
RM ?= rm -f
CFLAGS = -Wall -Wextra -pedantic -std=c99

LDFLAGS = -Lbuild -lmylib

.PHONY: all
all:
	$(MAKE) $(exec)

.PHONY: directories
directories:
	@mkdir -p bin build

UNAME_S := $(shell uname -s)
$(exec): directories main.o mylib.a
	$(CC) -o bin/$@ build/$(word 2,$^) $(LDFLAGS)
	@echo "\033[0;32m==> \033[0mBuilded to \033[1;34mbin/$@\033[0m"

mylib.a: $(objects)
	$(AR) rcs build/lib$@ $(addprefix build/,$^)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o build/$@

.PHONY: clean
clean:
	@$(RM) -r bin build

.PHONY: run
run: all
	@./bin/$(exec)
