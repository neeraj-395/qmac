#include <string.h>

#include "../include/helper.h"
#include "../include/bitmap.h"
#include "../include/utils.h"
#include "../include/parser.h"
#include "../include/group.h"
#include "../include/implicant.h"
#include "../include/coverage.h"

CoverageTable ct_create(uint16_t rows, uint16_t cols, uint8_t data[]) {
    uint8_t *table = SAFE_ALLOC(calloc(rows * cols, sizeof(uint8_t)));
    if(data) memcpy(table, data, rows * cols * sizeof(uint8_t));

    return (CoverageTable) {
        .table = table, .rows = rows, .cols = cols,
        .rmask = SAFE_ALLOC(calloc(rows, sizeof(bool))),
        .cmask = SAFE_ALLOC(calloc(cols, sizeof(bool))),
        .emask = SAFE_ALLOC(calloc(rows, sizeof(bool))),
    };
}

void ct_populate(CoverageTable *ct, const ParsedInput *data, ImpGroup *pi) {
    size_t bitmap_size = (1 << data->variable_count) / 8;
    uint8_t bmap[bitmap_size + 1];

    for(size_t i = 0; i < pi->size; i++) {
        memset(bmap, 0, bitmap_size);

        Implicant imp = pi->implicants[i];
        const int covsize = 1 << popcount(imp.mask);
        uint16_t coverage[covsize + 1];

        if(coverage_populate(&imp, coverage)) {
            for(int k = 0; k < covsize; k++) 
               bitmap_set(bmap, coverage[k]);
        } else bitmap_set(bmap, imp.term);

        for (size_t j = 0; j < data->included_count; j++) {
            if (bitmap_get(bmap, data->included_terms[j])) {
                ct_at(ct, i , j) = 1;
            }
        }
    }
}

void ct_destroy(CoverageTable *ct) {
    if(!ct) return;
    if(ct->table) free(ct->table);
    if(ct->cmask) free(ct->cmask);
    if(ct->rmask) free(ct->rmask);
    if(ct->emask) free(ct->emask);
    memset(ct, 0, sizeof(CoverageTable));
}

void ct_print(const CoverageTable *ct) {
    printf("\nCoverage Table:\n     ");
    for (int j = 0; j < ct->cols; j++) {
        if (!ct->cmask[j])
            printf("M%-2d ", j);
    } printf("\n");

    for (int i = 0; i < ct->rows; i++) {
        if (ct->rmask[i]) continue;
        printf("P%-2d%s ", i, ct->emask[i] ? "*" : " ");
        for (int j = 0; j < ct->cols; j++) {
            if (!ct->cmask[j])
                printf(" %d  ", ct_at(ct, i, j));
        }
        printf("\n");
    } printf("\n");
}


bool coverage_populate(const Implicant *a, uint16_t *result) {
    if (!a->mask) return false; // nothing to populate

    *(result++) = a->term; // start with the base term

    uint16_t cmask = a->mask;

    /**
     * Generate all terms by flipping one don't-care bit at a time.
     * This method walks through each set bit in the mask using Brian Kernighan's trick.
     */
    while (cmask) {
        uint16_t bit = cmask & -cmask; // isolate the lowest set bit

        *(result++) = a->term + bit; // add this varient to the result

        cmask &= (cmask - 1); // clear the lowest set bit
    }

    // Include the final term where all don't-care bits are 1
    *result = a->term + a->mask;

    return true;
}