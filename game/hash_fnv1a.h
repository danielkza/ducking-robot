/**
  * @file
  * Hashing utilities
  **/

#ifndef HASH_FNV1A_H
#define HASH_FNV1A_H

#include <stddef.h>
#include <stdint.h>

/**
 * Hashes arbitrary data, returning a 32-bit integer
 *
 * @param data Data to hash
 * @param len  Length of data in bytes
 *
 * @returns Hash value
 **/

uint32_t
hash_fnv1a(const void *data,
           size_t len);

/**
 * Hashes a character string
 *
 * @param str String to hash
 *
 * @returns Hash value
 **/


uint32_t
hash_fnv1a_str(const char *str);

#endif
