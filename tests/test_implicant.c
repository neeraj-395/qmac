#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/implicant.h"
#include "../include/helper.h"

static Implicant M(uint16_t term)
{
    return imp_create(term, 0, false);
}

static void test_imp_create(void)
{
    Implicant a = imp_create(0b1011, 0b0010, false);

    assert(a.term == 0b1011);
    assert(a.mask == 0b0010);
    assert(a.ones_count == 2);
    assert(a.is_combined == false);
    assert(a.is_dontcare == false);
}

static void test_imp_can_combine(void)
{
    Implicant a = M(0b0001);
    Implicant b = M(0b0011); // differ by 1 bit → combinable
    Implicant c = M(0b0110); // differ by 3 bits → NOT combinable

    assert(imp_can_combine(&a, &b) == true);
    assert(imp_can_combine(&a, &c) == false);
}

static void test_imp_combine(void)
{
    Implicant a = M(0b0001);
    Implicant b = M(0b0011);

    Implicant c = imp_combine(&a, &b);

    assert(c.term == 0b0001);
    assert(c.mask == 0b0010);
    assert(c.ones_count == 1);
    assert(c.is_dontcare == false);
}

static void test_imp_masked_combine(void)
{
    Implicant a = imp_create(0b0001, 0b0010, false);
    Implicant b = imp_create(0b0101, 0b0010, false);

    assert(imp_can_combine(&a, &b) == true);

    Implicant c = imp_combine(&a, &b);

    assert(c.term == 0b0001);
    assert(c.mask == 0b0110);
    assert(c.ones_count == 1);
}

static void test_imp_implies(void)
{
    Implicant a = imp_create(0b0001, 0b0010, false); // 0-01
    Implicant b = imp_create(0b0001, 0b0000, false); // 0001
    Implicant c = imp_create(0b0101, 0b0000, false); // 0101

    assert(imp_implies(&a, &b) == true);
    assert(imp_implies(&a, &c) == false);
}

static void test_imp_set_combined(void)
{
    Implicant a = M(0b0001);
    Implicant b = M(0b0011);

    imp_set_combined(&a, &b);

    assert(a.is_combined == true);
    assert(b.is_combined == true);
}

int main(void)
{
    test_imp_create();
    test_imp_can_combine();
    test_imp_combine();
    test_imp_masked_combine();
    test_imp_implies();
    test_imp_set_combined();
    TEST_OK("implicant tests passed");
    return 0;
}
