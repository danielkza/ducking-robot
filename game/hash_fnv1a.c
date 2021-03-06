#include <stddef.h>
#include <stdint.h>
#include <limits.h>

#include <assert.h>

#include "hash_fnv1a.h"

#define FNV1A_32BIT_PRIME 16777619u
#define FNV1A_32BIT_OFFSET 2166136261u

uint32_t
hash_fnv1a(const void *data,
           size_t len)
{
    unsigned char *b_data = (unsigned char *)data;
    uint32_t hash;
    size_t pos;

    assert(b_data != NULL);
    
    hash = FNV1A_32BIT_OFFSET;
    for(pos = 0; pos < len; pos++) {
        hash ^= b_data[pos];
        hash *= FNV1A_32BIT_PRIME;
    }

    return hash;
}

uint32_t
hash_fnv1a_str(const char *str)
{
    uint32_t hash;
    const char *pos;

    assert(str != NULL);
    
    for(pos = str; *pos != '\0'; pos++) {
        hash ^= *pos;
        hash *= FNV1A_32BIT_PRIME;
    }

    return hash;
}
