#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>      // fprintf
#include <stdlib.h>     // exit

static inline void *__safe_alloc__(void *ptr, const char *file, int line) {
    if (!ptr) {
        fprintf(stderr, "Memory allocation failed at %s:%d\n", file, line);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

#define SAFE_ALLOC(expr) __safe_alloc__((expr), __FILE__, __LINE__)

#endif // HELPER_H