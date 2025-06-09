#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#include "../include/implicant.h"
#include "../include/coverage.h"

void test_coverage_populate_basic() {
    Implicant a = {
        .term = 0b1000,
        .mask = 0b0110,
        .ones_count = 0,
        .is_combined = false,
        .is_dontcare = false
    };

    uint16_t result[8] = {0};
    bool ok = coverage_populate(&a, result);

    assert(ok == true);
    assert(result[0] == 8);
    assert(result[1] == 10);
    assert(result[2] == 12);
    assert(result[3] == 14);
}

void test_coverage_populate_nonzero_term() {
    Implicant a = {
        .term = 0b0100,
        .mask = 0b0001,
        .ones_count = 0,
        .is_combined = false,
        .is_dontcare = false
    };

    uint16_t result[4] = {0};
    bool ok = coverage_populate(&a, result);

    assert(ok == true);
    assert(result[0] == 0b0100);
    assert(result[1] == 0b0101);
    assert(result[2] == 0b0101);
}

void test_coverage_populate_zero_mask() {
    Implicant a = {
        .term = 5,
        .mask = 0,
        .ones_count = 0,
        .is_combined = false,
        .is_dontcare = false
    };

    uint16_t result[4] = {123, 123, 123, 123};
    bool ok = coverage_populate(&a, result);

    assert(ok == false);
    assert(result[0] == 123);
}

// ----------- Main ----------
int main() {
    test_coverage_populate_basic();
    test_coverage_populate_nonzero_term();
    test_coverage_populate_zero_mask();

    printf("Coverage populate tests passed \t\\[^_^]/\n");
    return 0;
}
