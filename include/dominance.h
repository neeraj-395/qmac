#include <stdbool.h>
#include <stdint.h>

typedef enum __dominance_type__ { 
    A_DOMINATES, B_DOMINATES, 
    NO_DOMINANCE, EQUAL
} Dominance;

static inline Dominance check_dominance(
    uint8_t *a_vector, uint8_t *b_vector, 
    bool *mask, uint16_t length, uint8_t stride
)
{
    bool a_strict = false, b_strict = false;
    
    for (uint16_t i = 0; i < length; i++) {
        if (mask != NULL && mask[i]) continue;
        
        uint8_t a_value = a_vector[i * stride];
        uint8_t b_value = b_vector[i * stride];
        
        if (a_value == 1 && b_value == 0) a_strict = true;
        else if (a_value == 0 && b_value == 1) b_strict = true;
    }
    
    if (!a_strict && !b_strict) return EQUAL;
    if (a_strict && !b_strict)  return A_DOMINATES;
    if (!a_strict && b_strict)  return B_DOMINATES;
    return NO_DOMINANCE;
}