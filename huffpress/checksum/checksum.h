#ifndef CHECKSHUM_H
#define CHECKSHUM_H

#include "export.h"

#include <stdint.h>

// "unknown type name 'size_t'" fix
#include <stddef.h>

typedef uint64_t checksum_t;

CHECKSUM_API checksum_t checksum(const char *buf, size_t len);

#endif // CHECKSHUM_H