#ifndef COVERAGE_H
#define COVERAGE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct __implicant__ Implicant;
typedef struct __implicant_group__ ImpGroup;
typedef struct __parsed_input_data__ ParsedInput;

#define ct_at(ct_ptr, row, col) ((ct_ptr)->table[(row) * (ct_ptr)->cols + (col)])

typedef struct __coverage_table__ {
    uint8_t      *table;
    bool         *rmask;
    bool         *cmask;
    bool         *emask;
    uint16_t      rows;
    uint16_t      cols;
} CoverageTable;

CoverageTable ct_create(uint16_t rows, uint16_t cols, uint8_t data[]);

void ct_populate(CoverageTable *ct, const ParsedInput *data, ImpGroup *pi);

void ct_destroy(CoverageTable *ct);

/**
 * @brief Populates the array `result` with all minterms covered by the given implicant.
 *
 * An implicant represents a generalized minterm that may cover multiple specific minterms
 * depending on the number of don't-care bits in its `mask`. Each '1' bit in the mask represents
 * a variable that can be either 0 or 1, leading to 2^k minterms for k set bits in the mask.
 *
 * This function generates those minterms efficiently using bit manipulation.
 *
 * @param a Pointer to the Implicant structure containing:
 *          - a->term: base term (e.g., 0100)
 *          - a->mask: mask indicating don't-care bit positions (e.g., 1010)
 * @param result Pointer to a buffer of size (2^popcount(mask)) to hold the result.
 *               The buffer must be preallocated by the caller.
 * @return true if the mask is non-zero and coverage is populated,
 *         false if the mask is zero (i.e., covers only the base term).
 */
bool coverage_populate(const Implicant *a, uint16_t *result);

void ct_print(const CoverageTable *ct);

#endif // COVERAGE_H