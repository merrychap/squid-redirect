CC     = gcc
CFLAGS = -D_REENTRANT -Wall -pedantic -Isrc
LDLIBS = -lpthread

TARGETS = src/main

all: $(TARGETS)

src/utils.o:    src/utils.c
src/rewriter.o: src/rewriter.c src/utils.c
src/main.o:     src/main.c src/utils.c

src/main: src/main.o src/utils.o src/rewriter.o

.PHONY: clean
clean:
	rm -f $(TARGETS) *~ */*~ */*.o