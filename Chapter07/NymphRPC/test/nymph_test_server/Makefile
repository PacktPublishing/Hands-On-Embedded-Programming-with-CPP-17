# Makefile for the NymphRC test server.
#
# (c) 2017 Nyanko.ws

GCC = g++
MAKEDIR = mkdir -p
RM = rm

OUTPUT = nymph_server
INCLUDE = -I $(TOP)/src
LIB := -L ../../lib -lnymphrpc -lPocoNet -lPocoUtil -lPocoFoundation -lPocoJSON
CFLAGS := $(INCLUDE) -std=c++11 -U__STRICT_ANSI__ -g3
SOURCES := $(wildcard *.cpp)
OBJECTS := $(addprefix obj/,$(notdir) $(SOURCES:.cpp=.o))

all: makedir $(OBJECTS) bin/$(OUTPUT)

makedir:
	$(MAKEDIR) obj
	$(MAKEDIR) bin

obj/%.o: %.cpp
	$(GCC) -c -o $@ $< $(CFLAGS)
	
bin/$(OUTPUT):
	$(GCC) -o $@ $(OBJECTS) $(LIB)

clean:
	$(RM) $(OBJECTS)
	