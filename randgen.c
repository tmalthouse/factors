#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("This program needs to be called with 2 arguments: \n \
                \t The quantity of random numbers to generate, \n \
                \t And the output file.\n");
        return 1;
    }

    int quantity = atoi(argv[1]);

    int seed;
    FILE *random = fopen("/dev/random", "r");

    if (!random) {
        printf("Couldn't open /dev/random! Are you running this on windows or something?\n");
        return 1;
    }

    fread(&seed, sizeof(int), 1, random);
    fclose(random);
    
    srand(seed);
    FILE *out = fopen(argv[2], "w");
    if (!out) {
        printf("Couldn't open file %s!\n", argv[2]);
        return 1;
    }

    for (int i=0; i<quantity; i++) {
        fprintf(out, "%d\n", rand());
    }
    fclose(out);
}
