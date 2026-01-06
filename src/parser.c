#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdint.h>

#include "helper.h"
#include "parser.h"

static void usage(const char *prog)
{
    fprintf(
        stderr,
        "Usage: %s -v <1..16> -m <minterms> [-d <dontcares>]\n"
        "Example: %s -v 4 -m 1,3,7,15 -d 0,2\n",
        prog, prog
    );

    exit(EXIT_FAILURE);
}

uint16_t *parse_list(const char *arg, uint16_t *count)
{
    char *buffer = SAFE_ALLOC(strdup(arg));

    uint16_t capacity = 1;
    for (const char *ch = arg; *ch; ch++) {
        if (*ch == ',') capacity++;
    }

    uint16_t *array = SAFE_ALLOC(malloc(capacity * sizeof(uint16_t)));
    char *token = strtok(buffer, ",");

    while (token)
    {
        char *end;
        long value = strtol(token, &end, 10);

        if (*end || value < 0 || value > UINT16_MAX)
            DIE("Invalid term value");

        array[(*count)++] = (uint16_t)value;
        token = strtok(NULL, ",");
    }

    free(buffer);
    return SAFE_ALLOC(realloc(array, (*count) * sizeof(uint16_t)));
}

static ParsedInput validate(ParsedInput data)
{
    if (data.variable_count < 1 || data.variable_count > 16) {
        free_parsed_data(&data);
        DIE("Variable count must be between 1 and 16");
    }

    const uint16_t limit = (uint16_t)(1u << data.variable_count);

    for (uint16_t i = 0; i < data.minterm_count; i++) {
        if (data.minterms[i] >= limit) {
            free_parsed_data(&data);
            DIE("Minterm out of range for variable count");
        }
    }

    for (uint16_t i = 0; i < data.dontcare_count; i++) {
        if (data.dontcares[i] >= limit) {
            free_parsed_data(&data);
            DIE("Don't-care term out of range for variable count");
        }
    }

    for (uint16_t i = 0; i < data.minterm_count; i++) {
        for (uint16_t j = 0; j < data.dontcare_count; j++) {
            if (data.minterms[i] == data.dontcares[j]) {
                free_parsed_data(&data);
                DIE("Term cannot be both minterm and don't-care");
            }
        }
    }

    return data;
}

ParsedInput parse_input(int argc, char *const *argv)
{
    int opt;
    uint8_t vars = 0;
    char *m_arg = NULL, *d_arg = NULL;

    while ((opt = getopt(argc, argv, "v:m:d:")) != -1)
    {
        switch (opt)
        {
            case 'v':
                long tmp = strtol(optarg, NULL, 10);
                vars = (tmp < 0) ? 0 : (uint8_t) tmp;
                break;

            case 'm':
                m_arg = optarg;
                break;

            case 'd':
                d_arg = optarg;
                break;

            default: usage(argv[0]);
        }
    }

    if(!vars || !m_arg) usage(argv[0]);

    uint16_t m_count = 0, d_count = 0;
    uint16_t *minterms = m_arg ? parse_list(m_arg, &m_count) : NULL;
    uint16_t *dontcares = d_arg ? parse_list(d_arg, &d_count) : NULL;

    return validate((ParsedInput) {
        .minterms = minterms,
        .dontcares = dontcares,
        .minterm_count = m_count,
        .dontcare_count = d_count,
        .variable_count = vars,
    });
}

void free_parsed_data(ParsedInput *data)
{
    if(!data) return;
    free(data->minterms);
    free(data->dontcares);
    memset(data, 0, sizeof(ParsedInput));
}
