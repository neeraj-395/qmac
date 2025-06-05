#include "utils.h"
#include "group.h"
#include "parser.h"

int main(int argc, char *const *argv) {
    LogicExpData min = logic_expression_parser(argc, argv);

    MintermGroup sub_groups[min.variable_count + 1];

    for(size_t i = 0; i <= min.variable_count; i++) {
        sub_groups[i] = group_create(min.variable_count);
    }

    for (size_t i = 0; i < min.included_count; i++) {
        Minterm in = mt_create(min.included_terms[i], DEFAULT_MASK, false);
        group_add_minterm(&sub_groups[in.ones_count], in);
    }

    for (size_t i = 0; i < min.excluded_count; i++) {
        Minterm ex = mt_create(min.excluded_terms[i], DEFAULT_MASK, true);
        group_add_minterm(&sub_groups[ex.ones_count], ex);
    }

    int iteration = 1;
    uint16_t maxcomb = permutation_with_choices(min.variable_count, iteration, BINARY_CHOICE);
    MintermGroup container = group_create(maxcomb);
    MintermGroup prime_implicants = group_create(min.variable_count);

    while(true) {
        for(int i = 0; i <= min.variable_count; i++) {
            if(i >= min.variable_count) {
                group_clear(&sub_groups[i]);
                continue;
            }

            if(!sub_groups[i].size) break;
            if(sub_groups[i + 1].size > 0) {
                group_combine(&sub_groups[i], &sub_groups[i + 1], &container);
            }

            group_uncombined_terms(&sub_groups[i], &prime_implicants);
            group_clear(&sub_groups[i]);
        }

        if(container.size == 0) break;
        for(size_t i = 0; i < container.size; i++) {
            group_add_minterm(
                &sub_groups[container.minterms[i].ones_count],
                container.minterms[i]
            );
        }

        group_clear(&container);
        iteration++;

        maxcomb = permutation_with_choices(min.variable_count, iteration, BINARY_CHOICE);
        if(maxcomb < container.capacity) continue;
        group_update_capacity(&container, maxcomb);
    }

    for(size_t i = 0; i < prime_implicants.size; i++) {
        mt_print(&prime_implicants.minterms[i]);
    }

    // Clearn Up -> subgroups, container, prime_implicants, min
    for(uint8_t i = 0; i <= min.variable_count; i++) {
        group_destroy(&sub_groups[i]);
    }

    group_destroy(&container);
    group_destroy(&prime_implicants);
    free_logic_expression_data(&min);

    return 0;
}