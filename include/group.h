#ifndef GROUP_H
#define GROUP_H

#include <stddef.h>

#define IG_GROWTH_FACTOR 2

typedef struct __implicant__ Implicant;

typedef struct __implicant_group__{
    Implicant  *implicants;
    size_t      capacity;
    size_t      size;
} ImpGroup;

ImpGroup group_create(size_t capacity);
void group_update_capacity(ImpGroup *a, size_t newcapacity);
void group_add_minterm(ImpGroup *a, const Implicant min);
void group_clear(ImpGroup *a);
void group_destroy(ImpGroup *a);
void group_combine(const ImpGroup *a, const ImpGroup *b, ImpGroup *c);
void group_uncombined_terms(ImpGroup *src, ImpGroup *dest);
void group_describe(const ImpGroup *a, const char *name);
void group_print(const ImpGroup *a, const char* name);

#endif // GROUP_H