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
    int *a_raw = NULL, *b_raw = NULL; /* coefficients for affine cipher, NULL for unset (error) */
    FILE *output_file;
    int n = -1; /* dimension of the matrix, -1 for unset (error) */
    int padding = 0;
    char *original_buffer;

    int i, j;

    mpz_t A, mod;
    mpz_inits(A, mod, NULL);

    /* Parse command line arguments */
    while ((opt = getopt(argc, argv, "CDn:m:a:b:i:o:")) != -1) {
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

            case 'n':
                n = atoi(optarg);
                if (n <= 1 || n > 3) {
                    fprintf(stderr, "Error: Dimension n must be a positive integer between 2 and 3\n");
                    return EXIT_FAILURE;
                }
                a_raw = malloc(n * n * sizeof(int));
                b_raw = malloc(n * sizeof(int));
                if (!a_raw || !b_raw) {
                    fprintf(stderr, "Error: Memory allocation failed\n");
                    return EXIT_FAILURE;
                }
                break;

            case 'a':
                for (i=0; i<n*n; i++) {
                    if (optind - 1 + i >= argc) {
                        fprintf(stderr, "Error: Not enough values for matrix 'a'\n");
                        return EXIT_FAILURE;
                    }
                    a_raw[i] = atoi(argv[optind - 1 + i]);
                }
                optind += n * n - 1;
                break;

            case 'b':
                for (i = 0; i < n; i++) {
                    if (optind - 1 + i >= argc) {
                        fprintf(stderr, "Error: Not enough values for vector 'b'\n");
                        return EXIT_FAILURE;
                    }
                    b_raw[i] = atoi(argv[optind - 1 + i]);
                }
                optind += n - 1;
                break;
            
            case 'i':
                input_filename = optarg;
                break;

            case 'o':
                output_filename = optarg;
                break;

            default:
                fprintf(stderr, "Usage: %s -C|-D -n n -m mod -a a -b b -i inputfile -o outputfile\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    /* Validate required arguments */
    if (cipher == -1 || mod_raw == -1 || a_raw == NULL || b_raw == NULL || n == -1) {
        fprintf(stderr, "Error: Missing or invalid arguments.\n");
        fprintf(stderr, "Usage: %s -C|-D -n n -m mod -a a -b b -i inputfile -o outputfile\n", argv[0]);
        return EXIT_FAILURE;
    }if (mod_raw <= 0) {
        fprintf(stderr, "Error: Mod must be a positive integer\n");
        return EXIT_FAILURE;
    }

    /* Convert mod to mpz_t */
    mpz_set_ui(mod, mod_raw);

    mpz_t** matrix = malloc(n * n * sizeof(mpz_t));
    mpz_t* vector = malloc(n * sizeof(mpz_t));

    for (i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(mpz_t));
        for (j = 0; j < n; j++){
            mpz_init(matrix[i][j]);
            mpz_init_set_si(matrix[i][j], a_raw[i * n + j]);
        }
    }

    for (i = 0; i < n; i++) {
        mpz_init(vector[i]);
        mpz_init_set_si(vector[i], b_raw[i]);
    }
    
    determinant(matrix, n, A);

    if (is_coprime(A, mod) == 0) {
        fprintf(stderr, "Error: a_det and mod are not coprime.\n");
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

    original_buffer = text; /* Keep pointer to free later */
    if (!cipher){
        /*Deciphering, read padding from header*/
        padding = text[bytes_read - 1] - 'A';
        text[bytes_read - 1] = '\0';
        bytes_read--;
    }else if (cipher) {
        padding = (n - (bytes_read % n)) % n;
        /*Pad the buffer to be a multiple of n*/
        if (bytes_read % n != 0) {
            size_t new_size = bytes_read + padding;
            text = realloc(text, new_size + 1);
            if (text == NULL) {
                perror("Error reallocating memory");
                return EXIT_FAILURE;
            }
            char padding_char = 'A' + (padding);
            memset(text + bytes_read, padding_char, padding);
            bytes_read = new_size;
            text[bytes_read] = '\0';
        }
    }

    char *buffer2 = malloc(bytes_read + 1);

    if (cipher) {
        /*Cipher*/
        affine_cipher_hill(text, buffer2, bytes_read, matrix, vector, n, mod);

        size_t len = strlen(buffer2);
        buffer2 = realloc(buffer2, len + 2); /* +1 para header, +1 para '\0' */
        if (!buffer2) {
            perror("Error reallocating memory for output buffer");
            return 1;
        }

        buffer2[bytes_read] = 'A' + (padding);
        buffer2[bytes_read + 1] = '\0';
        bytes_read = len + 1;
    }else{
        /*Decipher*/
        affine_decipher_hill(text, buffer2, bytes_read, matrix, vector, n, mod);
        bytes_read -= padding;
    }

    /*Open output file*/
    if (output_filename == NULL){
        output_file = stdout;
    }else{
        output_file = fopen(output_filename, "w");
        if (output_file == NULL) {
            perror("Error opening output file");
            return EXIT_FAILURE;
        }
    }

    fwrite(buffer2, sizeof(char), bytes_read, output_file);
    
    fclose(output_file);
    free(original_buffer);
    free(buffer2);

    /*free mpz*/
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++){
            mpz_clear(matrix[i][j]);
        }
        free(matrix[i]);
        mpz_clear(vector[i]);
    }
    free(matrix);
    free(vector);
    mpz_clear(mod);
    free(a_raw);
    free(b_raw);
    mpz_clear(A);

    return EXIT_SUCCESS;
}
