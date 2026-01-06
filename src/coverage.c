#include <string.h>

#include "../include/helper.h"
#include "../include/bitmap.h"
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
            for(int k = 0; k < covsize; k++) bitmap_set(bmap, coverage[k]);
        } else bitmap_set(bmap, imp.term);

        for (size_t j = 0; j < data->minterm_count; j++) {
            if (bitmap_get(bmap, data->minterms[j])) {
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


bool coverage_populate(const Implicant *imp, uint16_t *out)
{
    if (!imp->mask) {
        out[0] = imp->term;
        return false;
    }

    uint16_t mask = imp->mask;
    uint8_t pos[16];
    uint8_t k = 0;

    for (uint8_t i = 0; i < 16; i++) {
        if (mask & (1u << i))
            pos[k++] = i;
    }

    uint16_t count = 1u << k;

    for (uint16_t i = 0; i < count; i++) {
        uint16_t t = imp->term;
        for (uint8_t j = 0; j < k; j++) {
            if (i & (1u << j))
                t |= (1u << pos[j]);
        }
        out[i] = t;
    }

    return true;
}
