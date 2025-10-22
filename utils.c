#include "utils.h"
#include "lfsr.h"
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
        /*They are coprime*/
        mpz_clear(res);
        return 1;
    } else {
        /*They are not coprime*/
        mpz_clear(res);
        return 0;
    }

}

void euclides_ext(mpz_t a , mpz_t b, mpz_t gcd, mpz_t x, mpz_t y) {
    mpz_t r0, r1, r2, q;
    mpz_t x0, x1, y0, y1, x2, y2;
    mpz_inits(r0, r1, r2, q, x0, x1, y0, y1, x2, y2, NULL);

    mpz_set(r0, a);
    mpz_set(r1, b);

    mpz_set_ui(x0, 1); mpz_set_ui(y0, 0);   
    mpz_set_ui(x1, 0); mpz_set_ui(y1, 1);   

    while (mpz_sgn(r1) != 0) {
        mpz_fdiv_q(q, r0, r1);

        mpz_mul(r2, q, r1);
        mpz_sub(r2, r0, r2);

        mpz_mul(x2, q, x1);
        mpz_sub(x2, x0, x2);

        mpz_mul(y2, q, y1);
        mpz_sub(y2, y0, y2);


        mpz_set(r0, r1);  mpz_set(r1, r2);
        mpz_set(x0, x1);  mpz_set(x1, x2);
        mpz_set(y0, y1);  mpz_set(y1, y2);
    }

    mpz_set(gcd, r0);  
    mpz_set(x, x0);    
    mpz_set(y, y0);    

    mpz_clears(r0, r1, r2, q, x0, x1, y0, y1, x2, y2, NULL);
}

void inverse_mod(mpz_t a, mpz_t mod, mpz_t a_inv) {
    mpz_t gcd, x, y;
    mpz_inits(gcd, x, y, NULL);

    if (!is_coprime(a, mod)) {
        mpz_set_ui(a_inv, 0); 
        mpz_clears(gcd, x, y, NULL);
        return;
    }

    euclides_ext(a, mod, gcd, x, y);

    mpz_mod(a_inv, x, mod);

    mpz_clears(gcd, x, y, NULL);
}

void affine_cipher(const char *input, char *output, size_t length, mpz_t a, mpz_t b, mpz_t mod){

    mpz_t x, y;
    mpz_inits(x, y, NULL);

    for (size_t i = 0; i < length; i++) {
        char c = input[i];
        /*If the character is the the correct range*/
        if (c >= 'A' && c <= 'Z') {
            int x_int = c - 'A';
            mpz_set_ui(x, x_int);
            /*Apply affine transformation*/
            mpz_mul(y, a, x); // a * x
            mpz_add(y, y, b); // a * x + b
            mpz_mod(y, y, mod); // mod
            output[i] = (char)(mpz_get_ui(y) + 'A');
        } else {
            output[i] = c;
        }
    }
    output[length] = '\0';
    mpz_clears(x, y, NULL);
  
}

void affine_decipher(const char *input, char *output, size_t length, mpz_t a, mpz_t b, mpz_t mod){

    mpz_t a_inv, x, y;
    mpz_inits(a_inv, x, y, NULL);

    inverse_mod(a, mod, a_inv);

    for (size_t i = 0; i < length; i++) {
        char c = input[i];
        /*If the character is the the correct range*/
        if (c >= 'A' && c <= 'Z') {
            int y_int = c - 'A';
            mpz_set_ui(y, y_int);
            /*Apply inverse affine transformation*/
            mpz_sub(y, y, b); // y - b
            mpz_mod(y, y, mod); // mod
            mpz_mul(x, a_inv, y); // a_inv * (y - b)
            mpz_mod(x, x, mod); // mod
            output[i] = (char)(mpz_get_ui(x) + 'A');
        } else {
            output[i] = c;
        }
    }
    output[length] = '\0';
    mpz_clears(a_inv, x, y, NULL);
  
}



void determinant(mpz_t **matrix, int n, mpz_t det_out) {
    mpz_t det;
    mpz_init(det);

    if (n == 2){
        mpz_t term1, term2;
        mpz_inits(term1, term2, NULL);
        mpz_mul(term1, matrix[0][0], matrix[1][1]);
        mpz_mul(term2, matrix[0][1], matrix[1][0]);
        mpz_sub(det, term1, term2);
        mpz_clears(term1, term2, NULL);
        mpz_set(det_out, det);
        return;
    }
    if (n == 3) {
        mpz_t term1, term2, term3, term4, term5, term6;
        mpz_inits(term1, term2, term3, term4, term5, term6, NULL);
        mpz_mul(term1, matrix[0][0], matrix[1][1]);
        mpz_mul(term1, term1, matrix[2][2]);
        mpz_mul(term2, matrix[0][1], matrix[1][2]);
        mpz_mul(term2, term2, matrix[2][0]);
        mpz_mul(term3, matrix[0][2], matrix[1][0]);
        mpz_mul(term3, term3, matrix[2][1]);
        mpz_mul(term4, matrix[0][2], matrix[1][1]);
        mpz_mul(term4, term4, matrix[2][0]);
        mpz_mul(term5, matrix[0][0], matrix[1][2]);
        mpz_mul(term5, term5, matrix[2][1]);
        mpz_mul(term6, matrix[0][1], matrix[1][0]);
        mpz_mul(term6, term6, matrix[2][2]);
        mpz_add(det, term1, term2);
        mpz_add(det, det, term3);
        mpz_sub(det, det, term4);
        mpz_sub(det, det, term5);
        mpz_sub(det, det, term6);
        mpz_clears(term1, term2, term3, term4, term5, term6, NULL);
        mpz_set(det_out, det);
        return;
    }
    return;
}

void matrix_mul(mpz_t *result, mpz_t **A, mpz_t *x, int n, mpz_t mod) {

    /* Iterate over each row of A */
    for (int i = 0; i < n; i++) {
        mpz_set_ui(result[i], 0);
        /*Iterate over each column of x */
        for (int j = 0; j < n; j++) {
            mpz_t temp;
            mpz_init(temp);
            /*Multiply and accumulate*/
            mpz_mul(temp, A[i][j], x[j]);
            /* Add to the result */
            mpz_add(result[i], result[i], temp);
            mpz_clear(temp);
        }
        mpz_mod(result[i], result[i], mod);
    }
}


void affine_cipher_hill(const char *input, char *output, size_t length, mpz_t **A, mpz_t *b, int n, mpz_t mod) {


    /*Initialize vectors x and y*/
    mpz_t *x = malloc(n * sizeof(mpz_t));
    mpz_t *y = malloc(n * sizeof(mpz_t));
    int mask[length];
    char special_chars[length];
    for (int i = 0; i < n; i++) {
        mpz_init(x[i]);
        mpz_init(y[i]);
    }

    /*Process the input in blocks of size n*/
    for (size_t i = 0; i < length; i += n) {

        /* Build the input block vector x */
        for (int j = 0; j < n; j++) {
            if (i + j < length) {
                char c = input[i + j];
                if (c >= 'A' && c <= 'Z') {
                    int x_int = c - 'A';
                    mpz_set_ui(x[j], x_int);
                    mask[i + j] = 0;
                } else {
                    mask[i + j] = 1;
                    special_chars[i + j] = c;
                    mpz_set_ui(x[j], 0); 
                }
            } else {
                mpz_set_ui(x[j], 0); 
            }
        }

        /*Compute y = A * x + b (mod) */
        matrix_mul(y, A, x, n, mod);
        for (int j = 0; j < n; j++) {
            mpz_add(y[j], y[j], b[j]);
            mpz_mod(y[j], y[j], mod);
        }

        /* Write the output block */
        for (int j = 0; j < n; j++) {
            if (i + j < length) {
                if(mask[i + j] == 1){
                    output[i + j] = special_chars[i + j];
                }else{
                    output[i + j] = (char)(mpz_get_ui(y[j]) + 'A');
                }
            }
        }
    }
    output[length] = '\0';

    for (int i = 0; i < n; i++) {
        mpz_clear(x[i]);
        mpz_clear(y[i]);
    }
    free(x);
    free(y);
}


void inverse_matrix(mpz_t **matrix, mpz_t **inv_matrix, int n, mpz_t mod) {
    if (n == 2) {
        mpz_t det, det_inv;
        mpz_inits(det, det_inv, NULL);
        /*Calculate determinant and its inverse modulo mod*/
        determinant(matrix, n, det);
        inverse_mod(det, mod, det_inv);
        mpz_t temp;
        mpz_init(temp);

        /*Calculate inverse matrix using formula for 2x2 matrix*/
        mpz_mul(temp, matrix[1][1], det_inv);
        mpz_mod(inv_matrix[0][0], temp, mod);

        mpz_mul(temp, matrix[0][1], det_inv);
        mpz_neg(temp, temp);
        mpz_mod(inv_matrix[0][1], temp, mod);

        mpz_mul(temp, matrix[1][0], det_inv);
        mpz_neg(temp, temp);
        mpz_mod(inv_matrix[1][0], temp, mod);

        mpz_mul(temp, matrix[0][0], det_inv);
        mpz_mod(inv_matrix[1][1], temp, mod);

        mpz_clear(det);
        mpz_clear(det_inv);
        mpz_clear(temp);
    }

    else if (n == 3) {
        /*Not implemented*/
        return;
    }

}

void affine_decipher_hill(const char *input, char *output, size_t length, mpz_t **A, mpz_t *b, int n, mpz_t mod) {
    
    /*Compute inverse matrix of A*/
    mpz_t **A_inv = malloc(n * sizeof(mpz_t *));
    for (int i = 0; i < n; i++) {
        A_inv[i] = malloc(n * sizeof(mpz_t));
        for (int j = 0; j < n; j++) {
            mpz_init(A_inv[i][j]);
        }
    }
    inverse_matrix(A, A_inv, n, mod);

    /*Initialize vectors x and y*/
    mpz_t *x = malloc(n * sizeof(mpz_t));
    mpz_t *y = malloc(n * sizeof(mpz_t));
    for (int i = 0; i < n; i++) {
        mpz_init(x[i]);
        mpz_init(y[i]);
    }

    int mask[length];
    char special_chars[length];

    /*Process the input in blocks of size n*/
    for (size_t i = 0; i < length; i += n){

        /* Build the input block vector y */
        for (int j = 0; j < n; j++) {
            if (i + j < length) {
                char c = input[i + j];
                if (c >= 'A' && c <= 'Z') {
                    int y_int = c - 'A';
                    mpz_set_ui(y[j], y_int);
                    mask[i + j] = 0;
                } else {
                    mpz_set_ui(y[j], 0);
                    mask[i + j] = 1;
                    special_chars[i + j] = c;

                }
            } else {
                mpz_set_ui(y[j], 0); 
            }
        }

        /*Compute x = A_inv * (y - b) (mod) */
        for (int j = 0; j < n; j++) {
            mpz_sub(y[j], y[j], b[j]);
            mpz_mod(y[j], y[j], mod);
        }
        matrix_mul(x, A_inv, y, n, mod);

        /* Write the output block */
        for (int j = 0; j < n; j++) {
            if (i + j < length) {
                if(mask[i + j] == 1){
                    output[i + j] = special_chars[i + j];
                }else{
                    output[i + j] = (char)(mpz_get_ui(x[j]) + 'A');
                }
            }
        }
    }
    output[length] = '\0';

    for (int i = 0; i < n; i++) {
        mpz_clear(x[i]);
        mpz_clear(y[i]);
    }

    free(x);
    free(y);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mpz_clear(A_inv[i][j]);
        }
        free(A_inv[i]);
    }
    free(A_inv);
}

void vigenere_cipher(const char *input, char *output, size_t length, const char *key){

    size_t key_l = strlen(key);
    char c, k;
    int c_ciphered;

    for (size_t i = 0; i < length; i++) {
        c = input[i];
        if (c >= 'A' && c <= 'Z') {
            k = key[i % key_l];
            c_ciphered = ((c - 'A') + (k - 'A')) % 26;
            output[i] = (char)(c_ciphered + 'A');
        } else {
            output[i] = c;
        }
    }
    output[length] = '\0';

}

void vigenere_decipher(const char *input, char *output, size_t length, const char *key){

    size_t key_l = strlen(key);
    char c, k;
    int c_deciphered;

    for (size_t i = 0; i < length; i++) {
        c = input[i];
        if (c >= 'A' && c <= 'Z') {
            k = key[i % key_l];
            c_deciphered = ((c - 'A') - (k - 'A') + 26) % 26;
            output[i] = (char)(c_deciphered + 'A');
        } else {
            output[i] = c;
        }
    }
    output[length] = '\0';
}

/* Returns number of characters purged from buffer */
int normalize_AZ(char *buffer, size_t length, char *text) {
    size_t k = 0;
    int purged = 0;

    for (size_t i = 0; buffer[i] != '\0'; i++) {
        if (isalpha((unsigned char)buffer[i])) {
            text[k++] = toupper((unsigned char)buffer[i]);
        }else{
            purged++;
        }
    }
    text[k] = '\0';
    return purged;
}

double calculate_ic(const char *buffer, size_t length, int n) {
    char **cols;
    int i, j, col_index;
    double ic_total = 0.0;
    int freq[n][26];

    cols = malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++) {
        cols[i] = malloc((((length + n - 1) / n ) + 1) * sizeof(char)); /* ceil(lentgh/n) */
    }

    /*Initialize frequency array*/
    for (i = 0; i < n; i++) {
        for (j = 0; j < 26; j++) {
            freq[i][j] = 0;
        }
    }

    for (i = 0, col_index = 0; i < length; i++) {
        cols[i%n][col_index] = buffer[i];
        freq[i%n][buffer[i] - 'A']++;
        if ((i+1) % n == 0) {
            col_index++;
        }
    }

    for (i = 0; i < n; i++) {
        if (i < length % n) {
            cols[i][col_index] = '\0';
        } else {
            cols[i][col_index - 1] = '\0';
        }
    }

    for (i = 0; i < n; i++){
        size_t col_length = strlen(cols[i]);
        double ic_col = 0.0;
        for (j = 0; j < 26; j++) { /*For each letter A-Z (26 letters)*/
            ic_col += freq[i][j] * (freq[i][j] - 1);
        }
        ic_col /= (col_length * (col_length - 1));
        ic_total += ic_col;
    }

    for (i = 0; i < n; i++) {
        free(cols[i]);
    }
    free(cols);

    return ic_total / n;
}

int gcd_aux(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


int shrinking_bit(LFSR *r1, LFSR *r2){

    int bit;
    /* Generate bits until control bit is 1 */
    while (1) {
        int control = lfsr_next_bit(r1);
        int candidate = lfsr_next_bit(r2);
        if (control == 1) {
            bit = candidate;
            break;
        }
    }
    return bit;
}

void stream_cipher(const char *input, char *output, size_t length, LFSR *r1, LFSR *r2) {

    /*Use byte because we take 8 bits at a time to XOR with each byte of input*/
    for (size_t i = 0; i < length; i++) {
        uint8_t key_byte = 0;

        /* Generate 8 bits for the key byte */
        for (int bit = 0; bit < 8; bit++) {
            key_byte |= (shrinking_bit(r1, r2) << bit);
        }

        /* XOR*/
        output[i] = input[i] ^ key_byte;
    }

    output[length] = '\0';
}


void stream_cipher_mod(const char *input, char *output, size_t length, LFSR *r1, LFSR *r2, int mod) {

    for (size_t i = 0; i < length; i++) {

        char c = input[i];
        /*Transform letter to number 0–25*/
        int x = c - 'A';  
        int key = 0;

        /* Generate 5 bits for z */
        for (int bit = 0; bit < 5; bit++) { 
            key |= (shrinking_bit(r1, r2) << bit);
        }

        /* Ensure key is within mod */
        key %= mod;
        
        int y;
        y = (x + key) % mod;
        output[i] = 'A' + y;
    }

    output[length] = '\0';
}

void stream_decipher_mod(const char *input, char *output, size_t length, LFSR *r1, LFSR *r2, int mod) {

    for (size_t i = 0; i < length; i++) {

        char c = input[i];
        /*Transform letter to number 0–25*/
        int y = c - 'A';  
        int key = 0;

        /* Generate 5 bits for z */
        for (int bit = 0; bit < 5; bit++) { 
            key |= (shrinking_bit(r1, r2) << bit);
        }

        /* Ensure key is within mod */
        key %= mod;
        
        int x;
        x = (y - key + mod) % mod;
        output[i] = 'A' + x;
    }

    output[length] = '\0';
}













