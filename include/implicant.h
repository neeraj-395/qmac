#ifndef IMPLICANT_H
#define IMPLICANT_H

#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_MASK 0x0000

typedef struct __implicant__ {
    uint16_t    term;
    uint16_t    mask;
    uint8_t     ones_count;
    bool        is_combined;
    bool        is_dontcare;
} Implicant;

Implicant imp_create(uint16_t term, uint16_t mask, bool is_dontcare);

bool imp_can_combine(const Implicant *a, const Implicant *b);

bool imp_implies(const Implicant *a, const Implicant *b);

Implicant imp_combine(const Implicant *a, const Implicant *b);

void imp_set_combined(Implicant *a, Implicant *b);

void imp_print(const Implicant *a);

#endif // IMPLICANT_H