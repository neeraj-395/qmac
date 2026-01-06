#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../include/coverage.h"
#include "../include/reduction.h"
#include "../include/dominance.h"

bool reduction_remove_essential(CoverageTable *ct)
{
    bool found = false;

    for (uint16_t col = 0; col < ct->cols; col++) {
        if (ct->cmask[col]) continue;

        int count = 0;
        int essential_row = -1;

        for (uint16_t row = 0; row < ct->rows; row++) {
            if (ct->rmask[row]) continue;
            if (ct_at(ct, row, col)) {
                essential_row = row;
                if (++count > 1) break;
            }
        }

        if (count == 1 && essential_row >= 0) {
            found = true;
            ct->emask[essential_row] = true;
            ct->rmask[essential_row] = true;

            for (uint16_t k = 0; k < ct->cols; k++) {
                if (ct_at(ct, essential_row, k))
                    ct->cmask[k] = true;
            }
        }
    }

    return found;
}

bool reduction_row_dominance(CoverageTable *ct)
{
    bool found = false;
    bool mark[ct->rows];
    memset(mark, 0, sizeof mark);

    for (uint16_t i = 0; i < ct->rows; i++) {
        if (ct->rmask[i] || ct->emask[i]) continue;

        for (uint16_t j = i + 1; j < ct->rows; j++) {
            if (ct->rmask[j] || ct->emask[j]) continue;

            Dominance d = check_dominance(
                &ct_at(ct, i, 0),
                &ct_at(ct, j, 0),
                ct->cmask,
                ct->cols,
                1
            );

            if (d == A_DOMINATES) {
                mark[j] = true;
                found = true;
            } else if (d == B_DOMINATES) {
                mark[i] = true;
                found = true;
            } else if (d == EQUAL) {
                mark[j] = true;
                found = true;
            }
        }
    }

    for (uint16_t i = 0; i < ct->rows; i++)
        if (mark[i]) ct->rmask[i] = true;

    return found;
}

bool reduction_col_dominance(CoverageTable *ct)
{
    bool found = false;
    bool mark[ct->cols];
    memset(mark, 0, sizeof mark);

    for (uint16_t i = 0; i < ct->cols; i++) {
        if (ct->cmask[i]) continue;

        for (uint16_t j = i + 1; j < ct->cols; j++) {
            if (ct->cmask[j]) continue;

            Dominance d = check_dominance(
                &ct_at(ct, 0, i),
                &ct_at(ct, 0, j),
                ct->rmask,
                ct->rows,
                ct->cols
            );

            if (d == A_DOMINATES) {
                mark[j] = true;
                found = true;
            } else if (d == B_DOMINATES) {
                mark[i] = true;
                found = true;
            } else if (d == EQUAL) {
                mark[j] = true;
                found = true;
            }
        }
    }

    for (uint16_t i = 0; i < ct->cols; i++)
        if (mark[i]) ct->cmask[i] = true;

    return found;
}
