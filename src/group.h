#ifndef GROUP_H
#define GROUP_H

#include <stdlib.h>     // malloc, realloc
#include <string.h>     // memset

#include "helper.h"     // SAFE_ALLOC
#include "minterm.h"    // all(*)

#define MTGROUP_GROWTH_FACTOR 2

typedef struct __minterm_group_data__{
    Minterm    *minterms;
    size_t      capacity;
    size_t      size;
} MintermGroup;

static inline MintermGroup group_create(size_t capacity) {
    return (MintermGroup) { 
        .minterms = SAFE_ALLOC(malloc(capacity * sizeof(Minterm))), 
        .size = 0, .capacity = capacity
    };
}

static inline void group_update_capacity(MintermGroup *a, size_t newcapacity) {
    const size_t newsize = newcapacity * sizeof(Minterm);
    a->minterms = SAFE_ALLOC(realloc(a->minterms, newsize));
    if(newcapacity < a->size) a->size = newcapacity;
    a->capacity = newcapacity;
}

static inline void group_add_minterm(MintermGroup *a, const Minterm min) {
    if(a->size >= a->capacity) {
        size_t newcap = a->capacity * MTGROUP_GROWTH_FACTOR;
        group_update_capacity(a, newcap);
    } a->minterms[a->size++] = min;
}

static inline void group_clear(MintermGroup *a) {
    memset(a->minterms, 0, a->capacity * sizeof(Minterm));
    a->size = 0;
}

static inline void group_destroy(MintermGroup *a) {
    free(a->minterms); memset(a, 0, sizeof(MintermGroup));
}


static inline void group_combine(const MintermGroup *a, const MintermGroup *b, MintermGroup *c) {
    for(size_t i = 0; i < a->size; i++) {
        for(size_t j = 0; j < b->size; j++) {
            if(!mt_can_combine(&a->minterms[i], &b->minterms[j])) continue;
            mt_set_combined(&a->minterms[i], &b->minterms[j]);

            if(mt_implies(&a->minterms[i], &b->minterms[j])) continue;
            group_add_minterm(c, mt_combine(&a->minterms[i], &b->minterms[j]));
        }
    }
}

static inline void group_uncombined_terms(MintermGroup *src, MintermGroup *dest) {
    for(size_t i = 0; i < src->size; i++) {
        if(!src->minterms[i].is_combined && !src->minterms[i].is_dontcare) {
            group_add_minterm(dest, src->minterms[i]);
            src->minterms[i].is_combined = true;
        }
    }
}

static inline void group_describe(const MintermGroup *a, const char *name) {
    printf("<Group[%s]: %p -> size: %lu -> capacity: %lu>\n", 
            name, a->minterms, a->size, a->capacity);
}

static inline void group_print(const MintermGroup *a, const char* name) {
    printf("<Group[%s]: %p -> size: %lu -> capacity: %lu>\n",
            name, a->minterms, a->size, a->capacity);
    for(size_t j = 0; j < a->size; j++) mt_print(&a->minterms[j]);
}


#endif // GROUP_H