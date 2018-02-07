/* Copyright (c) 2011-2014, David Hauweele <david@hauweele.net>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

bool htable_str_cmp(const void *k1, const void *k2)
{
  return !strcmp(k1, k2);
}

bool htable_int_cmp(const void *k1, const void *k2)
{
  return k1 == k2;
}



/* Dan Bernstein's hash */
uint32_t hash_str_djb2(const void *key)
{
  const char *str = key;
  register uint32_t hash = 5381;
  int c;

  while((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

/* SDBM hash */
uint32_t hash_str_sdbm(const void *key)
{
  const char *str = key;
  register uint32_t hash = 0;
  int c;

  while((c = *str++))
    hash = c + (hash << 6) + (hash << 16) - hash;

  return hash;
}

/* Loose Loose K&R */
uint32_t hash_str_kr(const void *key)
{
  const char *str = key;
  register uint32_t hash = 0;
  int c;

  while((c = *str++))
    hash += c;

  return hash;
}

/* Peter Weinberger's hash */
uint32_t hash_str_pjw(const void *key)
{
  const char *str = key;
  uint32_t h = 0;
  uint32_t g;
  int c;

  while((c = *str++)) {
    h = (h << 4) + c;

    if((g = h & 0xf0000000)) {
      h ^= g >> 24;
      h ^= g;
    }
  }

  return h;
}

/* UNIX ELF hash */
uint32_t hash_str_elf(const void *key)
{
  const char *str = key;
  uint32_t h = 0;
  uint32_t g;
  int c;

  while((c = *str++)) {
    h = (h << 4) + c;

    if((g = h & 0xf0000000))
      h ^= g >> 24;
    h &= ~g;
  }

  return h;
}

/* Knuth hash */
uint32_t hash_str_knuth(const void *key)
{
  const char *str = key;
  uint32_t hash = 0;
  int c;

  while((c = *str++))
    hash = ((hash << 5) ^ (hash >> 27)) ^ c;

  return hash;
}

/* Robert Jenkins's string hash */
uint32_t hash_str_jenkins(const void *key)
{
  const char *str = key;
  register uint32_t hash = 0;
  int c;

  while((c = *str++)) {
    hash += c;
    hash += hash << 10;
    hash ^= hash >> 6;
  }

  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;

  return hash;
}



/* Robert Jenkins's integer hash */
uint32_t hash_int_jenkins(const void *key)
{
  register uint32_t a = (uintptr_t)key;

  a = (a+0x7ed55d16) + (a<<12);
  a = (a^0xc761c23c) ^ (a>>19);
  a = (a+0x165667b1) + (a<<5);
  a = (a+0xd3a2646c) ^ (a<<9);
  a = (a+0xfd7046c5) + (a<<3);
  a = (a^0xb55a4f09) ^ (a>>16);

  return a;
}

/* Van Jacobson's IPv4 hash */
uint32_t hash_int_jacobson(const void *key)
{
  register uint32_t hash = (uintptr_t)key;

  hash ^= (hash >> 23) ^ (hash >> 17);

  return hash;
}

/* Knuth's multiplicative hash */
uint32_t hash_int_knuth(const void *key)
{
  register uint32_t hash = (uintptr_t)key;

  hash *= 0x9e3779b1;

  return hash;
}
