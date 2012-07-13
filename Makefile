#
# Copyright (c) 2012 Sascha Steinbiss <steinbiss@zbh.uni-hamburg.de>
# Copyright (c) 2012 Sascha Kastens <mail@skastens.de>
# Copyright (c) 2012 Center for Bioinformatics, University of Hamburg
#

CC = gcc
CFLAGS = -O3 -g -Wall -Wunused-parameter -Werror
GT_FLAGS = -I$(gt_prefix)/include/genometools -I$(GTDIR)/src
GT_FLAGS_STATIC := $(GT_FLAGS)
GT_FLAGS += -lgenometools -L$(gt_prefix)/lib
GTK_FLAGS = `pkg-config --cflags --libs gtk+-2.0 gthread-2.0`
SOURCES := $(wildcard src/*.c)
OBJECTS := $(SOURCES:%.c=obj/%.o)

# system specific stuff (concerning 64bit compilation)
MACHINE:=$(shell uname -m)
ifeq ($(64bit),yes)
  ifneq ($(MACHINE),x86_64)
    m64=yes
  endif
  BIT:=64bit
else
  ifeq ($(MACHINE),x86_64)
    m32=yes
  endif
  BIT:=32bit
endif

ifneq ($(opt),no)
  CFLAGS += -O3
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

clean:
	rm -rf obj

cleanup: clean
	rm -rf bin

.PHONY: dist srcdist install

SYSTEM:=$(shell uname -s)
MACHINE:=$(shell uname -m)
VERSION:="`cat $(CURDIR)/VERSION`"
SYSTEMNAME:="$(SYSTEM)_$(MACHINE)"
GTDISTBASENAME:="ltrsift-$(VERSION)-$(SYSTEMNAME)-${BIT}"
DISTDIR:="$(CURDIR)/dist/$(SYSTEMNAME)"
SCRIPTSDIR:="$(CURDIR)/scripts"
GTDISTDIR:="$(DISTDIR)/$(GTDISTBASENAME)"

dist: all
	@echo "[build distribution]"
	@rm -rf $(GTDISTDIR)
	@rm -rf $(DISTDIR)/$(GTDISTBASENAME).tar.gz
	@mkdir -p $(GTDISTDIR)/bin
	@cp $(CURDIR)/LICENSE $(GTDISTDIR)
	@cp $(CURDIR)/CHANGELOG $(GTDISTDIR)
	@cp $(CURDIR)/VERSION $(GTDISTDIR)
	@cp $(CURDIR)/bin/ltrsift $(GTDISTDIR)/bin
	@strip $(GTDISTDIR)/bin/ltrsift
	@(test -f bin/ltrsift_static && cp bin/ltrsift_static $(GTDISTDIR)/bin && strip $(GTDISTDIR)/bin/ltrsift_static) || true
	@$(MAKE) prefix=$(GTDISTDIR) install
	@cd $(DISTDIR) && $(SCRIPTSDIR)/tar_root.sh $(GTDISTBASENAME)
	@cd $(DISTDIR) && gzip -f -9 $(GTDISTBASENAME).tar
	@echo "$(DISTDIR)/$(GTDISTBASENAME).tar.gz"

srcdist:
	git archive --format=tar --prefix=ltrsift-`cat VERSION`/ HEAD | \
        gzip -9 > ltrsift-`cat VERSION`-src.tar.gz

install: all
	test -d $(prefix)/bin || mkdir -p $(prefix)/bin
	cp bin/ltrsift $(prefix)/bin
	test -d $(prefix)/filters  || mkdir -p $(prefix)/filters
	cp -r filters/* $(prefix)/filters
	test -d $(prefix)/sample_data  || mkdir -p $(prefix)/sample_data
	cp -r sample_data/* $(prefix)/sample_data

