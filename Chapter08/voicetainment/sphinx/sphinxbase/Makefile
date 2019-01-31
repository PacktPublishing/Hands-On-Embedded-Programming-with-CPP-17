# Makefile for SphinxBase.
##
# Author: Maya Posch

export TOP := $(CURDIR)

GPP = g++
GCC = gcc
MAKEDIR = mkdir -p
RM = rm
AR = ar

OUTPUT = libsphinxbase.a
INCLUDE = -I . -I include/
#-DPOCO_WIN32_UTF8
#FLAGS := $(INCLUDE) -g3 -std=c++17 -U__STRICT_ANSI__
#LIB := -lnymphrpc -lPocoNet -lPocoUtil -lPocoFoundation -lPocoJSON
#CPPFLAGS := $(FLAGS)
CFLAGS :=  $(INCLUDE) -g3 -std=c11 -DHAVE_LONG_LONG=1
C_SOURCES := $(wildcard src/libsphinxbase/fe/*.c) \
			$(wildcard src/libsphinxbase/feat/*.c) \
			$(wildcard src/libsphinxbase/lm/*.c) \
			$(wildcard src/libsphinxbase/util/*.c)
C_OBJECTS := $(addprefix obj/,$(notdir) $(C_SOURCES:.c=.o))

all: makedir $(C_OBJECTS) lib/$(OUTPUT)
	
obj/%.o: %.cpp
	$(GPP) -c -o $@ $< $(CPPFLAGS)
	
obj/%.o: %.c
	$(GCC) -c -o $@ $< $(CFLAGS)
	
lib/$(OUTPUT): $(C_OBJECTS)
	-rm -f $@
	$(AR) rcs $@ $^
	
makedir:
	$(MAKEDIR) lib
	#$(MAKEDIR) obj/libsphinxad
	$(MAKEDIR) obj/src/libsphinxbase/fe
	$(MAKEDIR) obj/src/libsphinxbase/feat
	$(MAKEDIR) obj/src/libsphinxbase/lm
	$(MAKEDIR) obj/src/libsphinxbase/util
	# $(MAKEDIR) obj/sphinx_adtools
	# $(MAKEDIR) obj/sphinx_cepview
	# $(MAKEDIR) obj/sphinx_fe
	# $(MAKEDIR) obj/sphinx_jsgf2fsg
	# $(MAKEDIR) obj/sphinx_lmtools

clean:
	$(RM) $(C_OBJECTS)
	