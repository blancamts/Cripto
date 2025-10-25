#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bits/getopt_core.h>
#include <math.h>
#include "utils.h"

#define IC_RANDOM 0.0385
#define IC_ENGLISH 0.0650

int main(int argc, char *argv[]) {
    int opt;
    int n = -1; /* Key size */
    char *input_filename = NULL;
    char *output_filename = NULL;
    FILE *output_file = NULL;
    int language = 0; /* 0 for English, 1 for Spanish (DEFAULT: ENGLISH)*/  

    int i;

    /* Parse command line arguments */
    while ((opt = getopt(argc, argv, "n:i:o:l:")) != -1) {
        switch (opt) {
            case 'n':
                n = atoi(optarg);
                break;
            case 'i':
                input_filename = optarg;
                break;
            case 'o':   
                output_filename = optarg;
                break; 
            case 'l':
                language = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-n n] [-l language (0 for english / 1 for spanish)] [-i infile] [-o outfile]\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

   /*Open the input file for reading*/
    char *temp = NULL;
    size_t bytes_read = 0;

    if (input_filename == NULL) {
        /* Leer desde stdin */
        size_t capacity = 1024;
        temp = malloc(capacity);
        if (!temp) {
            perror("malloc");
            return EXIT_FAILURE;
        }
    
        int c;
        while ((c = fgetc(stdin)) != EOF) {
            if (bytes_read + 1 >= capacity) {
                capacity *= 2;
                temp = realloc(temp, capacity);
                if (!temp) {
                    perror("realloc");
                    return EXIT_FAILURE;
                }
            }
            temp[bytes_read++] = (char)c;
        }
        temp[bytes_read] = '\0';
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

        char *temp = malloc(bytes_read + 1);
        if (!temp) {
            perror("malloc");
            fclose(input_file);
            return EXIT_FAILURE;
        }

        if (fread(temp, 1, bytes_read, input_file) != bytes_read) {
            perror("Error reading file");
            fclose(input_file);
            free(temp);
            return EXIT_FAILURE;
        }
        temp[bytes_read] = '\0';
        
        fclose(input_file);
    }

    /* Normalize input to A-Z */
    char *buffer = malloc(bytes_read);

    int purged = normalize_AZ(temp, bytes_read, buffer);
    bytes_read = bytes_read - purged;

    if (n <= 0){
        n = bytes_read; /*Default try all n possible*/
    }

    double best_ic = 0.0;
    int best_ic_idx = 0;

    /*Calculate probable key length*/
    for (i = 1; i < n; i++){
        double ic = calculate_ic(buffer, bytes_read, i);
        printf("IC for n = %d: %f\n", i, ic);

        if (fabs(ic - IC_ENGLISH) < fabs(best_ic - IC_ENGLISH)){
            best_ic = ic;
            best_ic_idx = i;
        }
    }

    /*Calculate probable key*/
    char probable_key[best_ic_idx + 1];
    find_probable_key(buffer, bytes_read, best_ic_idx, probable_key, language);

    /* Open output file */
    output_file = fopen(output_filename, "wb");
    if (!output_file) {
       perror("Error opening output file");
       fclose(output_file);
       return EXIT_FAILURE;
    }

    /*Write probable key to output file*/
    fprintf(output_file, "Probable key (length %d): %s\n", best_ic_idx, probable_key);

    /* Clean up */
    fclose(output_file);
    free(buffer);
    free(temp);

    return EXIT_SUCCESS;

}
