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
sdl2_INSTALL_DIR := $(shell pwd)/SDL2
sdl2_CFLAGS = $(shell $(sdl2_INSTALL_DIR)/bin/sdl2-config --cflags)
sdl2_LDFLAGS = $(shell $(sdl2_INSTALL_DIR)/bin/sdl2-config --libs)

.PHONY: all
all:
	@if [ -f "$(sdl2_INSTALL_DIR)/bin/sdl2-config" ]; then \
		$(MAKE) $(exec); \
	else \
		printf "\033[0;31m==> \033[0msdl2 not found, installing...\n"; \
		$(MAKE) build-sdl2; \
		$(MAKE) $(exec); \
	fi

.PHONY: directories
directories:
	@mkdir -p bin build

UNAME_S := $(shell uname -s)
$(exec): directories main.o mylib.a
	$(CC) -o bin/$@ build/main.o $(sdl2_CFLAGS) $(sdl2_LDFLAGS) -lSDL2_image -Lbuild -lmylib
ifeq ($(UNAME_S),Darwin)
	@echo "\033[0;32m==> \033[0mBuilded to \033[1;34mbin/$@"
else
	@echo -e "\033[0;32m==> \033[0mBuilded to \033[1;34mbin/$@"
endif

main.o: main.c
	$(CC) $(CFLAGS) $(sdl2_CFLAGS) -c main.c -o build/$@

mylib.a: $(objects)
	$(AR) rcs build/lib$@ $(addprefix build/,$^)

%.o: %.c
	$(CC) $(CFLAGS) $(sdl2_CFLAGS) -c $< -o build/$@

.PHONY: clean
clean:
	@$(RM) -r bin build

.PHONY: build-sdl2
build-sdl2: directories sdl2 sdl2_image clean-build
	@if [ -f "$(sdl2_INSTALL_DIR)/bin/sdl2-config" ]; then \
		printf "\033[0;32m==> \033[0msdl2 installed successfully!\n"; \
	else \
		printf "\033[0;31m==> \033[0mInstallation error :(\n"; \
		exit 1; \
	fi

.PHONY: sdl2
.ONESHELL:
sdl2_VERSION = 2.32.2
sdl2_TARBALL = libsdl-org-SDL-release-$(sdl2_VERSION).tar.gz
sdl2:
	@echo "Downloading $@-$($@_VERSION) from GitHub..."
	@curl -L https://github.com/libsdl-org/SDL/tarball/release-$($@_VERSION) -o $($@_TARBALL)
	@mkdir -p $@-builder
	@tar xzf $($@_TARBALL) -C $@-builder --strip-components=1
	@$(RM) $($@_TARBALL)
	@mkdir -p $(sdl2_INSTALL_DIR)
	@echo "Building $@-$($@_VERSION)..."
	@mkdir -p $@-builder/build
	@cd $@-builder/build; ../configure --prefix $(sdl2_INSTALL_DIR); $(MAKE) install

.PHONY: sdl2_image
.ONESHELL:
sdl2_image_VERSION = 2.8.8
sdl2_image_TARBALL = libsdl-org-SDL_image-release-$(sdl2_image_VERSION).tar.gz
sdl2_image:
	@echo "Downloading sdl2_image-$($@_VERSION) from GitHub..."
	@curl -L https://github.com/libsdl-org/SDL_image/tarball/release-$($@_VERSION) -o $($@_TARBALL)
	@mkdir -p $@-builder
	@tar xzf $($@_TARBALL) -C $@-builder --strip-components=1
	@$(RM) $($@_TARBALL)
	@mkdir -p $(sdl2_INSTALL_DIR)
	@echo "Building $@-$($@_VERSION)..."
	@mkdir -p $@-builder/build
	@cd $@-builder/build; ../configure --prefix $(sdl2_INSTALL_DIR); $(MAKE) install

.PHONY: clean-build
clean-build:
	@$(RM) -r sdl2-builder sdl2_image-builder

.PHONY: clean-all
clean-all: clean clean-build
	@$(RM) -r SDL2
