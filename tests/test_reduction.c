#include <assert.h>
#include <stdio.h>

#include "../include/coverage.h"
#include "../include/reduction.h"
#include "../include/helper.h"

static void test_essential(void)
{
    CoverageTable ct = ct_create(3, 3, (uint8_t[]){
        1,0,1,
        0,1,0,
        1,0,0
    });

    assert(reduction_remove_essential(&ct));
    assert(ct.emask[1]);
    assert(ct.rmask[1]);
    assert(ct.cmask[1]);

    ct_destroy(&ct);
}

static void test_row_dominance(void)
{
    CoverageTable ct = ct_create(3, 3, (uint8_t[]){
        1,0,1,
        1,0,1,
        0,1,1
    });

    assert(reduction_row_dominance(&ct));
    assert(ct.rmask[1]);
    assert(!ct.rmask[0]);

    ct_destroy(&ct);
}

static void test_col_dominance(void)
{
    CoverageTable ct = ct_create(3, 3, (uint8_t[]){
        1,1,0,
        0,0,1,
        1,1,0
    });

    assert(reduction_col_dominance(&ct));
    assert(ct.cmask[1]);
    assert(!ct.cmask[0]);

    ct_destroy(&ct);
}

static void test_combined(void)
{
    CoverageTable ct = ct_create(4, 4, (uint8_t[]){
        1,0,1,0,
        1,0,1,0,
        0,1,0,0,
        0,0,1,1
    });

    assert(reduction_remove_essential(&ct));
    assert(reduction_row_dominance(&ct));
    assert(reduction_remove_essential(&ct));

    assert(ct.rmask[1]);
    assert(ct.emask[0] && ct.emask[2] && ct.emask[3]);
    assert(ct.cmask[0] && ct.cmask[1] && ct.cmask[2] && ct.cmask[3]);

    ct_destroy(&ct);
}

int main(void)
{
    test_essential();
    test_row_dominance();
    test_col_dominance();
    test_combined();
    TEST_OK("reduction tests passed");
    return 0;
}
