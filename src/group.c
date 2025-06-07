#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/helper.h"
#include "../include/implicant.h"
#include "../include/group.h"

ImpGroup group_create(size_t capacity) {
    return (ImpGroup) { 
        .implicants = SAFE_ALLOC(malloc(capacity * sizeof(Implicant))), 
        .size = 0, .capacity = capacity
    };
}

void group_update_capacity(ImpGroup *a, size_t newcapacity) {
    const size_t newsize = newcapacity * sizeof(Implicant);
    a->implicants = SAFE_ALLOC(realloc(a->implicants, newsize));
    if(newcapacity < a->size) a->size = newcapacity;
    a->capacity = newcapacity;
}

void group_add_minterm(ImpGroup *a, const Implicant min) {
    if(a->size >= a->capacity) {
        size_t newcap = a->capacity * IG_GROWTH_FACTOR;
        group_update_capacity(a, newcap);
    } 
    a->implicants[a->size++] = min;
}

void group_clear(ImpGroup *a) {
    memset(a->implicants, 0, a->capacity * sizeof(Implicant));
    a->size = 0;
}

void group_destroy(ImpGroup *a) {
    free(a->implicants); 
    memset(a, 0, sizeof(ImpGroup));
}

void group_combine(const ImpGroup *a, const ImpGroup *b, ImpGroup *c) {
    for(size_t i = 0; i < a->size; i++) {
        for(size_t j = 0; j < b->size; j++) {
            if(!imp_can_combine(&a->implicants[i], &b->implicants[j])) continue;
            imp_set_combined(&a->implicants[i], &b->implicants[j]);

            if(imp_implies(&a->implicants[i], &b->implicants[j])) continue;
            group_add_minterm(c, imp_combine(&a->implicants[i], &b->implicants[j]));
        }
    }
}

void group_uncombined_terms(ImpGroup *src, ImpGroup *dest) {
    for(size_t i = 0; i < src->size; i++) {
        if(!src->implicants[i].is_combined && !src->implicants[i].is_dontcare) {
            group_add_minterm(dest, src->implicants[i]);
            src->implicants[i].is_combined = true;
        }
    }
}

void group_describe(const ImpGroup *a, const char *name) {
    printf("<Group[%s]: %p -> size: %lu -> capacity: %lu>\n",
            name, a->implicants, a->size, a->capacity);
}

void group_print(const ImpGroup *a, const char* name) {
    printf("<Group[%s]: %p -> size: %lu -> capacity: %lu>\n",
            name, a->implicants, a->size, a->capacity);
    for(size_t j = 0; j < a->size; j++) imp_print(&a->implicants[j]);
}