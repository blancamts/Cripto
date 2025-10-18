#include "lfsr.h"

void lfsr_init(LFSR *l, uint32_t seed, uint32_t mask, int size){
    l->state = seed;
    l->mask = mask;
    l->size = size;
}

int lfsr_next_bit(LFSR *l){
    int output;
    uint32_t x;
    int feedback = 0;

    output = l->state & 1;
    x = l->state & l->mask;

    while (x){
        feedback ^= (x & 1)
        x >>= 1;
    }

    l->state = (l->state >> 1) | (uint32_t)feedback << (l->size - 1)

    return output;
}