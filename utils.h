#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gmp.h>
#include <ctype.h>
#include "lfsr.h"

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Implementation of the Euclidean Algorithm using the GMP library.
 *                This function computes gcd(a, b) and stores the result in res.
 *  Function:
 *      void euclides(mpz_t a, mpz_t b, mpz_t res);
 *
 *  Parameters:
 *      a   - First operand (mpz_t)
 *      b   - Second operand (mpz_t)
 *      res - Result variable where gcd(a, b) will be stored (mpz_t)
 *  Returns:
 *      void
 * ============================================================================
 */
void euclides(mpz_t a , mpz_t b, mpz_t res);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Checks whether two numbers are coprime using the Euclidean algorithm.
 *  Function:
 *      int is_coprime(mpz_t a, mpz_t b);
 *
 *  Parameters:
 *      a - First operand (mpz_t)
 *      b - Second operand (mpz_t)
 *  Returns:
 *      1 if gcd(a, b) == 1 (coprime), 0 otherwise
 * ============================================================================
 */
int is_coprime(mpz_t a, mpz_t b);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Extended Euclidean Algorithm using the GMP library.
 *                This function computes gcd(a, b) and finds integers x and y such that:
 *                a*x + b*y = gcd(a, b)
 *  Function:
 *      void euclides_ext(mpz_t a, mpz_t b, mpz_t gcd, mpz_t x, mpz_t y);
 *
 *  Parameters:
 *      a   - First operand (mpz_t)
 *      b   - Second operand (mpz_t)
 *      gcd - Result variable where gcd(a, b) will be stored (mpz_t)
 *      x   - Result variable where the coefficient for a will be stored (mpz_t)
 *      y   - Result variable where the coefficient for b will be stored (mpz_t)
 *  Returns:
 *      void
 * ============================================================================
 */
void euclides_ext(mpz_t a , mpz_t b, mpz_t gcd, mpz_t x, mpz_t y);

/*
* ============================================================================
*  Authors     : Blanca Matas, Luis Nuñez
*  Description : Computes modular inverse a⁻¹ (mod m) using the extended Euclidean algorithm.
*  Function:
*      void inverse_mod(mpz_t a, mpz_t mod, mpz_t a_inv);
*
*  Parameters:
*      a     - Operand (mpz_t)
*      mod   - Modulus (mpz_t)
*      a_inv - Output variable where the modular inverse will be stored (mpz_t)
*  Returns:
*      void
* ============================================================================
*/
void inverse_mod(mpz_t a, mpz_t mod, mpz_t a_inv);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Applies the affine cipher encryption: E(x) = (a*x + b) mod m.
 *  Function:
 *      void affine_cipher(const char *input, char *output, size_t length,
 *                         mpz_t a, mpz_t b, mpz_t mod);
 *
 *  Parameters:
 *      input  - Plaintext string
 *      output - Ciphertext buffer
 *      length - Length of input text
 *      a      - Multiplicative coefficient (mpz_t)
 *      b      - Additive coefficient (mpz_t)
 *      mod    - Modulus (mpz_t)
 *  Returns:
 *      void
 * ============================================================================
 */
void affine_cipher(const char *input, char *output, size_t length, mpz_t a, mpz_t b, mpz_t mod);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Applies the affine cipher decryption: D(y) = a⁻¹ * (y - b) mod m.
 *  Function:
 *      void affine_decipher(const char *input, char *output, size_t length,
 *                           mpz_t a, mpz_t b, mpz_t mod);
 *
 *  Parameters:
 *      input  - Ciphertext string
 *      output - Plaintext buffer
 *      length - Length of input text
 *      a      - Multiplicative coefficient (mpz_t)
 *      b      - Additive coefficient (mpz_t)
 *      mod    - Modulus (mpz_t)
 *  Returns:
 *      void
 * ============================================================================
 */
void affine_decipher(const char *input, char *output, size_t length, mpz_t a, mpz_t b, mpz_t mod);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Computes the determinant of a square matrix using GMP arithmetic.
 *  Function:
 *      void determinant(mpz_t **matrix, int n, mpz_t det_out);
 *
 *  Parameters:
 *      matrix   - Input square matrix (array of mpz_t pointers)
 *      n        - Dimension of the matrix (n x n)
 *      det_out  - Output variable to store the determinant
 *  Returns:
 *      void
 * ============================================================================
 */
void determinant(mpz_t **matrix, int n, mpz_t det_out);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Multiplies a matrix A (n x n) by a vector x (size n) under modulo arithmetic.
 *  Function:
 *      void matrix_mul(mpz_t *result, mpz_t **A, mpz_t *x, int n, mpz_t mod);
 *
 *  Parameters:
 *      result - Output vector (mpz_t array)
 *      A      - Input matrix (n x n)
 *      x      - Input vector (size n)
 *      n      - Dimension of the matrix and vectors
 *      mod    - Modulus for modular arithmetic
 *  Returns:
 *      void
 * ============================================================================
 */
void matrix_mul(mpz_t *result, mpz_t **A, mpz_t *x, int n, mpz_t mod);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Encrypts text using the affine Hill cipher.
 *                Applies the transformation y = (A * x + b) mod m.
 *  Function:
 *      void affine_cipher_hill(const char *input, char *output, size_t length,
 *                              mpz_t **A, mpz_t *b, int n, mpz_t mod);
 *
 *  Parameters:
 *      input   - Plaintext string
 *      output  - Ciphertext buffer
 *      length  - Length of input text
 *      A       - Key matrix (n x n)
 *      b       - Key vector (size n)
 *      n       - Block size (dimension)
 *      mod     - Modulus for arithmetic operations
 *  Returns:
 *      void
 * ============================================================================
 */
void affine_cipher_hill(const char *input, char *output, size_t length, mpz_t **A, mpz_t *b, int n, mpz_t mod);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Computes the modular inverse of a square matrix using GMP arithmetic.
 *                Currently implemented for 2x2 matrices.
 *  Function:
 *      void inverse_matrix(mpz_t **matrix, mpz_t **inv_matrix, int n, mpz_t mod);
 *
 *  Parameters:
 *      matrix     - Input matrix to invert (n x n)
 *      inv_matrix - Output matrix that will store the inverse
 *      n          - Dimension of the matrix (currently supports n = 2)
 *      mod        - Modulus for modular arithmetic
 *  Returns:
 *      void
 * ============================================================================
 */
void inverse_matrix(mpz_t **matrix, mpz_t **inv_matrix, int n, mpz_t mod);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Decrypts text encrypted with the affine Hill cipher.
 *                Applies the inverse transformation x = A⁻¹ * (y - b) mod m.
 *  Function:
 *      void affine_decipher_hill(const char *input, char *output, size_t length,
 *                                mpz_t **A, mpz_t *b, int n, mpz_t mod);
 *
 *  Parameters:
 *      input   - Ciphertext string
 *      output  - Plaintext buffer
 *      length  - Length of input text
 *      A       - Key matrix (n x n)
 *      b       - Key vector (size n)
 *      n       - Block size (dimension)
 *      mod     - Modulus for arithmetic operations
 *  Returns:
 *      void
 * ============================================================================
 */
void affine_decipher_hill(const char *input, char *output, size_t length, mpz_t **A, mpz_t *b, int n, mpz_t mod);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Encrypts text using the Vigenère cipher with a given key.
 *                Each character is shifted by the corresponding key character.
 *  Function:
 *      void vigenere_cipher(const char *input, char *output, size_t length,
 *                           const char *key);
 *
 *  Parameters:
 *      input   - Plaintext string
 *      output  - Ciphertext buffer
 *      length  - Length of input text
 *      key     - Key string (A–Z letters)
 *  Returns:
 *      void
 * ============================================================================
 */
void vigenere_cipher(const char *input, char *output, size_t length, const char *key);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Decrypts text encrypted with the Vigenère cipher using the same key.
 *                Reverses the shift applied during encryption.
 *  Function:
 *      void vigenere_decipher(const char *input, char *output, size_t length,
 *                             const char *key);
 *
 *  Parameters:
 *      input   - Ciphertext string
 *      output  - Plaintext buffer
 *      length  - Length of input text
 *      key     - Key string (A–Z letters)
 *  Returns:
 *      void
 * ============================================================================
 */
void vigenere_decipher(const char *input, char *output, size_t length, const char *key);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Normalizes a text buffer by removing non-alphabetic characters
 *                and converting all letters to uppercase (A–Z).
 *  Function:
 *      int normalize_AZ(char *buffer, size_t length, char *text);
 *
 *  Parameters:
 *      buffer - Input text buffer
 *      length - Length of the input text
 *      text   - Output buffer to store normalized text
 *  Returns:
 *      Number of characters removed (non-alphabetic)
 * ============================================================================
 */
int normalize_AZ(char *buffer, size_t length, char *text);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Calculates the Index of Coincidence (IC) for a given text.
 *                Used to estimate probable key length in Vigenère analysis.
 *  Function:
 *      double calculate_ic(const char *buffer, size_t length, int n);
 *
 *  Parameters:
 *      buffer - Input text (A–Z uppercase)
 *      length - Length of text
 *      n      - Hypothesized key length
 *  Returns:
 *      Average Index of Coincidence value across all n columns
 * ============================================================================
 */
double calculate_ic(const char *buffer, size_t length, int n);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Estimates the most probable key for a Vigenère cipher 
 *                using frequency analysis and the correlation method.
 *  Function:
 *      void find_probable_key(const char *buffer, size_t length, int n,
 *                             char *probable_key, int language);
 *
 *  Parameters:
 *      buffer        - Ciphertext text (A–Z uppercase)
 *      length        - Length of ciphertext
 *      n             - Key length to analyze
 *      probable_key  - Output string where the estimated key is stored
 *      language      - 0 for English frequencies, 1 for Spanish
 *  Returns:
 *      void
 * ============================================================================
 */
void find_probable_key(const char *buffer, size_t length, int n, char *probable_key, int language);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Computes the greatest common divisor (GCD) of two integers 
 *                using the standard iterative Euclidean algorithm.
 *  Function:
 *      int gcd_aux(int a, int b);
 *
 *  Parameters:
 *      a - First integer
 *      b - Second integer
 *  Returns:
 *      gcd(a, b)
 * ============================================================================
 */
int gcd_aux(int a, int b);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Generates a single bit from a shrinking generator.
 *                Uses two LFSRs: one as control and one as data source.
 *  Function:
 *      int shrinking_bit(LFSR *r1, LFSR *r2);
 *
 *  Parameters:
 *      r1 - Control LFSR (decides whether to output or discard a bit)
 *      r2 - Data LFSR (produces candidate bits)
 *  Returns:
 *      One generated bit (0 or 1)
 * ============================================================================
 */
int shrinking_bit(LFSR *r1, LFSR *r2);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Implements a stream cipher using two LFSRs combined 
 *                via a shrinking generator. Encrypts or decrypts by XOR.
 *  Function:
 *      void stream_cipher(const char *input, char *output, size_t length,
 *                         uint32_t seed1, uint32_t seed2);
 *
 *  Parameters:
 *      input  - Input data (plaintext or ciphertext)
 *      output - Output buffer
 *      length - Length of data
 *      seed1  - Initial state for first LFSR
 *      seed2  - Initial state for second LFSR
 *  Returns:
 *      void
 * ============================================================================
 */
void stream_cipher(const char *input, char *output, size_t length, uint32_t seed1, uint32_t seed2);


/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Decrypts text encrypted with the modular stream cipher.
 *                Reverses modular addition by subtracting key modulo m.
 *  Function:
 *      void stream_decipher_mod(const char *input, char *output, size_t length,
 *                               uint32_t seed1, uint32_t seed2, int mod);
 *
 *  Parameters:
 *      input  - Ciphertext (A–Z)
 *      output - Plaintext buffer
 *      length - Length of text
 *      seed1  - Seed for first LFSR
 *      seed2  - Seed for second LFSR
 *      mod    - Modulus for character arithmetic (typically 26)
 *  Returns:
 *      void
 * ============================================================================
 */
void stream_cipher_mod(const char *input, char *output, size_t length, uint32_t seed1, uint32_t seed2, int mod);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Decrypts text encrypted with the modular stream cipher.
 *                Reverses modular addition by subtracting key modulo m.
 *  Function:
 *      void stream_decipher_mod(const char *input, char *output, size_t length,
 *                               uint32_t seed1, uint32_t seed2, int mod);
 *
 *  Parameters:
 *      input  - Ciphertext (A–Z)
 *      output - Plaintext buffer
 *      length - Length of text
 *      seed1  - Seed for first LFSR
 *      seed2  - Seed for second LFSR
 *      mod    - Modulus for character arithmetic (typically 26)
 *  Returns:
 *      void
 * ============================================================================
 */
void stream_decipher_mod(const char *input, char *output, size_t length, uint32_t seed1, uint32_t seed2, int mod);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Parses a comma-separated string of integers into an integer array.
 *  Function:
 *      int parse_values(const char *str, int *vec);
 *
 *  Parameters:
 *      str - Input string (e.g. "3,1,4,2")
 *      vec - Output array where parsed integers are stored
 *  Returns:
 *      Number of integers successfully parsed
 * ============================================================================
 */
int parse_values(const char *str, int *vec);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Encrypts text using a double permutation cipher based on
 *                row and column permutations defined by K1 and K2.
 *                Padding with 'X' is applied if needed.
 *  Function:
 *      void permutation_cipher(const char *input, char *output,
 *                              const char *K1_str, const char *K2_str);
 *
 *  Parameters:
 *      input   - Plaintext string
 *      output  - Ciphertext buffer
 *      K1_str  - String representing row permutation (e.g. "3,1,4,2")
 *      K2_str  - String representing column permutation (e.g. "4,2,1,3")
 *  Returns:
 *      void
 * ============================================================================
 */
void permutation_cipher(const char *input, char *output, const char *K1_str, const char *K2_str);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Computes the inverse of a given permutation vector.
 *                For each position i, sets inv[K[i]] = i.
 *  Function:
 *      void inverse_permutation(int *K, int *inv, int size);
 *
 *  Parameters:
 *      K    - Original permutation vector
 *      inv  - Output array where the inverse permutation will be stored
 *      size - Size of permutation vector
 *  Returns:
 *      void
 * ============================================================================
 */
void inverse_permutation(int *K, int *inv, int size);

/*
 * ============================================================================
 *  Authors     : Blanca Matas, Luis Nuñez
 *  Description : Decrypts text encrypted with the double permutation cipher
 *                by applying inverse permutations of rows and columns.
 *  Function:
 *      void permutation_decipher(const char *input, char *output,
 *                                const char *K1_str, const char *K2_str);
 *
 *  Parameters:
 *      input   - Ciphertext string
 *      output  - Plaintext buffer
 *      K1_str  - String representing row permutation (e.g. "3,1,4,2")
 *      K2_str  - String representing column permutation (e.g. "4,2,1,3")
 *  Returns:
 *      void
 * ============================================================================
 */
void permutation_decipher(const char *input, char *output, const char *K1_str, const char *K2_str);

#endif /*UTILS_H*/