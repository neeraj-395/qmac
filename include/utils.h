#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

uint16_t nCr(uint8_t n, uint8_t r);

unsigned popcount(unsigned n);

uint32_t max_combination(uint8_t var_count, uint8_t minimization_level);

#endif // UTILS_H