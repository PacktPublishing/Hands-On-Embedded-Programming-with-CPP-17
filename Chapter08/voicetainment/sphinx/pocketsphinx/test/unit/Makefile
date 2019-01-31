# Makefile for PocketSphinx unit tests.
##
# Author: Maya Posch

export TOP := $(CURDIR)

GPP = g++
GCC = gcc
MAKEDIR = mkdir -p
RM = rm
AR = ar

#OUTPUT = libpocketsphinx.a
INCLUDE = -I . -I ../../include/ -I ../../../sphinxbase/include/ -I ../../src/libpocketsphinx/
#-DPOCO_WIN32_UTF8
#FLAGS := $(INCLUDE) -g3 -std=c++17 -U__STRICT_ANSI__
#LIB := -lnymphrpc -lPocoNet -lPocoUtil -lPocoFoundation -lPocoJSON
LIB := -L ../../lib/ -L ../../../sphinxbase/lib/ -lpocketsphinx -lsphinxbase
#CPPFLAGS := $(FLAGS)
CFLAGS :=  $(INCLUDE) -g3 -std=c11 -DHAVE_LONG_LONG=1 -DDATADIR="\"../../data"\" -DMODELDIR="\"../../../model"\"
C_SOURCES := $(wildcard *.c)
#C_OBJECTS := $(addprefix obj/,$(notdir) $(C_SOURCES:.c=.o))
C_BIN := $(addprefix bin/,$(notdir) $(C_SOURCES:.c=))

all: makedir $(C_BIN)
#$(C_OBJECTS) lib/$(OUTPUT)

bin/%: $(C_SOURCES)
	$(GCC) -o $@ $< $(CFLAGS) $(LIB)
	
obj/%.o: %.cpp
	$(GPP) -c -o $@ $< $(CPPFLAGS)
	
obj/%.o: %.c
	$(GCC) -c -o $@ $< $(CFLAGS)
	
lib/$(OUTPUT): $(C_OBJECTS)
	-rm -f $@
	$(AR) rcs $@ $^
	
makedir:
	$(MAKEDIR) bin
	#$(MAKEDIR) obj/libsphinxad
	#$(MAKEDIR) obj/src/libpocketsphinx
	# $(MAKEDIR) obj/sphinx_adtools
	# $(MAKEDIR) obj/sphinx_cepview
	# $(MAKEDIR) obj/sphinx_fe
	# $(MAKEDIR) obj/sphinx_jsgf2fsg
	# $(MAKEDIR) obj/sphinx_lmtools

clean:
	$(RM) $(C_OBJECTS)
	