#include <stdio.h>
#include <string.h>

#include "../include/coverage.h"
#include "../include/reduction.h"
#include "../include/dominance.h"

#define ONE_STRIDE 1

bool reduction_remove_essential(CoverageTable *ct) {
    bool found = false;
    for(uint16_t j = 0; j < ct->cols; j++) {
        if(ct->cmask[j]) continue;
        
        int count = 0, essential_row = -1;
        for(uint16_t i = 0; i < ct->rows; i++) {
            if(!ct->rmask[i] && ct_at(ct, i, j) == 1) {
                essential_row = i;
                count = count + 1;
                if(count > 1) break;
            }
        }
        
        if(count == 1) {
            found = true;
            ct->emask[essential_row] = true;
            ct->rmask[essential_row] = true;
            for (int k = 0; k < ct->cols; k++) {
                if (ct_at(ct, essential_row, k) == 1) {
                    ct->cmask[k] = true;
                }
            }
        }
    }
    return found;
}

static void evaluate_dominance(bool *mask, uint16_t A, uint16_t B, Dominance result, bool *found) {
    switch(result) {
        case NO_DOMINANCE: return;
        case A_DOMINATES:  mask[B] = true; break;
        case B_DOMINATES:  mask[A] = true; break;
        case EQUAL:        mask[(A > B) ? A : B] = true; break;
        default:           return;
    } (*found) = true;
}

bool reduction_row_dominance(CoverageTable *ct) {
    bool found = false;
    for(uint16_t i = 0; i < ct->rows; i++) {
        if(ct->rmask[i]) continue;
        for(uint16_t k = 0; k < ct->rows; k++) {
            if(k == i || ct->rmask[k]) continue;
            evaluate_dominance(ct->rmask, i, k, check_dominance(
                &ct_at(ct, i, 0), &ct_at(ct, k, 0), 
                ct->cmask, ct->cols, ONE_STRIDE
            ), &found);
        }
    }
    return found;
}

bool reduction_col_dominance(CoverageTable *ct) {
    bool found = false;
    for (uint16_t j = 0; j < ct->cols; j++) {
        if (ct->cmask[j]) continue;
        for (uint16_t k = 0; k < ct->cols; k++) {
            if (k == j || ct->cmask[k]) continue;
            evaluate_dominance(ct->cmask, j, k, check_dominance(
                &ct_at(ct, 0, j), &ct_at(ct, 0, k), 
                ct->rmask, ct->rows, ct->cols
            ), &found);
        }
    }
    return found;
}