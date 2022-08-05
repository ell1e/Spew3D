

#ifndef SPEW3D_STRING_H_
#define SPEW3D_STRING_H_

#include <stdint.h>


static inline uint16_t spew3d_simplehash(const char *k) {
    uint16_t hash = 0;

    while (*k != '\0') {
        uint16_t c = *((uint8_t*)k);
        hash = ((hash << 5) + hash) ^ c;
    }

    return hash;
}

#endif  // SPEW3D_STRING_H_
