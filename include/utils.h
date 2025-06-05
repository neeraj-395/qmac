#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define BINARY_CHOICE 0x02

uint16_t nCr(uint16_t n, uint16_t r);

uint16_t permutation_with_choices (
    uint16_t    num_items, 
    uint8_t     num_items_to_select, 
    uint8_t     num_choices
);

#endif // UTILS_H