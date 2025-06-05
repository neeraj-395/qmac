#include <math.h>

#include "../include/utils.h"

static uint16_t _cache_combs[16][16] = {0};

uint16_t nCr(uint16_t n, uint16_t r) {
    if (r > n) return 0;
    if (r == 0 || r == n) return 1;
    if (_cache_combs[n][r] != 0) return _cache_combs[n][r];
    _cache_combs[n][r] = nCr(n - 1, r - 1) + nCr(n - 1, r);
    return _cache_combs[n][r];
}

uint16_t permutation_with_choices (
    uint16_t    num_items, 
    uint8_t     num_items_to_select, 
    uint8_t     num_choices
)
{
    uint16_t combinations = nCr(num_items, num_items_to_select);
    uint16_t remaining_choices = pow(num_choices, num_items - num_items_to_select);
    return combinations * remaining_choices;
}
