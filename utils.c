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
    for (i = 0; i < n; i++) {
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

void stream_cipher(const char *input, char *output, size_t length, uint32_t seed1, uint32_t seed2) {

    LFSR r1, r2;
    lfsr_init(&r1, seed1, 0x00400006u, 32);
    lfsr_init(&r2, seed2, 0xA3000000, 32);

    /*Use byte because we take 8 bits at a time to XOR with each byte of input*/
    for (size_t i = 0; i < length; i++) {
        uint8_t key_byte = 0;

        /* Generate 8 bits for the key byte */
        for (int bit = 0; bit < 8; bit++) {
            key_byte |= (shrinking_bit(&r1, &r2) << bit);
        }


        /* XOR*/
        output[i] = input[i] ^ key_byte;
    }

}


void stream_cipher_mod(const char *input, char *output, size_t length, uint32_t seed1, uint32_t seed2, int mod) {


    LFSR r1, r2;
    lfsr_init(&r1, seed1, 0x00400006u, 32);
    lfsr_init(&r2, seed2, 0xA3000000, 32);

    for (size_t i = 0; i < length; i++) {

        char c = input[i];
        /*Transform letter to number 0–25*/
        int x = c - 'A';  
        int key = 0;

        /* Generate 5 bits for z */
        for (int bit = 0; bit < 5; bit++) {
            key = (key << 1) | shrinking_bit(&r1, &r2);
        }
        /* Ensure key is within mod */
        key %= mod;

        
        int y;
        y = (x + key) % mod;
        output[i] = 'A' + y;
    }

    output[length] = '\0';
}

void stream_decipher_mod(const char *input, char *output, size_t length, uint32_t seed1, uint32_t seed2, int mod) {

    LFSR r1, r2;
    lfsr_init(&r1, seed1, 0x00400006u, 32);
    lfsr_init(&r2, seed2, 0xA3000000, 32);


    for (size_t i = 0; i < length; i++) {

        char c = input[i];
        /*Transform letter to number 0–25*/
        int y = c - 'A';  
        int key = 0;

        /* Generate 5 bits for z */
        for (int bit = 0; bit < 5; bit++) {
            key = (key << 1) | shrinking_bit(&r1, &r2);
        }


        /* Ensure key is within mod */
        key %= mod;

        
        int x;
        x = (y - key + mod) % mod;
        output[i] = 'A' + x;
    }

    output[length] = '\0';
}


void find_probable_key(const char *buffer, size_t length, int n, char *probable_key, int language) {
    char **cols;
    int i, j;
    int freq[n][26];
    int col_len[n];

    const double P_english[26] = {
        0.0804, 0.0154, 0.0306, 0.0399, 0.1251, 0.0230, 0.0196, 0.0549,
        0.0726, 0.0016, 0.0067, 0.0414, 0.0253, 0.0709, 0.0760, 0.0200,
        0.0011, 0.0612, 0.0654, 0.0925, 0.0271, 0.0099, 0.0192, 0.0019,
        0.0173, 0.0019
    };

    const double P_spanish[26] = {
        0.1196, 0.0092, 0.0292, 0.0687, 0.1678, 0.0052, 0.0073, 0.0089,
        0.0415, 0.0030, 0.0000, 0.0837, 0.0212, 0.0701, 0.0869, 0.0277,
        0.0153, 0.0494, 0.0788, 0.0331, 0.0480, 0.0039, 0.0000, 0.0006,
        0.0154, 0.0015
    };

    const double *P;
    if (language == 1){
        P = P_spanish;
    }else{
        P = P_english;
    }

    cols = malloc(n * sizeof(char *));
    for (i = 0; i < n; i++) {
        cols[i] = malloc((((length + n - 1) / n ) + 1) * sizeof(char)); /* ceil(lentgh/n) */ 
    }

    /*Initialize frequency array*/
    for (i = 0; i < n; i++) {
        for (j = 0; j < 26; j++) {
            freq[i][j] = 0;
        }
    }

    /*Rearange text into columns and calculate frequencies*/
    for (i = 0; i < n; i++)
        col_len[i] = 0;

    for (i = 0; i < length; i++) {
        int col = i % n;
        char c = buffer[i];
        if (c >= 'A' && c <= 'Z') {
            cols[col][col_len[col]] = c;
            freq[col][c - 'A']++;
            col_len[col]++;
        }
    }

    for (i = 0; i < n; i++) {
        double best_M = 0.0;
        int best_k = 0;
        int len = col_len[i];

        for (int k = 0; k < 26; k++) {
            double M = 0.0;
            for (int j = 0; j < 26; j++) {
                int shifted = (j + k) % 26;
                M += P[j] * ((double)freq[i][shifted] / len);
            }
            if (M > best_M) {
                best_M = M;
                best_k = k;
            }
        }

        probable_key[i] = 'A' + best_k;
    }

    probable_key[n] = '\0';

    for (i = 0; i < n; i++) {
        free(cols[i]);
    }
    free(cols);
}

int parse_values(const char *str, int *vec) {

    char *str_copy = strdup(str);
    if (!str_copy) return -1;

    char *token = strtok(str_copy, ",");
    int i = 0;

    while (token != NULL) {
        vec[i++] = atoi(token);
        token = strtok(NULL, ",");
    }

    free(str_copy);
    return i; 
}


void permutation_cipher(const char *input, char *output, const char *K1_str, const char *K2_str) {

    int *K1, *K2;
    K1 = malloc(strlen(K1_str) * sizeof(int));
    K2 = malloc(strlen(K2_str) * sizeof(int));

    int i, pad = 0;
    int M = parse_values(K1_str, K1);
    int N = parse_values(K2_str, K2);

    char *padded_text = malloc(strlen(input) + M*N);

    int block_size = M * N;
    int length = strlen(input);

    
    int res = length % block_size;
    pad = block_size - res;

    int padded_length = length + pad;

    for (int k = 0; k < length; k++)
        padded_text[k] = input[k];


    for (int k = length; k < padded_length; k++)
        padded_text[k] = 'X';

    padded_text[padded_length] = '\0';

    length = padded_length;


    for (i = 0; i < length; i += block_size){

        char block[M][N];
        char temp[M][N];
        char out[M][N];

        /* Fill the block */
        for (int m = 0; m < M; m++) {
            for (int n = 0; n < N; n++) {
                block[m][n] = padded_text[i + m * N + n];
            }
        }

        /*Permute rows*/
       for(int m1 = 0; m1 < M; m1++) {
            for (int n1 = 0; n1 < N; n1++) {
                temp[K1[m1]][n1] = block[m1][n1];
            }
        }

        /*Permute columns*/
        for (int n2 = 0; n2 < N; n2++) {
            for (int m2 = 0; m2 < M; m2++) {
                out[m2][K2[n2]] = temp[m2][n2];
            }
        }

        /*Write output*/
        for(int m = 0; m < M; m++) {
            for (int n = 0; n < N; n++) {
                output[i + m * N + n] = out[m][n];
            }
        }


    }

    output[length] = '\0';

    free(K1);
    free(K2);
    free(padded_text);
 

}

void inverse_permutation(int *K, int *inv, int size) {
    for (int i = 0; i < size; i++)
        inv[K[i]] = i;
}


void permutation_decipher(const char *input, char *output, const char *K1_str, const char *K2_str) {

    int *K1, *K2;
    K1 = malloc(strlen(K1_str) * sizeof(int));
    K2 = malloc(strlen(K2_str) * sizeof(int));


    int i;

    int M = parse_values(K1_str, K1);
    int N = parse_values(K2_str, K2);

    int *K1_inv = malloc(M * sizeof(int));
    int *K2_inv = malloc(N * sizeof(int));

    inverse_permutation(K1, K1_inv, M);
    inverse_permutation(K2, K2_inv, N);

    int block_size = M * N;
    int length = strlen(input);

    for (i = 0; i < length; i += block_size){

        char block[M][N];
        char temp[M][N];
        char out[M][N];

        /* Fill the block */
        for (int m = 0; m < M; m++) {
            for (int n = 0; n < N; n++) {
                block[m][n] = input[i + m * N + n];
            }
        }

        /*Inverse permute columns*/
        for (int m1 = 0; m1 < M; m1++) {
            for (int n1 = 0; n1 < N; n1++) {
                temp[m1][K2_inv[n1]] = block[m1][n1];
            }
        }

        /*Inverse permute rows*/
        for (int n2 = 0; n2 < N; n2++) {
            for (int m2 = 0; m2 < M; m2++) {
                out[K1_inv[m2]][n2] = temp[m2][n2];
            }
        }


        /*Write output*/
        for(int m = 0; m < M; m++) {
            for (int n = 0; n < N; n++) {
                output[i + m * N + n] = out[m][n];
            }
        }

    }

    output[length] = '\0';

    free(K1);
    free(K2);
    free(K1_inv);
    free(K2_inv);

}



