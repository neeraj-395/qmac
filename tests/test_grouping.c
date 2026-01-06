#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/group.h"
#include "../include/implicant.h"

static Implicant M(uint16_t term)
{
    return imp_create(term, 0, false);
}

static void test_group_create_destroy(void)
{
    ImpGroup g = group_create(0);

    assert(g.implicants != NULL);
    assert(g.size == 0);
    assert(g.capacity >= 1);

    group_destroy(&g);
}

static void test_group_add_and_clear(void)
{
    ImpGroup g = group_create(2);

    group_add_minterm(&g, M(1));
    group_add_minterm(&g, M(3));

    assert(g.size == 2);

    group_clear(&g);
    assert(g.size == 0);

    group_destroy(&g);
}

static void test_group_combine_simple(void)
{
    ImpGroup g1 = group_create(2);
    ImpGroup g2 = group_create(2);
    ImpGroup out = group_create(2);

    group_add_minterm(&g1, M(0b0001));
    group_add_minterm(&g2, M(0b0011));

    group_combine(&g1, &g2, &out);

    assert(out.size == 1);

    Implicant r = out.implicants[0];
    assert(r.term == 0b0001);
    assert(r.mask == 0b0010);
    assert(r.ones_count == 1);

    assert(g1.implicants[0].is_combined == true);
    assert(g2.implicants[0].is_combined == true);

    group_destroy(&g1);
    group_destroy(&g2);
    group_destroy(&out);
}

static void test_group_no_invalid_combine(void)
{
    ImpGroup g1 = group_create(2);
    ImpGroup g2 = group_create(2);
    ImpGroup out = group_create(2);

    group_add_minterm(&g1, M(0b0001));
    group_add_minterm(&g2, M(0b0110)); /* differs by >1 bit */

    group_combine(&g1, &g2, &out);

    assert(out.size == 0);
    assert(g1.implicants[0].is_combined == false);
    assert(g2.implicants[0].is_combined == false);

    group_destroy(&g1);
    group_destroy(&g2);
    group_destroy(&out);
}

static void test_group_uncombined_terms(void)
{
    ImpGroup src = group_create(3);
    ImpGroup dest = group_create(3);

    Implicant a = M(1);
    Implicant b = M(3);
    Implicant c = M(7);

    b.is_combined = true; /* simulate previous combination */

    group_add_minterm(&src, a);
    group_add_minterm(&src, b);
    group_add_minterm(&src, c);

    group_uncombined_terms(&src, &dest);

    assert(dest.size == 2);
    assert(dest.implicants[0].term == 1);
    assert(dest.implicants[1].term == 7);

    group_destroy(&src);
    group_destroy(&dest);
}

static void test_group_duplicate_elimination(void)
{
    ImpGroup g1 = group_create(2);
    ImpGroup g2 = group_create(2);
    ImpGroup out = group_create(2);

    group_add_minterm(&g1, M(0b0001));
    group_add_minterm(&g1, M(0b0101));

    group_add_minterm(&g2, M(0b0011));
    group_add_minterm(&g2, M(0b0111));

    group_combine(&g1, &g2, &out);

    assert(out.size == 2);

    group_destroy(&g1);
    group_destroy(&g2);
    group_destroy(&out);
}


int main(void)
{
    test_group_create_destroy();
    test_group_add_and_clear();
    test_group_combine_simple();
    test_group_no_invalid_combine();
    test_group_uncombined_terms();
    test_group_duplicate_elimination();

    printf("All grouping tests passed \t\\[^_^]/\n");
    return 0;
}
