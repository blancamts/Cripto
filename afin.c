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
    FILE *input_file, *output_file;

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

    /*Choose to cipher or decipher based on user input*/
    if (cipher == 1) {
        affine_cipher(buffer, buffer2, bytes_read, a, b, mod);
    } else {
        affine_decipher(buffer, buffer2, bytes_read, a, b, mod);
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

    /*Write the ciphered data to the output file*/
    fwrite(buffer2, sizeof(char), bytes_read, output_file);
    fclose(output_file);
    free(buffer);
    free(buffer2);


    return 0;
}


