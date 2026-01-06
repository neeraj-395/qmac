#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "../include/implicant.h"
#include "../include/coverage.h"
#include "../include/helper.h"

static void test_basic_mask_expansion(void)
{
    Implicant a = {
        .term = 0b1000,
        .mask = 0b0110,
        .is_combined = false,
        .is_dontcare = false
    };

    uint16_t out[4] = {0};
    bool ok = coverage_populate(&a, out);

    assert(ok == true);
    assert(out[0] == 8);
    assert(out[1] == 10);
    assert(out[2] == 12);
    assert(out[3] == 14);
}

static void test_single_bit_mask(void)
{
    Implicant a = {
        .term = 0b0100,
        .mask = 0b0001,
        .is_combined = false,
        .is_dontcare = false
    };

    uint16_t out[2] = {0};
    bool ok = coverage_populate(&a, out);

    assert(ok == true);
    assert(out[0] == 4);
    assert(out[1] == 5);
}

static void test_zero_mask(void)
{
    Implicant a = {
        .term = 7,
        .mask = 0,
        .is_combined = false,
        .is_dontcare = false
    };

    uint16_t out[1] = {123};
    bool ok = coverage_populate(&a, out);

    assert(ok == false);
}


int main(void)
{
    test_basic_mask_expansion();
    test_single_bit_mask();
    test_zero_mask();

    TEST_OK("coverage_populate tests passed");
    return 0;
}
