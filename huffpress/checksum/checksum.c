#include "checksum.h"

checksum_t checksum(const char *buf, size_t len) {
    uint64_t hash = 0xcbf29ce484222325;
    for (size_t i = 0; i < len; i++) {
        hash ^= (uint8_t)buf[i];
        hash *= 0x00000100000001b3;
    }
    return hash;
}