#include<stdint.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>

int countlines(FILE *fp)
{
	int lines = 0;
    int ch;
    do {
        ch = fgetc(fp);
        if (ch == '\n') {
            lines++;
        }
    } while (ch != EOF);

	rewind(fp);
	return lines;
}

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

	if (num == 0) {
		return 0; //1's prime factorization is defined as being null
	}

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

void fill_num_array(FILE *fp, uint64_t *arr)
{
	uint64_t index = 0;
	while (!feof(fp)) {
		char buf[32]; //Large enough to hold 2^64
		if (fgets(buf, 32, fp)) {
			arr[index] = atoll(buf);
			index++;
		}
	}
	rewind(fp);
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

	if (argc!=2) {
		fprintf(stderr, "This program needs ONE argument—a list of numbers to factorize\n");
		return 1;
	}

	FILE *input = fopen(argv[1], "r");
	if (!input) {
		fprintf(stderr, "Invalid file specified!");
		return 1;
	}

	int numcount = countlines(input);
    printf("Done parsing input file!");

	uint64_t *numbers = calloc(sizeof(uint64_t), numcount);

	fill_num_array(input, numbers);

	FILE *output = freopen(NULL, "w", input);	

	for (uint64_t i=0; i<numcount; i++) {
		uint64_t factors[64];
		uint8_t factor_count = factor(numbers[i], factors, primes, numprimes);
		fprintf(output, "%llu: ", numbers[i]);
		for (uint8_t j=0; j<factor_count; j++) {
			fprintf(output, "%llu ", factors[j]);
		}
		fprintf(output, "\n");
        if (i%1000==0) {
            printf("Finished factoring number %llu\n", i);
        }
	}
	return 0;
}

