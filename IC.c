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

    int i;

    /* Parse command line arguments */
    while ((opt = getopt(argc, argv, "n:i:o:")) != -1) {
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
            default:
                fprintf(stderr, "Usage: %s [-n n] [-i infile] [-o outfile]\n", argv[0]);
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
        bytes_read = ftell(input_file);
        rewind(input_file);

        temp = malloc(bytes_read + 1);
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

    for (i = 1; i < n; i++){
        double ic = calculate_ic(buffer, bytes_read, i);

        if (fabs(ic - IC_ENGLISH) < fabs(best_ic - IC_ENGLISH)){
            best_ic = ic;
            best_ic_idx = i;
        }
    }

    /*Open the output file for writing*/
    if (output_filename == NULL){
        output_file = stdout;
    }
    else{
        output_file = fopen(output_filename, "w");   
        if (output_file == NULL) {
            perror("Error opening output file");
            free(buffer);
            return EXIT_FAILURE;
        }
    }

    fprintf(output_file, "====== INDEX OF COINCIDENCE TEST =====\n");
    fprintf(output_file, "Best IC found for n = %d: %f\n", best_ic_idx, best_ic);
    fprintf(output_file, "Diffetence to English IC: %f\n", fabs(best_ic - IC_ENGLISH));
    fprintf(output_file, "\n");
    fprintf(output_file, "Expected IC for random text: %f\n", IC_RANDOM);
    fprintf(output_file, "Expected IC for English text: %f\n", IC_ENGLISH);
    fclose(output_file);
    free(buffer);

    return EXIT_SUCCESS;
}





