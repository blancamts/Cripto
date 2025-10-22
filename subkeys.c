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
    int l = -1; /* Key size */
    char *input_filename = NULL;
    char *output_filename = NULL;
    FILE *input_file = NULL;
    FILE *output_file = NULL;  

    int i;

    /* Parse command line arguments */
    while ((opt = getopt(argc, argv, "l:i:o:")) != -1) {
        switch (opt) {
            case 'l':
                l = atoi(optarg);
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

   /* Open input file */
   input_file = fopen(input_filename, "rb");
   if (!input_file) {
       perror("Error opening input file");
       return EXIT_FAILURE;
   }

   /* Open output file */
   output_file = fopen(output_filename, "wb");
   if (!output_file) {
       perror("Error opening output file");
       fclose(input_file);
       return EXIT_FAILURE;
   }

    char *temp = NULL;
    
    size_t size = 0;

    /*Read the entire file into memory*/
    ssize_t bytes_read = getline(&temp, &size, input_file);
    if (bytes_read == -1) {
        perror("Error reading file");
        fclose(input_file);
        return EXIT_FAILURE;
    }

    char *buffer = malloc(bytes_read);

    int purged = normalize_AZ(temp, bytes_read, buffer);
    bytes_read = bytes_read - purged;

    if (l <= 0){
        l = bytes_read; /*Default try all n possible*/
    }

    fclose(input_file);

    double best_ic = 0.0;
    int best_ic_idx = 0;

    for (i = 1; i < l; i++){
        double ic = calculate_ic(buffer, bytes_read, i);
        printf("IC for n = %d: %f\n", i, ic);

        if (fabs(ic - IC_ENGLISH) < fabs(best_ic - IC_ENGLISH)){
            best_ic = ic;
            best_ic_idx = i;
        }
    }

    

   /* Clean up */
   fclose(input_file);
   fclose(output_file);
   return EXIT_SUCCESS;


}
