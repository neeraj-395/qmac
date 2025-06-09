#include <stdio.h>      // printf
#include <stdlib.h>     // malloc, realloc, exit
#include <string.h>     // strtok, strlen
#include <getopt.h>     // getopt, optarg

#include "../include/helper.h"     // SAFE_ALLOC
#include "../include/parser.h"     // function prototypes

void print_usage(const char *prog_name) {
    printf("Usage: %s -v <varsize> -m <m-terms> -d <d-terms>\n", prog_name);
    printf("Example: %s -v 3 -m 1,2,3 -d 4,5,6\n", prog_name);
}

uint16_t *parse_terms(char *arg, uint16_t *count) {
    int len = strlen(arg);
    char *tok = strtok(arg, ",");
    uint16_t *terms = SAFE_ALLOC(malloc(len * sizeof(uint16_t)));
    while (tok && *count < len) {
        terms[(*count)++] = (uint16_t)atoi(tok);
        tok = strtok(NULL, ",");
    }
    return SAFE_ALLOC(realloc(terms, (*count) * sizeof(uint16_t)));
}

ParsedInput input_parser(int argc, char *const *argv) {
    uint8_t varsize = 0;
    char *m_arg = NULL, *d_arg = NULL;
    int opt = getopt(argc, argv, "v:m:d");

    do {
        switch (opt) {
            case 'v': varsize = (uint8_t)atoi(optarg); 
                      break;
            case 'm': m_arg = optarg;
                      break;
            case 'd': d_arg = optarg; 
                      break;
            default:  print_usage(argv[0]);
                      exit(EXIT_FAILURE);
        }
    } while((opt = getopt(argc, argv, "v:m:d:")) != -1);

    uint16_t incount = 0, excount = 0;
    uint16_t *interms = (m_arg) ? parse_terms(m_arg, &incount) : NULL;
    uint16_t *exterms = (d_arg) ? parse_terms(d_arg, &excount) : NULL;

    return (ParsedInput) {
        .included_terms = interms, .included_count = incount,
        .excluded_terms = exterms, .excluded_count = excount,
        .variable_count = varsize
    };
}

void free_parsed_data(ParsedInput *td) {
    free(td->included_terms);
    free(td->excluded_terms);
    memset(td, 0, sizeof(*td));
}
