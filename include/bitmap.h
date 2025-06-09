#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <stdbool.h>

void bitmap_set(uint8_t *bitmap, uint16_t value) {
    bitmap[value >> 3] |= (1 << (value & 0x07));
}

bool bitmap_get(uint8_t *bitmap, uint16_t value) {
    return (bitmap[value >> 3] >> (value & 0x07)) & 1;
}

#endif // BITMAP_H