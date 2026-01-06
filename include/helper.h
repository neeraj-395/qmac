#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>

static inline void *__safe_alloc__(void *ptr, const char *file, int line) {
    if (!ptr) {
        fprintf(stderr, "Memory allocation failed at %s:%d\n", file, line);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

#define SAFE_ALLOC(expr) __safe_alloc__((expr), __FILE__, __LINE__)

#define DIE(message) do { \
    fprintf(stderr, "[ERROR] %s\n", message); exit(EXIT_FAILURE); \
} while(0)

#define WARN(message) do { \
    fprintf(stderr, "[WARN] %s\n", message); \
} while(0)

#endif