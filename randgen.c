#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 4) {
        printf("This program needs to be called with 3 arguments: \n \
                \t The maximum number to generate, \n \
                \t The quantity of random numbers to generate, \n \
                \t And the output file.\n");
        return 1;
    }
    
    int max = atoi(argv[1]);
    int quantity = atoi(argv[2]);

    int seed;
    FILE *random = fopen("/dev/random", "r");

    if (!random) {
        printf("Couldn't open /dev/random! Are you running this on windows or something?\n");
        return 1;
    }

    fread(&seed, sizeof(int), 1, random);
    fclose(random);
    
    srand(seed);
    FILE *out = fopen(argv[3], "w");
    if (!out) {
        printf("Couldn't open file %s!\n", argv[2]);
        return 1;
    }

    for (int i=0; i<quantity; i++) {
        fprintf(out, "%d\n", rand()%max);
    }
    fclose(out);
}
