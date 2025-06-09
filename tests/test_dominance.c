#include <stdio.h>
#include <assert.h>
#include "../include/dominance.h"
#include "../include/coverage.h"

int main() {
    uint8_t A1[] = {1, 0};  // covers m0
    uint8_t B1[] = {1, 0};  // equal
    assert(check_dominance(A1, B1, NULL, 2, 1) == EQUAL);

    uint8_t A2[] = {1, 1};
    uint8_t B2[] = {1, 0};  // A covers more than B
    assert(check_dominance(A2, B2, NULL, 2, 1) == A_DOMINATES);

    uint8_t A3[] = {0, 1};
    uint8_t B3[] = {1, 1};  // B covers more
    assert(check_dominance(A3, B3, NULL, 2, 1) == B_DOMINATES);

    uint8_t A4[] = {0, 1};
    uint8_t B4[] = {1, 0};  // No dominance
    assert(check_dominance(A4, B4, NULL, 2, 1) == NO_DOMINANCE);

    // With a mask (ignore second bit)
    uint8_t A5[] = {1, 1};
    uint8_t B5[] = {1, 0};
    bool mask[] = {false, true}; // ignore index 1
    assert(check_dominance(A5, B5, mask, 2, 1) == EQUAL);

    // Longer vector with stride
    CoverageTable ct = ct_create(2, 6, (uint8_t[]) {
        0, 9, 0, 8, 1, 7,
        0, 2, 0, 6, 1, 5,
    });

    assert(check_dominance(&ct_at(&ct, 0, 0), &ct_at(&ct, 0, 2), NULL, 2, 6) == EQUAL);

    ct_destroy(&ct);

    printf("All dominance tests passed \t\\[^_^]/\n");

    return 0;
}