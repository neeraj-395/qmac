#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../include/utils.h"
#include "../include/coverage.h"
#include "../include/parser.h"
#include "../include/group.h"
#include "../include/implicant.h"
#include "../include/reduction.h"

void test_coverage_table_simple() {
    ParsedInput data = {
        .included_terms = (uint16_t[]) {1, 2, 3, 4, 5, 6, 7},
        .included_count = 7,
        .excluded_terms = NULL,
        .excluded_count = 0,
        .variable_count = 3
    };

    ImpGroup group = {
        .implicants = (Implicant[]) {
            { .term = 1, .mask = 6 },
            { .term = 2, .mask = 5 },
            { .term = 4, .mask = 3 },
        },
        .size = 3
    };

    group_print(&group, "example");

    CoverageTable ct = ct_create(group.size, data.included_count, NULL);
    ct_populate(&ct, &data, &group);

    ct_print(&ct);

    uint8_t expected[3][7] = {
        /* A */ { 1,  0,  1,  0,  1,  0,  1},
        /* B */ { 0,  1,  1,  0,  0,  1,  1},
        /* C */ { 0,  0,  0,  1,  1,  1,  1},
    };

    for (uint8_t row = 0; row < group.size; row++) {
        assert(memcmp(&ct_at(&ct, row, 0), expected[row], sizeof(uint8_t) * data.included_count) == 0);
    }

    ct_destroy(&ct);
}

int main(void) {
    test_coverage_table_simple();
    printf("Coverage table tests passed \t\\[^_^]/\n");
    return 0;
}
