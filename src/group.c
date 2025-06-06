#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/helper.h"
#include "../include/minterm.h"
#include "../include/group.h"

MintermGroup group_create(size_t capacity) {
    return (MintermGroup) { 
        .minterms = SAFE_ALLOC(malloc(capacity * sizeof(Minterm))), 
        .size = 0, .capacity = capacity
    };
}

void group_update_capacity(MintermGroup *a, size_t newcapacity) {
    const size_t newsize = newcapacity * sizeof(Minterm);
    a->minterms = SAFE_ALLOC(realloc(a->minterms, newsize));
    if(newcapacity < a->size) a->size = newcapacity;
    a->capacity = newcapacity;
}

void group_add_minterm(MintermGroup *a, const Minterm min) {
    if(a->size >= a->capacity) {
        size_t newcap = a->capacity * MTGROUP_GROWTH_FACTOR;
        group_update_capacity(a, newcap);
    } 
    a->minterms[a->size++] = min;
}

void group_clear(MintermGroup *a) {
    memset(a->minterms, 0, a->capacity * sizeof(Minterm));
    a->size = 0;
}

void group_destroy(MintermGroup *a) {
    free(a->minterms); 
    memset(a, 0, sizeof(MintermGroup));
}

void group_combine(const MintermGroup *a, const MintermGroup *b, MintermGroup *c) {
    for(size_t i = 0; i < a->size; i++) {
        for(size_t j = 0; j < b->size; j++) {
            if(!mt_can_combine(&a->minterms[i], &b->minterms[j])) continue;
            mt_set_combined(&a->minterms[i], &b->minterms[j]);

            if(mt_implies(&a->minterms[i], &b->minterms[j])) continue;
            group_add_minterm(c, mt_combine(&a->minterms[i], &b->minterms[j]));
        }
    }
}

void group_uncombined_terms(MintermGroup *src, MintermGroup *dest) {
    for(size_t i = 0; i < src->size; i++) {
        if(!src->minterms[i].is_combined && !src->minterms[i].is_dontcare) {
            group_add_minterm(dest, src->minterms[i]);
            src->minterms[i].is_combined = true;
        }
    }
}

void group_describe(const MintermGroup *a, const char *name) {
    printf("<Group[%s]: %p -> size: %lu -> capacity: %lu>\n",
            name, a->minterms, a->size, a->capacity);
}

void group_print(const MintermGroup *a, const char* name) {
    printf("<Group[%s]: %p -> size: %lu -> capacity: %lu>\n",
            name, a->minterms, a->size, a->capacity);
    for(size_t j = 0; j < a->size; j++) mt_print(&a->minterms[j]);
}