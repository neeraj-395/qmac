#include "../include/utils.h"
#include "../include/parser.h"
#include "../include/implicant.h"
#include "../include/group.h"

int main(int argc, char *const *argv) {
    ParsedInput data = input_parser(argc, argv);

    ImpGroup sub_groups[data.variable_count + 1];

    for(size_t i = 0; i <= data.variable_count; i++) {
        sub_groups[i] = group_create(data.variable_count);
    }

    for (size_t i = 0; i < data.included_count; i++) {
        Implicant in = imp_create(data.included_terms[i], DEFAULT_MASK, false);
        group_add_minterm(&sub_groups[in.ones_count], in);
    }

    for (size_t i = 0; i < data.excluded_count; i++) {
        Implicant ex = imp_create(data.excluded_terms[i], DEFAULT_MASK, true);
        group_add_minterm(&sub_groups[ex.ones_count], ex);
    }

    uint8_t iteration = 1;
    uint32_t maxcomb = max_combination(data.variable_count, iteration);
    ImpGroup container = group_create(maxcomb);
    ImpGroup prime = group_create(data.variable_count);

    while(true) {
        for(uint8_t i = 0; i <= data.variable_count; i++) {
            if(i >= data.variable_count) {
                group_clear(&sub_groups[i]);
                continue;
            }

            if(!sub_groups[i].size) continue;
            if(sub_groups[i + 1].size > 0) {
                group_combine(&sub_groups[i], &sub_groups[i + 1], &container);
            }

            group_uncombined_terms(&sub_groups[i], &prime);
            group_clear(&sub_groups[i]);
        }

        if(container.size == 0) break;
        for(size_t i = 0; i < container.size; i++) {
            group_add_minterm(
                &sub_groups[container.implicants[i].ones_count],
                container.implicants[i]
            );
        }

        group_clear(&container);
        iteration++;

        maxcomb = max_combination(data.variable_count, iteration);
        if(maxcomb < container.capacity) continue;
        group_update_capacity(&container, maxcomb);
    }

    for(size_t i = 0; i < prime.size; i++) {
        imp_print(&prime.implicants[i]);
    }

    // Clearn Up -> subgroups, container, prime, data
    for(uint8_t i = 0; i <= data.variable_count; i++) {
        group_destroy(&sub_groups[i]);
    }

    group_destroy(&container);
    group_destroy(&prime);
    free_parsed_data(&data);

    return 0;
}