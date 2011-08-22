CC = gcc
CFLAGS = -g -Wall -Wunused-parameter -Werror -m64
GT_FLAGS = -lgenometools -I$(gt_prefix)/include/genometools -L$(gt_prefix)/lib
GTK_FLAGS = `pkg-config --cflags --libs gtk+-2.0` -export-dynamic
SOURCES := $(wildcard src/*.c)
OBJECTS := $(SOURCES:%.c=obj/%.o)

# set prefix for location of genometools
gt_prefix ?= /usr/local

.PREFIXES = .c .o

.PHONY: all clean cleanup dirs

all: dirs bin/ltrgui

dirs: bin obj obj/src bin/ltrgui.glade

obj/%.o: %.c
	$(CC) -c $(CFLAGS) $(GTK_FLAGS) $(GT_FLAGS) $< -o $(@)

bin/ltrgui: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS) $(GTK_FLAGS) $(GT_FLAGS)

bin obj obj/src:
	@echo '[create $(@)]'
	@test -d $(@) || mkdir -p $(@)

bin/ltrgui.glade: src/gui/ltrgui.glade
	@echo '[copy $(@F)]'
	@cp -u src/gui/$(@F) $(@D)

clean:
	rm -rf obj

cleanup: clean
	rm -rf bin
