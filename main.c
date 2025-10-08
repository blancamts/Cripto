#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt;
    int cipher = -1; // 1 for cipher, 0 for decipher, -1 for unset (error)
    char *input_filename = NULL;
    char *output_filename = NULL;
    int mod = -1; // -1 for unset (error)
    int a = -1, b = -1; // coefficients for affine cipher, -1 for unset (error)

    // Parse command line arguments
    while ((opt = getopt(argc, argv, "CDm:a:b:i:o:")) != -1) {
        switch (opt) {
            case 'C':
                cipher = 1
                break;
            case 'D':
                cipher = 0;
                break;
            case 'm':
                mod = atoi(optarg);
                break;
            case 'a':
                a = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
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
    if (cipher == -1 || mod == -1 || a == -1 || b == -1 || input_filename == NULL || output_filename == NULL) {
        fprintf(stderr, "Error: Missing or invalid arguments.\n");
        fprintf(stderr, "Usage: %s -C|-D -m mod -a a -b b -i inputfile -o outputfile\n", argv[0]);
        return EXIT_FAILURE;
    }if (mod <= 0 || a >= mod || b < 0 || b >= mod) {
        fprintf(stderr, "Error: Invalid values for mod, a, or b.\n");
        return EXIT_FAILURE;
    }





    return 0;
}