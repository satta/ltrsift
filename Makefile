CC = gcc
CFLAGS = -g -Wall -Wunused-parameter -Werror `pkg-config --cflags --libs gtk+-2.0` -export-dynamic -lgenometools
LDFLAGS =
EXECUTABLE = bin/ltrgui
GT_FLAGS = -I/usr/local/include/genometools -L/usr/local/lib
SOURCES := $(wildcard src/*.c)
OBJECTS := $(SOURCES:%.c=obj/%.o)

.PHONY: all clean objects cleanup

all: bin obj obj/src bin/ltrgui.glade $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(CFLAGS) $(GT_FLAGS)

bin obj obj/src:
	@echo '[create $(@)]'
	@test -d $(@) || mkdir -p $(@)

bin/ltrgui.glade:
	@echo '[copy $(@F)]'
	cp src/gui/$(@F) $(@D)

$(OBJECTS): %.o:
	$(CC) -c $(CFLAGS) $(GT_FLAGS) $(@:obj/%.o=%.c) -o $@

clean:
	rm -rf obj

cleanup: clean
	rm -rf bin