#include <stdio.h>

#include "../include/minterm.h"

static unsigned mt_popcount(uint16_t n) {
    return __builtin_popcount(n); // only works in gcc compiler
}

Minterm mt_create(uint16_t term, uint16_t mask, bool is_dontcare) {
    return (Minterm) {
        .term = term, .mask = mask, .ones_count = mt_popcount(term), 
        .is_combined = false, .is_dontcare = is_dontcare,
    };
}

bool mt_can_combine(const Minterm *a, const Minterm *b) {
    return mt_popcount(a->term ^ b->term) == 1 && a->mask == b->mask;
}

bool mt_implies(const Minterm *a, const Minterm *b) {
    return (a->term ^ b->term) < a->mask;
}

Minterm mt_combine(const Minterm *a, const Minterm *b) {
    return mt_create((a->term & b->term),
                     (a->term ^ b->term) ^ a->mask,
                     (a->is_dontcare && b->is_dontcare));
}

void mt_set_combined(Minterm *a, Minterm *b) {
    if(a) a->is_combined = true;
    if(b) b->is_combined = true;
}

void mt_print(const Minterm *a) { // %*b format specifier works only in gcc compiler
    printf("<term: %08b, mask: %08b, ones_count: %d, combined: %d, is_dontcare: %d>\n",
              a->term, a->mask, a->ones_count, a->is_combined, a->is_dontcare);
}
