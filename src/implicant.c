#include <stdio.h>

#include "../include/helper.h"
#include "../include/implicant.h"

Implicant imp_create(uint16_t term, uint16_t mask, bool is_dontcare) {
    return (Implicant) {
        .term = term, .mask = mask, 
        .ones_count = popcount(term & ~mask), 
        .is_combined = false, .is_dontcare = is_dontcare,
    };
}

bool imp_can_combine(const Implicant *a, const Implicant *b) {
    if(a->mask != b->mask) return false;
    return popcount(a->term ^ b->term) == 1 && !((a->term ^ b->term) & a->mask);
}

bool imp_implies(const Implicant *a, const Implicant *b) {
    return (a->term & ~a->mask) == (b->term & ~a->mask);
}

Implicant imp_combine(const Implicant *a, const Implicant *b) {
    return imp_create(
        a->term & b->term,
        a->mask | (a->term ^ b->term),
        a->is_dontcare && b->is_dontcare
    );
}

void imp_set_combined(Implicant *a, Implicant *b) {
    if(a) a->is_combined = true;
    if(b) b->is_combined = true;
}

void imp_print(const Implicant *a) {
    printf(
        "<term: %-2d| mask: %-2d| ones_count: %-2d| combined: %d | is_dontcare: %d>\n", 
        a->term,
        a->mask,
        a->ones_count,
        a->is_combined,
        a->is_dontcare
    );
}
