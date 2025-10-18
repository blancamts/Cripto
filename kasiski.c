#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <bits/getopt_core.h>
#include "utils.h"

#define MAX_TEXT 10000000
#define MAX_NGRAM 10
#define MAX_DIV 300
#define MAX_GCD 1000

int main(int argc, char *argv[]) {
    int opt;
    int ngram = 3;              
    char *input_filename = NULL; 
    char *output_filename = NULL;
    FILE *input_file, *output_file;

    while ((opt = getopt(argc, argv, "l:i:o:")) != -1) {
        switch (opt) {
            case 'l':
                ngram = atoi(optarg);
                break;
            case 'i':
                input_filename = optarg;
                break;
            case 'o':
                output_filename = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l ngram_length] [-i infile] [-o outfile]\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (ngram < 2) {
        fprintf(stderr, "Error: n-gram length must bigger than 2.\n");
        return EXIT_FAILURE;
    }


    if (input_filename == NULL) {
        input_file = stdin;
    } else {
        input_file = fopen(input_filename, "r");
        if (input_file == NULL) {
            perror("Error opening input file");
            return EXIT_FAILURE;
        }
    }

    fseek(input_file, 0, SEEK_END);
    long f_size = ftell(input_file);
    rewind(input_file);
    char *buffer = malloc(f_size + 1);
    if (!buffer) {
        fclose(input_file);
        fprintf(stderr, "Error: cannot allocate %ld bytes.\n", f_size);
        return EXIT_FAILURE;
    }

    size_t bytes_read = fread(buffer, 1, f_size, input_file);
    buffer[bytes_read] = '\0';
    fclose(input_file);

    size_t k = 0;
    char *text = malloc(bytes_read + 1);
    if (!text) {
        free(buffer);
        fprintf(stderr, "Error: cannot allocate %zd bytes.\n", bytes_read + 1);
        return EXIT_FAILURE;
    }

     /* Normalize input to uppercase A-Z */
    for (size_t i = 0; buffer[i] != '\0'; i++) {
        if (isalpha((unsigned char)buffer[i])) {
            text[k++] = toupper((unsigned char)buffer[i]);
        }
    }

    text[k] = '\0';

    /* Open output file */
    if (output_filename == NULL) {
        output_file = stdout;
    } else {
        output_file = fopen(output_filename, "w");
        if (output_file == NULL) {
            perror("Error opening output file");
            return EXIT_FAILURE;
        }
    }

    fprintf(output_file, "====== KASISKI TEST =====\n");
    fprintf(output_file, "n-gram length: %d\n\n", ngram);


    int *distances = malloc(MAX_TEXT * sizeof(int));
    int len = strlen(text);
    char **ngram_keys = malloc(MAX_GCD * sizeof(char *));
    if (!ngram_keys) {
        fprintf(stderr, "Error allocating memory for ngram_keys.\n");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < MAX_GCD; i++) {
        ngram_keys[i] = calloc(MAX_NGRAM + 1, sizeof(char));
        if (!ngram_keys[i]) {
            fprintf(stderr, "Error allocating ngram_keys[%d].\n", i);
            return EXIT_FAILURE;
        }
    }


    int gcd_freq[MAX_DIV] = {0};

    /* Analyze each n-gram found */
    for (size_t i = 0; i < len - ngram; i++) {
        char temp[MAX_NGRAM + 1];
        strncpy(temp, text + i, ngram);
        temp[ngram] = '\0';

        /*Search for all occurrences of the n-gram */
        size_t positions[1000];
        int count = 0;
        positions[count++] = i;

        for (size_t j = i + ngram; j < len - ngram; j++) {
            if (strncmp(temp, text + j, ngram) == 0) {
                positions[count++] = j;
            }
        }

        /* If found at least 2 occurrences, calculate GCD of distances */
        if (count >= 2) {
            /* Compute distances */
            int dists[1000];
            for (int k = 1; k < count; k++) {
                dists[k - 1] = positions[k] - positions[k - 1];
            }

            /*Compute GCD of distances*/
            int g = dists[0];
            for (int k = 1; k < count - 1; k++) {
                g = gcd_aux(g, dists[k]);
            }

            if (g > 1 && g < MAX_DIV) {
                gcd_freq[g]++;
                fprintf(output_file, "%s -> GCD: %d (positions:", temp, g);
                for (int k = 0; k < count; k++) fprintf(output_file, " %zu", positions[k]);
                fprintf(output_file, ")\n");
            }
        }
    }

    fprintf(output_file, "\n===== GCD frequencies =====\n");
    for (int g = 2; g < MAX_DIV; g++) {
        if (gcd_freq[g] > 0)
            fprintf(output_file, "%3d -> %d\n", g, gcd_freq[g]);
    }

    

    fclose(output_file);
    free(buffer);
    free(text);
    free(distances);
    for (int i = 0; i < MAX_GCD; i++) {
        free(ngram_keys[i]);
    }
    free(ngram_keys);
    

    return EXIT_SUCCESS;
}

