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
    char *input_filename = NULL;
    char *output_filename = NULL;
    FILE *input_file = NULL;
    FILE *output_file = NULL;  
    int cipher = -1;
    int m = -1

    int i;

    /* Parse command line arguments */
    while ((opt = getopt(argc, argv, "l:i:o:m:")) != -1) {
        switch (opt) {
            case 'C':
                cipher = 1;
                break;
            case 'D':
                cipher = 0;
                break;
            case 'i':
                input_filename = optarg;
                break;
            case 'o':   
                output_filename = optarg;
                break; 
            case 'm':
                m = atoi(optarg)
                break;
            default:
                fprintf(stderr, "Usage: %s -C|-D -m mod -a a -b b -i inputfile -o outputfile\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (cipher == -1) {
        fprintf(stderr, "Error: Missing or invalid arguments.\n");
        fprintf(stderr, "Usage: %s -C|-D [-i infile] [-o outfile]\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Read input */
    if (input_filename == NULL){
        input_file = stdin;
    }else{
        input_file = fopen(input_filename, "r");
        if (input_file == NULL) {
            perror("Error opening input file");
            return EXIT_FAILURE;
        }
    }

    char *buffer = NULL;
    size_t size = 0;

    /*Read the entire file into memory*/
    ssize_t bytes_read = getline(&buffer, &size, input_file);
    if (bytes_read == -1) {
        perror("Error reading file");
        fclose(input_file);
        return EXIT_FAILURE;
    }
    
    fclose(input_file);

    char * text = malloc(bytes_read + 1);
    int purged = normalize_AZ(buffer, bytes_read, text);
    bytes_read = bytes_read - purged;
    char *buffer2 = malloc(bytes_read + 1);

    if (cipher) {
        if (m == -1){
            /* cifrador bit a bit */
        }else{
            /* cifrador mod m */
        }
        /*Cipher*/
    } else {
        /*Decipher*/
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
    fwrite(buffer2, sizeof(char), bytes_read, output_file);

    
    fclose(output_file);
    free(buffer);
    free(buffer2);

    return EXIT_SUCCESS;

    

    

}