#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bits/getopt_core.h>
#include "utils.h"

int main(int argc, char *argv[]) {
    int opt;
    int cipher = -1; /* 1 for cipher, 0 for decipher, -1 for unset (error) */
    char *input_filename = NULL;
    char *output_filename = NULL;
    int mod_raw = -1; /* -1 for unset (error) */
    int a_raw = -1, b_raw = -1; /* coefficients for affine cipher, -1 for unset (error) */
    FILE *output_file;

    mpz_t a, b, mod;
    mpz_inits(a, b, mod, NULL);

    /* Parse command line arguments */
    while ((opt = getopt(argc, argv, "CDm:a:b:i:o:")) != -1) {
        switch (opt) {
            case 'C':
                cipher = 1;
                break;
            case 'D':
                cipher = 0;
                break;
            case 'm':
                mod_raw = atoi(optarg);
                break;
            case 'a':
                a_raw = atoi(optarg);
                break;
            case 'b':
                b_raw = atoi(optarg);
                break;
            case 'i':
                input_filename = optarg;
                break;
            case 'o':
                output_filename = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s -C|-D -m mod -a a -b b -i inputfile -o outputfile\n", argv[0]);
                return EXIT_FAILURE;
        }
    }
    /* Verify the correct arguments were passed in the execution */
    if (cipher == -1 || mod_raw == -1 || a_raw == -1 || b_raw == -1) {
        fprintf(stderr, "Error: Missing or invalid arguments.\n");
        fprintf(stderr, "Usage: %s -C|-D -m mod -a a -b b -i inputfile -o outputfile\n", argv[0]);
        return EXIT_FAILURE;
    }if (mod_raw <= 0) {
        fprintf(stderr, "Error: Invalid values for mod, a, or b.\n");
        return EXIT_FAILURE;
    }

    mpz_set_ui(a, a_raw);
    mpz_set_ui(b, b_raw);
    mpz_set_ui(mod, mod_raw);

    /*Verify if a and mod are coprime*/
    if (is_coprime(a, mod) == 0) {
        fprintf(stderr, "Error: a and mod are not coprime.\n");
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
    
    char * text = malloc(bytes_read + 1);
    int purged = normalize_AZ(buffer, bytes_read, text);
    bytes_read = bytes_read - purged;

    char *buffer2 = malloc(bytes_read + 1);

    /*Choose to cipher or decipher based on user input*/
    if (cipher == 1) {
        affine_cipher(text, buffer2, bytes_read, a, b, mod);
    } else {
        affine_decipher(text, buffer2, bytes_read, a, b, mod);
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
            free(text);
            free(buffer2);
            return EXIT_FAILURE;
        }
    }

    /*Write the ciphered data to the output file*/
    fwrite(buffer2, sizeof(char), bytes_read, output_file);
    fclose(output_file);
    free(buffer);
    free(buffer2);
    free(text);

    return 0;
}