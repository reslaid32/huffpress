#ifndef CHECKSHUM_H
#define CHECKSHUM_H

#include <stdint.h>

typedef uint64_t checksum_t;

checksum_t checksum(const char *buf, size_t len);

#endif // CHECKSHUM_H