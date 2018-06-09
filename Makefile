CC     = gcc
CFLAGS = -D_REENTRANT -Wall -pedantic -Isrc
LDLIBS = -lpthread

TARGETS = src/main

all: $(TARGETS)

src/hashtable.o: src/hashtable.c
src/utils.o:     src/utils.c
src/jsmn.o:      src/jsmn.c
src/rewriter.o:  src/rewriter.c src/utils.c
src/main.o:      src/main.c src/utils.c src/rewriter.c

src/main: src/main.o src/utils.o src/rewriter.o src/jsmn.o src/hashtable.o

.PHONY: clean
clean:
	rm -f $(TARGETS) *~ */*~ */*.o