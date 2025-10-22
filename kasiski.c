#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <bits/getopt_core.h>
#include "utils.h"

#define MAX_TEXT 10000000
#define MAX_NGRAM 10
#define MAX_DIV 400
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

    char *text = malloc(bytes_read + 1);
    if (!text) {
        free(buffer);
        fprintf(stderr, "Error: cannot allocate %zd bytes.\n", bytes_read + 1);
        return EXIT_FAILURE;
    }

    /* Normalize input to uppercase A-Z */
    normalize_AZ(buffer, bytes_read, text);

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


    int flag = 0;

    for (size_t i = 0; i + ngram <= len && !flag; i++) {
        /* Take n-gram starting at position i */
        char temp[MAX_NGRAM + 1];
        strncpy(temp, text + i, ngram);
        temp[ngram] = '\0';

        /* Search for further occurrences */
        size_t positions[2000];
        int count = 0;

        /* Save first occurrence */
        positions[count++] = i;

        /* Look for further occurrences */
        for (size_t j = i + 1; j + ngram <= len; j++) {
            if (strncmp(temp, text + j, ngram) == 0) {
                positions[count++] = j;
            }
        }

        if (count >= 2) {
            /* Calculate distances and their GCD */
            int g = 0;
            for (int k = 1; k < count; k++) {
                int dist = (int)(positions[k] - positions[k - 1]);
                if (k == 1) g = dist;
                else g = gcd_aux(g, dist);
            }

            if (g > 1) {
                /* Output results */
                fprintf(output_file, "N-gram: %s\n", temp);
                fprintf(output_file, "Occurrences (%d):", count);
                for (int k = 0; k < count; k++) {
                    fprintf(output_file, " %zu", positions[k]);
                }
                fprintf(output_file, "\n");
                fprintf(output_file, "GCD of distances = %d -> possible key length\n\n", g);
                flag = 1;
                break;
            }
        }
    }
    

    if (!flag) {
        fprintf(output_file, "No repeated n-grams found for any tested size (>=2 and <= %d).\n", ngram);
    }

    fclose(output_file);
    free(buffer);
    free(text);
    free(distances);
    

    return EXIT_SUCCESS;
}

