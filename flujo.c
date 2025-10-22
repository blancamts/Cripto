#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bits/getopt_core.h>
#include <math.h>
#include "utils.h"


int main(int argc, char *argv[]) {
    int opt;
    char *input_filename = NULL;
    char *output_filename = NULL;
    FILE *input_file = NULL;
    FILE *output_file = NULL;  
    int cipher = -1;
    int m = -1;
    uint32_t seed1 = 0;
    uint32_t seed2 = 0;


    /* Parse command line arguments */
    while ((opt = getopt(argc, argv, "CDi:o:1:2:m:")) != -1){

        switch (opt) {
            case 'C':
                cipher = 1;
                break;
            case 'D':
                cipher = 0;
                break;
            case '1':
                seed1 = (uint32_t)atoi(optarg);
                break;
            case '2':
                seed2 = (uint32_t)atoi(optarg);
                break;
            case 'i':
                input_filename = optarg;
                break;
            case 'o':   
                output_filename = optarg;
                break; 
            case 'm':
                m = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -C|-D [-1 seed1] [-2 seed2] [-m mod] [-i infile] [-o outfile]\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (cipher == -1) {
        fprintf(stderr, "Error: Missing or invalid arguments.\n");
        fprintf(stderr, "Usage: %s -C|-D [-1 seed1] [-2 seed2] [-m mod] [-i infile] [-o outfile]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (seed1 == 0 || seed2 == 0) {
        fprintf(stderr, "Error: Seeds must be non-zero.\n");
        return EXIT_FAILURE;
    }

    /* Read input */
    if (input_filename == NULL){
        input_file = stdin;
    }else{
        if(m == -1){
            input_file = fopen(input_filename, "rb");
        }else{
            input_file = fopen(input_filename, "r");
        }
        if (input_file == NULL) {
            perror("Error opening input file");
            return EXIT_FAILURE;
        }
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

    char * text = malloc(bytes_read + 1);

    if(m != -1){
        int purged = normalize_AZ(buffer, bytes_read, text);
        bytes_read = bytes_read - purged;
    }

    char *buffer2 = malloc(bytes_read + 1);

    if(m == -1){
        memcpy(text, buffer, bytes_read);
        stream_cipher(text, buffer2, bytes_read, seed1, seed2);
    }else{
        if (cipher == 1){
            stream_cipher_mod(text, buffer2, bytes_read, seed1, seed2, m);
        }else{
            stream_decipher_mod(text, buffer2, bytes_read, seed1, seed2, m);
        }
    }
        

    /*Open the output file for writing*/
    if (output_filename == NULL){
        output_file = stdout;
    }
    else{
        if(m == -1){
            output_file = fopen(output_filename, "wb");
        }else{
            output_file = fopen(output_filename, "w");   
        }
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