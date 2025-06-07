#include <stdio.h>
#include <stdint.h>
#include "../include/implicant.h"


int main() {
    Implicant a = { .term= 0, .mask = 10, .ones_count = 0, .is_dontcare = 0, .is_combined = 0};

    Coverage b = imp_coverage(a);

    printf("{");
    for(int i = 0; i < b.count; i++) {
        printf("%d,", b.mins[i]);
    } printf("}\n");

    return 0;
}
