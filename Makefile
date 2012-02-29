CC = gcc
CFLAGS = -g -Wall -Wunused-parameter -Werror
GT_FLAGS = -lgenometools -I$(gt_prefix)/include/genometools -L$(gt_prefix)/lib
GTK_FLAGS = `pkg-config --cflags --libs gtk+-2.0 gthread-2.0` -export-dynamic
SOURCES := $(wildcard src/*.c)
OBJECTS := $(SOURCES:%.c=obj/%.o)

# system specific stuff (concerning 64bit compilation)
MACHINE:=$(shell uname -m)
ifeq ($(64bit),yes)
  ifneq ($(MACHINE),x86_64)
    m64=yes
  endif
else
  ifeq ($(MACHINE),x86_64)
    m32=yes
  endif
endif

ifeq ($(m32),yes)
  CFLAGS += -m32
endif

ifeq ($(m64),yes)
  CFLAGS += -m64
endif

# set prefix for location of genometools
gt_prefix ?= /usr/local

.PREFIXES = .c .o

.PHONY: all clean cleanup dirs

all: dirs bin/ltrsift

dirs: bin obj obj/src

obj/%.o: %.c
	@echo "[compile $@]"
	@$(CC) -c $(CFLAGS) $(GTK_FLAGS) $(GT_FLAGS) $< -o $(@)

bin/ltrsift: $(OBJECTS)
	@echo "[linking $@]"
	@$(CC) $(OBJECTS) -o $@ $(CFLAGS) $(GTK_FLAGS) $(GT_FLAGS)

bin obj obj/src:
	@echo '[create $(@)]'
	@test -d $(@) || mkdir -p $(@)

clean:
	rm -rf obj

cleanup: clean
	rm -rf bin
