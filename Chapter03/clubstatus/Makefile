#

TARGET = clubstatus
SOURCES := $(wildcard *.cpp)

# since POCO 1.4.2 release: PocoSQLite -> PocoDataSQLite, PocoMySQL -> PocoDataMySQL, PocoODBC -> PocoDataODBC
POCO_OLD = $(shell ldconfig -p | grep libPocoSQLite)
ifeq ($(POCO_OLD),)
LIBS += -lPocoDataSQLite
else
LIBS += -lPocoSQLite
endif


ifdef TEST
LDFLAGS := $(LDFLAGS) -lmosquittopp -lmosquitto -lPocoNet -lPocoNetSSL -lPocoUtil -lPocoData $(LIBS) -lPocoFoundation
INCLUDES := -Iwiring
SOURCES += $(wildcard wiring/*.cpp)
else
LDFLAGS := $(LDFLAGS) -lmosquittopp -lmosquitto -lPocoNet -lPocoNetSSL -lPocoUtil -lPocoData $(LIBS) -lPocoFoundation -lwiringPi
endif

CFLAGS := $(CFLAGS) -g3 -std=c++14 -lpthread $(INCLUDES)

CC = g++

all: 
	$(CC) -o $(TARGET) $(SOURCES) $(CFLAGS) $(LDFLAGS)

clean : 
	-rm -f *.o clubstatus

.PHONY: all clean
