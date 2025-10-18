#ifndef LFSR_H
#define LFSR_H

#include <stdint.h>

/* Struct for LFSR */
typedef struct {
    uint32_t state;  /* actual state of the LFSR */
    uint32_t mask;   /* mask defining the feedback taps */
    int size;        /* size of the LFSR in bits */
} LFSR;

void lfsr_init(LFSR *l, uint32_t seed, uint32_t mask, int size);

int lfsr_next_bit(LFSR *l);

#endif