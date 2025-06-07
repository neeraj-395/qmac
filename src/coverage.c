#include "../include/helper.h"
#include "../include/parser.h"
#include "../include/group.h"
#include "../include/coverage.h"

CoverageTable ct_create(uint16_t min_count, uint16_t pi_count) {
    uint8_t **table = SAFE_ALLOC(malloc(sizeof(uint8_t *) * min_count));
    for(uint16_t i = 0; i < min_count; i++) {
        table[i] = SAFE_ALLOC(calloc(pi_count, sizeof(uint8_t)));
    }

    return (CoverageTable) {
        .table = table, .rows = min_count, .cols = pi_count,
        .rmask = SAFE_ALLOC(calloc(min_count, sizeof(bool))),
        .cmask = SAFE_ALLOC(calloc(pi_count, sizeof(bool)))
    };
}

uint16_t coverage_next(uint16_t term, uint16_t *mask) {
    uint16_t isolated_bit = (*mask & -*mask);
    *mask &= (*mask - 1);
    return term + isolated_bit;
}

bool coverage_set(uint16_t term, uint16_t mask, ImpCoverage *result) {
    if(!mask) return false; 
    result->mins[0] = term;
    result->mins[result->count - 1] = mask;
    
    for(uint16_t i = 1; i < result->count - 1; i++) {
        result->mins[i] = coverage_next(term, &mask);
    } return true;
}

void ct_populate(const ParsedInput *data, ImpGroup *pi) {
    for(uint16_t i = 0; i < data->included_count; i++) {
        // TODO: program a coverate table populator
    }
}

