#ifndef MINTERM_H
#define MINTERM_H

#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_MASK 0x0000

typedef struct __minterm_data__ {
    uint16_t    term;
    uint16_t    mask;
    uint8_t     ones_count;
    bool        is_combined;
    bool        is_dontcare;
} Minterm;

Minterm mt_create(uint16_t term, uint16_t mask, bool is_dontcare);

bool mt_can_combine(const Minterm *a, const Minterm *b);

bool mt_implies(const Minterm *a, const Minterm *b);

Minterm mt_combine(const Minterm *a, const Minterm *b);

void mt_set_combined(Minterm *a, Minterm *b);

void mt_print(const Minterm *a);

#endif // MINTERM_H