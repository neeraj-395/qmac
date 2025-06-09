#ifndef REDUCTION_H
#define REDUCTION_H

#include <stdbool.h>

typedef struct __coverage_table__ CoverageTable;

bool reduction_remove_essential(CoverageTable *ct);

bool reduction_row_dominance(CoverageTable *ct);

bool reduction_col_dominance(CoverageTable *ct);

#endif // REDUCTION_H