#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "../include/coverage.h"
#include "../include/group.h"
#include "../include/implicant.h"
#include "../include/parser.h"
#include "../include/helper.h"

static void test_simple_table(void)
{
    ParsedInput data = {
        .minterms = (uint16_t[]){1,2,3,4,5,6,7},
        .minterm_count = 7,
        .dontcares = NULL,
        .dontcare_count = 0,
        .variable_count = 3
    };

    ImpGroup pi = {
        .implicants = (Implicant[]){
            {.term = 1, .mask = 6},
            {.term = 2, .mask = 5},
            {.term = 4, .mask = 3},
        },
        .size = 3,
        .capacity = 3
    };

    CoverageTable ct = ct_create(pi.size, data.minterm_count, NULL);
    ct_populate(&ct, &data, &pi);

    uint8_t expected[3][7] = {
        {1,0,1,0,1,0,1},
        {0,1,1,0,0,1,1},
        {0,0,0,1,1,1,1},
    };

    for (uint8_t r = 0; r < pi.size; r++) {
        assert(
            memcmp(&ct_at(&ct, r, 0),
                   expected[r],
                   data.minterm_count) == 0
        );
    }

    ct_destroy(&ct);
}

int main(void)
{
    test_simple_table();
    TEST_OK("coverage table tests passed");
    return 0;
}
