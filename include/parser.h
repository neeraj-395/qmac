#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

typedef struct
{
    uint16_t    *minterms;
    uint16_t    *dontcares;
    uint16_t     minterm_count;
    uint16_t     dontcare_count;
    uint8_t      variable_count;
} ParsedInput;

ParsedInput parse_input(int argc, char *const *argv);

void free_parsed_data(ParsedInput *data);

#endif
