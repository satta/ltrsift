CC = gcc
CFLAGS = -g -Wall -Wunused-parameter -Werror
GT_FLAGS = -I$(gt_prefix)/include/genometools
GT_FLAGS_STATIC := $(GT_FLAGS) -lbz2
GT_FLAGS += -lgenometools -L$(gt_prefix)/lib
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

ifeq ($(static),yes)
  STATICBIN := bin/ltrsift_static
endif

ifeq ($(m32),yes)
  CFLAGS += -m32
endif

ifeq ($(m64),yes)
  CFLAGS += -m64
endif

# set prefix for location of genometools
gt_prefix ?= /usr/local

# set prefix for installation
prefix ?= /usr/local

# set data prefix
data_prefix ?= /usr/share

.PREFIXES = .c .o

.PHONY: all clean cleanup dirs install

all: dirs bin/ltrsift $(STATICBIN)

dirs: bin obj obj/src

obj/%.o: %.c
	@echo "[compile $@]"
	@$(CC) -c $(CFLAGS) $(GTK_FLAGS) $(GT_FLAGS) $< -o $(@)

bin/ltrsift: $(OBJECTS)
	@echo "[linking $@]"
	@$(CC) $(OBJECTS) -o $@ $(CFLAGS) $(GTK_FLAGS) $(GT_FLAGS)

bin/ltrsift_static: $(OBJECTS) $(gt_prefix)/lib/libgenometools.a
	@echo "[linking $@]"
	@$(CC) $(OBJECTS) $(gt_prefix)/lib/libgenometools.a -o $@ $(CFLAGS) $(GTK_FLAGS) $(GT_FLAGS_STATIC)

bin obj obj/src:
	@echo '[create $(@)]'
	@test -d $(@) || mkdir -p $(@)

install: all
	test -d $(prefix)/bin || mkdir -p $(prefix)/bin
	cp bin/ltrsift $(prefix)/bin
	test -d $(data_prefix)/ltrsift || mkdir -p $(data_prefix)/ltrsift
	cp style/default.style $(data_prefix)/ltrsift

clean:
	rm -rf obj

cleanup: clean
	rm -rf bin
