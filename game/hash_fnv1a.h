#ifndef HASH_FNV1A_H
#define HASH_FNV1A_H

#include <stddef.h>
#include <stdint.h>

uint32_t
hash_fnv1a(const void *data,
           size_t len);

uint32_t
hash_fnv1a_str(const char *str);

#endif
