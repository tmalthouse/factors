CFLAGS=-Wpedantic -Wall

all: cache factor

cache: createcache.c
	gcc createcache.c $(CFLAGS) -std=c11 -lm -ggdb -ocache

factor: factor.c
	gcc factor.c $(CFLAGS) -std=c11 -ggdb -ofactor

randgen: randgen.c
	gcc randgen.c $(CFLAGS) -std=c11 -orandgen
