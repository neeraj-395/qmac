#include <stdio.h>

#include "../include/utils.h"
#include "../include/implicant.h"

Implicant imp_create(uint16_t term, uint16_t mask, bool is_dontcare) {
    return (Implicant) {
        .term = term, .mask = mask, .ones_count = popcount(term), 
        .is_combined = false, .is_dontcare = is_dontcare,
    };
}

bool imp_can_combine(const Implicant *a, const Implicant *b) {
    return popcount(a->term ^ b->term) == 1 && a->mask == b->mask;
}

bool imp_implies(const Implicant *a, const Implicant *b) {
    return (a->term ^ b->term) < a->mask;
}

Implicant imp_combine(const Implicant *a, const Implicant *b) {
    return imp_create((a->term & b->term),
                     (a->term ^ b->term) ^ a->mask,
                     (a->is_dontcare && b->is_dontcare));
}

void imp_set_combined(Implicant *a, Implicant *b) {
    if(a) a->is_combined = true;
    if(b) b->is_combined = true;
}

void imp_print(const Implicant *a) { // %*b format specifier works only in gcc compiler
    printf("<term: %-2d (%08b) | mask: %-2d (%08b) | ones_count: %-2d| combined: %d | is_dontcare: %d>\n",
                a->term, a->term, a->mask, a->mask, a->ones_count, a->is_combined, a->is_dontcare);
}
