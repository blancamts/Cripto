#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gmp.h>

void euclides(mpz_t a , mpz_t b, mpz_t res) {

    mpz_t r0, r1, r2, q;

    mpz_inits(r0, r1, r2, q, NULL);

    mpz_set(r0, a);
    mpz_set(r1, b);

    while (mpz_sgn(r1) != 0) {
        mpz_fdiv_q(q, r0, r1);  // q = r0 / r1
        mpz_mul(r2, q, r1);     // r2 = q * r1
        mpz_sub(r2, r0, r2);    // r2 = r0 - r2
        mpz_set(r0, r1);        // r0 = r1
        mpz_set(r1, r2);        // r1 = r2
    }

    mpz_set(res, r0);

    mpz_clears(r0, r1, r2, q, NULL);

}

int is_coprime(mpz_t a, mpz_t b) {

    mpz_t res;
    mpz_init(res);

    euclides(a, b, res);

    if (mpz_cmp_ui(res, 1) == 0) {
        // They are coprime
        mpz_clear(res);
        return 1;
    } else {
        // They are not coprime
        mpz_clear(res);
        return 0;
    }

}



int euclides_ext(int a, int m) {
    
}