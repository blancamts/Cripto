#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gmp.h>

void euclides(mpz_t a , mpz_t b, mpz_t res);
int is_coprime(mpz_t a, mpz_t b);
void euclides_ext(mpz_t a , mpz_t b, mpz_t gcd, mpz_t x, mpz_t y);
int has_inverse_mod(mpz_t a, mpz_t mod);
void inverse_mod(mpz_t a, mpz_t mod, mpz_t a_inv);
void affine_cipher(const char *input, char *output, size_t length, mpz_t a, mpz_t b, mpz_t mod);
void affine_decipher(const char *input, char *output, size_t length, mpz_t a, mpz_t b, mpz_t mod);
void determinant(mpz_t **matrix, int n, mpz_t det_out);
void matrix_mul(mpz_t *result, mpz_t **A, mpz_t *x, int n, mpz_t mod);
void affine_cipher_hill(const char *input, char *output, size_t length, mpz_t **A, mpz_t *b, int n, mpz_t mod);
void inverse_matrix(mpz_t **matrix, mpz_t **inv_matrix, int n, mpz_t mod);
void affine_decipher_hill(const char *input, char *output, size_t length, mpz_t **A, mpz_t *b, int n, mpz_t mod);
#endif /*UTILS_H*/