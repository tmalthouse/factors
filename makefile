CFLAGS=-Wpedantic -Wall -O3 -ffast-math -std=gnu11

all: cache factor

cache: createcache.c
	gcc createcache.c $(CFLAGS) -lm -ocache

factor: factor.c
	gcc factor.c $(CFLAGS) -lgmp -ofactor

randgen: randgen.c
	gcc randgen.c $(CFLAGS) -orandgen
