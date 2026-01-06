#include "../include/helper.h"

static uint16_t _cache_combs[17][17] = {0};

uint16_t nCr(uint8_t n, uint8_t r) {
    if (r > n) return 0;
    if (r == 0 || r == n) return 1;
    if (r > n - r) r = n - r;
    if (_cache_combs[n][r] != 0) return _cache_combs[n][r];
    _cache_combs[n][r] = nCr(n - 1, r - 1) + nCr(n - 1, r);
    return _cache_combs[n][r];
}

unsigned popcount(unsigned n) {
    #ifdef __GNUC__
        return __builtin_popcount(n);
    #else
        unsigned c = 0;
        while (n) { c++; n &= n - 1; }
        return c;
    #endif
}

uint32_t max_combination(uint8_t var_count, uint8_t minimization_level)
{
    if(minimization_level > var_count) return 0;
    uint16_t combinations = nCr(var_count, minimization_level);
    uint32_t remaining_choices = 1u << (var_count - minimization_level);
    return combinations * remaining_choices;
}
