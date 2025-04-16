# Copyright (c) 2025. Max Charrier. All Rights reserved.
# Ce fichier permet de compiler le projet de C en utilisant la ligne de commande.
# Pour l'utiliser, il est nécessaire d'avoir un compilateur C et les outils curl et make d'installé sur votre machine.

# Pour macOS, installer les outils en ligne de commande :
# xcode-select --install

# Pour GNU/Linux, utiliser votre gestionnaire de paquet usuel :
# Debian, Ubuntu : sudo apt install gcc make curl -y
# |- Si vous avez une vieille configuration : sudo apt-get install gcc make curl -y
# Fedora : sudo dnf install gcc make curl -y
# |- Si vous avez une vieille configuration : sudo yum install gcc make curl -y
# Arch Linux : sudo pacman -Syu gcc make curl
# Void Linux : sudo xbps-install -Su gcc make curl
# Gentoo : emerge dev-build/make sys-devel/gcc net-misc/curl

# Pour *BSD : contactez-moi

# Ce programme n'est pas compatible avec la plateforme Windows NT, utiliser les instructions fournis sur UPdago en utilisant Code::Blocks

# Variables à modifier
exec = up_mobilites # Nom de votre exécutable
sources = bus.c list.c sort.c utils.c # Tout vos fichiers de code, execepté main.c (duplication)


##############################################################################
#                              NE PAS MODIFIER                               #
##############################################################################
objects = $(sources:.c=.o)

CC ?= gcc
AR ?= ar
RM ?= rm -f
CFLAGS = -Wall -Wextra -Wno-invalid-utf8 -Wno-invalid-source-encoding -pedantic -std=c99
DEBUG = -ggdb
raylib_CFLAGS = -I/Users/max/Desktop/etu/c/up_mobilites/raylib-5.5/include
raylib_LDFLAGS = -Lraylib-5.5/lib -lraylib

.PHONY: all
all:
	$(MAKE) $(exec)

.PHONY: directories
directories:
	@mkdir -p bin build

UNAME_S := $(shell uname -s)
$(exec): directories main.o mylib.a
	$(CC) -o bin/$@ build/$(word 2,$^) $(raylib_CFLAGS) -Wl,-rpath,@executable_path/../raylib-5.5/lib $(raylib_LDFLAGS) -Lbuild -lmylib
	@echo "\033[0;32m==> \033[0mBuilded to \033[1;34mbin/$@\033[0m"

mylib.a: $(objects)
	$(AR) rcs build/lib$@ $(addprefix build/,$^)

%.o: %.c
	$(CC) $(CFLAGS) $(raylib_CFLAGS) -c $< -o build/$@

.PHONY: clean
clean:
	@$(RM) -r bin build

.PHONY: run
run: all
	@./bin/$(exec)
