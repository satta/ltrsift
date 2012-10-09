#
# Copyright (c) 2012 Sascha Steinbiss <steinbiss@zbh.uni-hamburg.de>
# Copyright (c) 2012 Sascha Kastens <mail@skastens.de>
# Copyright (c) 2012 Center for Bioinformatics, University of Hamburg
#

ifndef CC
  CC:=gcc
endif
CFLAGS += -g -Wall -Wunused-parameter
GT_FLAGS := -I$(gt_prefix)/include/genometools \
            -I$(gt_prefix)/include/genometools -I$(GTDIR)/src
GT_FLAGS_STATIC := $(GT_FLAGS) `pkg-config --cflags --libs pango pangocairo`
GT_FLAGS += -lgenometools -L$(gt_prefix)/lib $(LDFLAGS)
GTK_FLAGS = `pkg-config --cflags --libs gtk+-2.0 gthread-2.0`
SOURCES := $(wildcard src/*.c)
OBJECTS := $(filter-out obj/src/ltrsift.o obj/src/ltrsift_encode.o, $(SOURCES:%.c=obj/%.o))

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
  STATICBIN := bin/ltrsift_static bin/ltrsift_encode_static
endif

ifeq ($(m32),yes)
  CFLAGS += -m32
endif

ifeq ($(m64),yes)
  CFLAGS += -m64
endif

ifneq ($(errorcheck),no)
  CFLAGS += -Werror
endif

# set prefix for location of genometools
gt_prefix ?= /usr/local

# set prefix for installation
prefix ?= /usr/local

.PREFIXES = .c .o

.PHONY: all clean cleanup dirs install

all: dirs bin/ltrsift bin/ltrsift_encode $(STATICBIN)
	@(test -f bin/ltrsift_encode_static && \
	   cp bin/ltrsift_encode_static sample_data) \
	 || (test -f bin/ltrsift_encode && \
	   cp bin/ltrsift_encode sample_data)
dirs: bin obj obj/src

obj/%.o: %.c
	@echo "[compile $@]"
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(GTK_FLAGS) $(GT_FLAGS) $< -o $(@)

bin/ltrsift: $(OBJECTS) obj/src/ltrsift.o
	@echo "[linking $@]"
	@$(CC) $(OBJECTS) obj/src/ltrsift.o -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(GTK_FLAGS) $(GT_FLAGS)

bin/ltrsift_encode: obj/src/ltrsift_encode.o
	@echo "[linking $@]"
	@$(CC) obj/src/ltrsift_encode.o -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(GT_FLAGS)

bin/ltrsift_static: obj/src/ltrsift.o $(OBJECTS) $(gt_prefix)/lib/libgenometools.a
	@echo "[linking $@]"
	@$(CC) $(OBJECTS) obj/src/ltrsift.o $(gt_prefix)/lib/libgenometools.a \
	  -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(GT_FLAGS_STATIC) $(GTK_FLAGS) -lbz2

bin/ltrsift_encode_static: obj/src/ltrsift_encode.o $(gt_prefix)/lib/libgenometools.a
	@echo "[linking $@]"
	@$(CC) obj/src/ltrsift_encode.o $(gt_prefix)/lib/libgenometools.a \
	   -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) \
	   $(GT_FLAGS_STATIC) -lbz2 -lz -lcairo -lm

bin obj obj/src:
	@echo '[create $(@)]'
	@test -d $(@) || mkdir -p $(@)

clean:
	rm -rf obj

cleanup: clean
	rm -rf bin sample_data/ltrsift_encode

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
	@cp $(CURDIR)/README $(GTDISTDIR)
	@cp $(CURDIR)/run_ltrsift $(GTDISTDIR)
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
	cp bin/* $(prefix)/bin
	test -d $(prefix)/filters  || mkdir -p $(prefix)/filters
	cp -r filters/* $(prefix)/filters
	test -d $(prefix)/sample_data  || mkdir -p $(prefix)/sample_data
	cp -r sample_data/* $(prefix)/sample_data
	test -d $(prefix)/style  || mkdir -p $(prefix)/style
	cp -r style/* $(prefix)/style
