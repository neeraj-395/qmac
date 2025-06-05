#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

typedef struct LogicExpData {
    uint16_t *included_terms;
    uint16_t  included_count;
    uint16_t *excluded_terms;
    uint16_t  excluded_count;
    uint8_t   variable_count;
} LogicExpData;

void print_usage(const char *prog_name);

uint16_t *parse_terms(char *arg, uint16_t *count);

void free_logic_expression_data(LogicExpData *td);

LogicExpData logic_expression_parser(int argc, char *const *argv);


#endif // PARSER_H
