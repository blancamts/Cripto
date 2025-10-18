#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"
#include <bits/getopt_core.h>

int main(int argc, char *argv[]) {
    int opt;
    int cipher = -1;
    char *key = NULL;
    char *input_filename = NULL;
    char *output_filename = NULL;
    FILE *input_file, *output_file;

    while ((opt = getopt(argc, argv, "CDk:i:o:")) != -1) {
        switch (opt) {
            case 'C':
                cipher = 1;
                break;
            case 'D':
                cipher = 0;
                break;
            case 'k':
                key = optarg;
                break;
            case 'i':
                input_filename = optarg;
                break;
            case 'o':
                output_filename = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s -C|-D -k key [-i infile] [-o outfile]\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (cipher == -1 || key == NULL) {
        fprintf(stderr, "Error: Missing or invalid arguments.\n");
        fprintf(stderr, "Usage: %s -C|-D -k key [-i infile] [-o outfile]\n", argv[0]);
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
    char *buffer2 = malloc(bytes_read + 1);

    if (cipher) {
        /*Cipher*/
        vigenere_cipher(buffer, buffer2, bytes_read, key);
    } else {
        /*Decipher*/
        vigenere_decipher(buffer, buffer2, bytes_read, key);
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
