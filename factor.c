#include<stdint.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>

bool sorted_array_contains(uint64_t val, uint64_t *array, uint64_t len)
{
	for (uint64_t i=0; array[i]>val || i>len; i++) {
		if (array[i] == val) {
			return true;
		}
	}
	return false;
}

uint8_t factor(uint64_t num, uint64_t *factors, uint64_t *primes, uint64_t primes_count)
{
	uint8_t factors_found = 0;
	if (sorted_array_contains(num, primes, primes_count)) {
		factors[0] = num;
		return 1;
	}

	for (uint64_t i=0; i<primes_count; i++) {
		if (num%primes[i] == 0) {
            factors[0] = primes[i];
			factors_found++;
			factors_found += factor(num/primes[i], factors+1, primes, primes_count);
			break;
		}
	}
	return factors_found;
}

int main(int argc, char **argv) 
{
	FILE *primes_f;
	primes_f = fopen("primes.cache", "r");
	fseek(primes_f, 0L, SEEK_END);
	size_t sz = ftell(primes_f);
	rewind(primes_f);
	
	uint64_t *primes = calloc(sz, 1);
	uint64_t numprimes = sz/sizeof(uint64_t);

	fread(primes, sizeof(uint64_t), numprimes, primes_f);

	uint64_t factor_num = 43232;
	
	uint64_t factors[64];

	uint8_t fcount = factor(factor_num, factors, primes, numprimes);

	for (int i=0; i<fcount; i++) {
		printf("Factor %d is %llu\n", i, factors[i]);
	}
	return 0;
}

