#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>

bool isprime (uint64_t num)
{
	uint64_t s = sqrt(num);
	if (num>2 && num%2 == 0) {
		return false;
	}

	for (uint64_t i=3; i<=s; i+=2) {
		if (num%i == 0) {
			return false;
		}
	}
	return true;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "This program MUST be called with exactly one argument—the number of primes to cache.");
		return 1;
	}

	uint64_t max = strtoull(argv[1], NULL, 10);
	uint64_t *primes = calloc(10 + max*.4, sizeof(uint64_t));
	uint64_t primes_index = 0;

	uint64_t i;
	for (i=2; i<max; i++) {
		if (isprime(i)) {
			//printf("%llu is prime\n", i);
			primes[primes_index] = i;
			primes_index++;
		}
	}

	FILE *out;
	out = fopen("primes.cache", "w");

	fwrite(primes, sizeof(uint64_t), primes_index, out);
	fclose(out);
	return 1;
}
