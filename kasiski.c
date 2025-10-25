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
    FILE *output_file;

    while ((opt = getopt(argc, argv, "n:i:o:")) != -1) {
        switch (opt) {
            case 'n':
                ngram = atoi(optarg);
                break;
            case 'i':
                input_filename = optarg;
                break;
            case 'o':
                output_filename = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-n ngram_length] [-i infile] [-o outfile]\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (ngram < 2) {
        fprintf(stderr, "Error: n-gram length must bigger than 2.\n");
        return EXIT_FAILURE;
    }


    /*Open the input file for reading*/
    char *buffer = NULL;
    size_t bytes_read = 0;

    if (input_filename == NULL) {
        /* Leer desde stdin */
        size_t capacity = 1024;
        buffer = malloc(capacity);
        if (!buffer) {
            perror("malloc");
            return EXIT_FAILURE;
        }
    
        int c;
        while ((c = fgetc(stdin)) != EOF) {
            if (bytes_read + 1 >= capacity) {
                capacity *= 2;
                buffer = realloc(buffer, capacity);
                if (!buffer) {
                    perror("realloc");
                    return EXIT_FAILURE;
                }
            }
            buffer[bytes_read++] = (char)c;
        }
        buffer[bytes_read] = '\0';
    } else {
        /* Leer desde archivo */
        FILE *input_file = fopen(input_filename, "rb");
        if (input_file == NULL) {
            perror("Error opening input file");
            return EXIT_FAILURE;
        }

        fseek(input_file, 0, SEEK_END);
        long bytes_read = ftell(input_file);
        rewind(input_file);

        char *buffer = malloc(bytes_read + 1);
        if (!buffer) {
            perror("malloc");
            fclose(input_file);
            return EXIT_FAILURE;
        }

        if (fread(buffer, 1, bytes_read, input_file) != bytes_read) {
            perror("Error reading file");
            fclose(input_file);
            free(buffer);
            return EXIT_FAILURE;
        }
        buffer[bytes_read] = '\0';
        
        fclose(input_file);
    }

    char *text = malloc(bytes_read + 1);
    int purged = normalize_AZ(buffer, bytes_read, text);
    bytes_read = bytes_read - purged;

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

