#ifndef MINTERM_H
#define MINTERM_H

#include <stdio.h>      // printf
#include <stdint.h>     // uint16_t, uint8_t
#include <stdbool.h>    // bool, true, false

#define DEFAULT_MASK 0x0000

static inline unsigned mt_popcount(uint16_t n) {
    return __builtin_popcount(n); // only works in gcc compiler
}

typedef struct __minterm_data__ {
    uint16_t    term;
    uint16_t    mask;
    uint8_t     ones_count;
    bool        is_combined;
    bool        is_dontcare;
} Minterm;

static inline Minterm mt_create(uint16_t term, uint16_t mask, bool is_dontcare) {
    return (Minterm) {
        .term = term, .mask = mask, .ones_count = mt_popcount(term), 
        .is_combined = false, .is_dontcare = is_dontcare,
    };
}

static inline bool mt_can_combine(const Minterm *a, const Minterm *b) {
    return mt_popcount(a->term ^ b->term) == 1 && a->mask == b->mask;
}

static inline bool mt_implies(const Minterm *a, const Minterm *b) {
    return (a->term ^ b->term) < a->mask;
}

static inline Minterm mt_combine(const Minterm *a, const Minterm *b) {
    return mt_create((a->term & b->term),
                     (a->term ^ b->term) ^ a->mask,
                     (a->is_dontcare && b->is_dontcare));
}

static inline void mt_set_combined(Minterm *a, Minterm *b) {
    if(a) a->is_combined = true;
    if(b) b->is_combined = true;
}

static inline void mt_print(const Minterm *a) { // %*b format specifier works only in gcc compiler
    printf("<term: %08b, mask: %08b, ones_count: %d, combined: %d, is_dontcare: %d>\n",
              a->term, a->mask, a->ones_count, a->is_combined, a->is_dontcare);
}


#endif // MINTERM_H