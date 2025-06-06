#ifndef GROUP_H
#define GROUP_H

#include <stddef.h>

#define MTGROUP_GROWTH_FACTOR 2

typedef struct __minterm_data__ Minterm;

typedef struct __minterm_group_data__{
    Minterm    *minterms;
    size_t      capacity;
    size_t      size;
} MintermGroup;

MintermGroup group_create(size_t capacity);

void group_update_capacity(MintermGroup *a, size_t newcapacity);

void group_add_minterm(MintermGroup *a, const Minterm min);

void group_clear(MintermGroup *a);

void group_destroy(MintermGroup *a);

void group_combine(const MintermGroup *a, const MintermGroup *b, MintermGroup *c);

void group_uncombined_terms(MintermGroup *src, MintermGroup *dest);

void group_describe(const MintermGroup *a, const char *name);

void group_print(const MintermGroup *a, const char* name);

#endif // GROUP_H