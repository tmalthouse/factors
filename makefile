cache: createcache.c
	gcc createcache.c -std=c11 -lm -ggdb -ocache

factor: factor.c
	gcc factor.c -std=c11 -ggdb -ocache
