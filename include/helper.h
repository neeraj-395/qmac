#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static inline void *__safe_alloc__(void *ptr, const char *file, int line) {
    if (!ptr) {
        fprintf(stderr, "Memory allocation failed at %s:%d\n", file, line);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

#define SAFE_ALLOC(expr) __safe_alloc__((expr), __FILE__, __LINE__)

#define TEST_OK(name) printf("%-36s ....... \\[^_^]/\n", name)

#define DIE(message) do { \
    fprintf(stderr, "[ERROR] %s\n", message); exit(EXIT_FAILURE); \
} while(0)

#define WARN(message) do { \
    fprintf(stderr, "[WARN] %s\n", message); \
} while(0)

uint16_t nCr(uint8_t n, uint8_t r);
unsigned popcount(unsigned n);
uint32_t max_combination(uint8_t var_count, uint8_t minimization_level);

#endif