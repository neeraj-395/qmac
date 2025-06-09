#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/coverage.h"
#include "../include/reduction.h"
#include "../include/dominance.h"

void test_essential_removal() {
    CoverageTable ct1 = ct_create(3, 3, (uint8_t[]) {
        1, 0, 1,
        0 ,1, 0,
        1, 0, 0, // Essential Implicant
    });

    assert(reduction_remove_essential(&ct1));
    assert(ct1.emask[1] == true);
    assert(ct1.rmask[1] == true);
    assert(ct1.cmask[1] == true);
    ct_destroy(&ct1);

    CoverageTable ct2 = ct_create(3, 3, (uint8_t[]) {
        1, 1, 0,
        1 ,0, 1, // No Essential Implicant
        0, 1, 1,
    });

    assert(!reduction_remove_essential(&ct2));
    assert(!ct2.emask[0] && !ct2.emask[1] && !ct2.emask[2]);
    ct_destroy(&ct2);  
}

void test_row_dominance() {
    CoverageTable ct1 = ct_create(3, 3, (uint8_t[]) {
        1, 0, 1, // P0
        1, 0, 1, // P1: identical to P0
        0, 1, 1, // P2: different
    });

    assert(reduction_row_dominance(&ct1)); // failed
    assert(ct1.rmask[1] == true);
    assert(ct1.rmask[0] == false);
    ct_destroy(&ct1);

    CoverageTable ct2 = ct_create(3, 3, (uint8_t[]) {
        1, 1, 1,  // P0: dominates P1
        1, 0, 1,  // P1: dominated by P0
        0, 1, 0,   // P2: different
    });

    assert(reduction_row_dominance(&ct2));
    assert(ct2.rmask[1] == true);
    assert(ct2.rmask[0] == false);
    ct_destroy(&ct2);

    CoverageTable ct3 = ct_create(3, 3, (uint8_t[]) {
        1, 0, 1,  // P0
        0, 1, 1,  // P1: no dominance
        1, 1, 0,   // P2: no dominance
    });

    assert(!reduction_row_dominance(&ct3));
    assert(!ct3.rmask[0] && !ct3.rmask[1] && !ct3.rmask[2]);
    ct_destroy(&ct3);
}

void test_col_dominance() {
    // Test case 1: Identical columns
    CoverageTable ct1 = ct_create(3, 3, (uint8_t[]) {
        1, 1, 0,  // P0
        0, 0, 1,  // P1
        1, 1, 0,   // P2
    });

    assert(reduction_col_dominance(&ct1));
    assert(ct1.cmask[1] == true);
    assert(ct1.cmask[0] == false);
    ct_destroy(&ct1);

    // Test case 2: Dominating columns
    CoverageTable ct2 = ct_create(3, 3, (uint8_t[]) {
        1, 1, 0,  // P0
        1, 0, 1,  // P1
        1, 1, 0,  // P2
    });

    assert(reduction_col_dominance(&ct2));
    assert(ct2.cmask[1] == true);
    assert(ct2.cmask[0] == false);
    ct_destroy(&ct2);
}

void test_combined_reduction() {
    CoverageTable ct = ct_create(4, 4, (uint8_t[]) {
        1, 0, 1, 0,   // P0
        1, 0, 1, 0,   // P1: identical to P0
        0, 1, 0, 0,   // P2: essential for C1
        0, 0, 1, 1,   // P3
    });

    assert(reduction_remove_essential(&ct));
    assert(reduction_row_dominance(&ct));
    assert(reduction_remove_essential(&ct));
    assert(ct.rmask[1] == true);
    assert(ct.emask[0] && ct.emask[2] && ct.emask[3]);
    assert(ct.cmask[0] && ct.cmask[1] && ct.cmask[2] && ct.cmask[3]);
    ct_destroy(&ct);
}

int main() {
    test_row_dominance();
    test_col_dominance();
    test_essential_removal();
    test_combined_reduction();

    printf("All reduction tests passed \t\\[^_^]/\n");
}