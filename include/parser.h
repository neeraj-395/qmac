#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdint.h>

typedef struct __parsed_input_data__ {
    uint16_t *included_terms;
    size_t    included_count;
    uint16_t *excluded_terms;
    size_t    excluded_count;
    uint8_t   variable_count;
} ParsedInput;

void print_usage(const char *prog_name);

uint16_t *parse_terms(char *arg, uint16_t *count);

ParsedInput input_parser(int argc, char *const *argv);

void free_parsed_data(ParsedInput *td);


#endif // PARSER_H
