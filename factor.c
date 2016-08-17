#include<stdint.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>

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



bool sorted_array_contains(mpz_t val, mpz_t *array, uint64_t len)
{
	for (uint64_t i=0; mpz_cmp(array[i], val)>0 || i>len; i++) {
		if (!mpz_cmp(array[i], val)) {
			return true;
		}
	}
	return false;
}

bool isprime(mpz_t val, mpz_t *primes, uint64_t len)
{
    mpz_t max_cached;
    mpz_init_set(max_cached, primes[len-1]);

    if (mpz_cmp(val, max_cached)<=0) {
        if (sorted_array_contains(val, primes, len)) {
            return true;
        } else {
            return false;
        }
    }

    int result = mpz_probab_prime_p(val, 50);

    if (result == 2) {
        return true;
    } else if (result == 0) {
        return false;
    } else {
        // We could implement another, deterministic check here, but this path is only traveled 10e-29% of the time, so...)
        return true;
    }
}

uint8_t factor(mpz_t num, mpz_t *factors, mpz_t *primes, uint64_t primes_count)
{
	uint8_t factors_found = 0;

	if (!mpz_cmp_si(num, 1)) {
		return 0; //1's prime factorization is defined as being null
	}

	if (isprime(num, primes, primes_count)) {
	    mpz_init_set(factors[0], num);
		return 1;
	}

    mpz_t mod, quot;
    mpz_init(mod);
    mpz_init(quot);
	for (uint64_t i=0; i<primes_count; i++) {
        if (({mpz_mod(mod, num, primes[i]); !mpz_cmp(mod, 0);})) {
           mpz_set(factors[0], primes[i]);
           factors_found++;
           factors_found += factor(({mpz_divexact(quot, num, primes[i]); quot;}), 
                                factors+1, primes, primes_count);
           break;
        }
	}
    mpz_clears(mod, quot, NULL);
	return factors_found;
}

void fill_num_array(FILE *fp, mpz_t *arr)
{
	uint64_t index = 0;
	while (!feof(fp)) {
		char buf[128]; //That's big enough to hold any reasonable number—up to 10^127
		if (fgets(buf, 128, fp)) {
            mpz_init_set_str(arr[index], buf, 10);
			index++;
		}
	}
	rewind(fp);
}

int main(int argc, char **argv) 
{
	FILE *primes_f;
	primes_f = fopen("primes.cache", "r");
    if (primes_f == NULL) {
        printf("Couldn't open prime cache file!");
        return 1;
    }
    
	fseek(primes_f, 0L, SEEK_END);
	uint64_t sz = countlines(primes_f);
	
	mpz_t *primes = calloc(sz, sizeof(mpz_t));
	uint64_t numprimes = sz;

    fill_num_array(primes_f, primes);

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
    printf("Done parsing input file!\n");

	mpz_t *numbers = calloc(sizeof(mpz_t), numcount);

	fill_num_array(input, numbers);
    
    fclose(input);
    FILE *output = fopen(argv[1], "w");

    if (output == NULL) {
        printf("Couldn't output to file!\n");
        perror("Error:");
        return 1;
    }

	for (uint64_t i=0; i<numcount; i++) {
		mpz_t factors[64];
		uint8_t factor_count = factor(numbers[i], factors, primes, numprimes);

        char buf[128];
        mpz_get_str(buf, 10, numbers[i]);
		fprintf(output, "%s: ", buf);
		for (uint8_t j=0; j<factor_count; j++) {
            mpz_get_str(buf, 10, factors[j]);
			fprintf(output, "%s ", buf);
		}
		fprintf(output, "\n");
        if (i%1000==0) {
            printf("Finished factoring number %llu\n", i);
        }
	}
	return 0;
}

