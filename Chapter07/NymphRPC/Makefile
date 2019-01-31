# Makefile for the NymphRC library.
#
# Allows one to build the library as a .a file, or
# build the test server and client.
#
# (c) 2017 Nyanko.ws

export TOP := $(CURDIR)

GCC = g++
MAKEDIR = mkdir -p
RM = rm
AR = ar

OUTPUT = libnymphrpc.a
INCLUDE = -I src
#-DPOCO_WIN32_UTF8
CFLAGS := $(INCLUDE) -g3 -std=c++11 -U__STRICT_ANSI__ -O1
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(addprefix obj/,$(notdir) $(SOURCES:.cpp=.o))

all: lib test

lib: makedir $(OBJECTS) lib/$(OUTPUT)
	
obj/%.o: %.cpp
	$(GCC) -c -o $@ $< $(CFLAGS)
	
lib/$(OUTPUT): $(OBJECTS)
	-rm -f $@
	$(AR) rcs $@ $^
	
makedir:
	$(MAKEDIR) lib
	$(MAKEDIR) obj
	$(MAKEDIR) obj/src
	
test: test-client test-server
	
test-client:
	$(MAKE) -C ./test/nymph_test_client
	
test-server:
	$(MAKE) -C ./test/nymph_test_server

clean: clean-lib clean-test

clean-test: clean-test-client clean-test-server

clean-lib:
	$(RM) $(OBJECTS)
	
clean-test-client:
	$(MAKE) -C ./test/nymph_test_client clean
	
clean-test-server:
	$(MAKE) -C ./test/nymph_test_server clean
	