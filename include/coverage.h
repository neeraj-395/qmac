#ifndef COVERAGE_H
#define COVERAGE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct __implicant_coverage__ {
    uint16_t *mins;
    uint16_t count;
} ImpCoverage;

typedef struct __coverage_table__ {
    uint8_t     **table;
    bool         *rmask;
    bool         *cmask;
    uint16_t      rows;
    uint16_t      cols;
} CoverageTable;

uint16_t coverage_next(uint16_t term, uint16_t *mask);

bool coverage_set(uint16_t term, uint16_t mask, ImpCoverage *result);

#endif // COVERAGE_H