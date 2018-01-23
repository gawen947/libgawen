/* Copyright (c) 2013, David Hauweele <david@hauweele.net>
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

#include <stdlib.h>
#include <stdint.h>

#include "sm-kr.h"

struct kr {
  const char *pattern;
  const char *text;
  uint32_t hash_pattern;
  uint32_t hash_text;

  unsigned int len;
  unsigned int index;
};

kr_t kr_create(const char *pattern)
{
  const char *s;
  uint32_t hash = 0;
  uint32_t len  = 0;

  struct kr *kr = malloc(sizeof(struct kr));

  if(!kr)
    return NULL;

  for(s = pattern ; *s ; s++) {
    hash = (hash << 1) + *s;
    len++;
  }

  kr->pattern      = pattern;
  kr->hash_pattern = hash;
  kr->len          = len - 1;

  return kr;
}

static bool match(const char *pattern, const char *text)
{
  const char *p, *t;

  for(p = pattern, t = text ; *p == *t && *p != '\0' ; p++, t++);

  if(*p != *t && *p != '\0')
    return false;

  return true;
}

bool kr_match(kr_t kr, const char *text, size_t size)
{
  unsigned int i;
  unsigned int len = kr->len;
  uint32_t hash    = 0;

  for(i = 0 ; kr->pattern[i] != '\0' && text[i] != '\0' ; i++)
    hash = (hash << 1) + text[i];

  if(hash == kr->hash_pattern && match(kr->pattern, text))
    return true;

  for(i = 1 ; i < size - len ; i++) {
    hash -= (text[i-1] << len);
    hash  = (hash << 1) + text[i+len];

    if(hash == kr->hash_pattern && match(kr->pattern, text + i))
      return true;
  }

  return false;
}

int kr_matchall(kr_t kr, const char *text, size_t size)
{
  unsigned int i;
  unsigned int len;
  uint32_t     hash;

  if(text) {
    hash = 0;

    for(i = 0 ; kr->pattern[i] != '\0' && text[i] != '\0' ; i++)
      hash = (hash << 1) + text[i];

    kr->text      = text;
    kr->hash_text = hash;
    kr->index     = 1;

    if(hash == kr->hash_pattern && match(kr->pattern, text))
      return 0;
  }

  text = kr->text;
  len  = kr->len;
  hash = kr->hash_text;

  for(i = kr->index ; i < size - len ; i++) {
    hash -= (text[i-1] << len);
    hash  = (hash << 1) + text[i+len];

    if(hash == kr->hash_pattern && match(kr->pattern, text + i)) {
      kr->hash_text = hash;
      kr->index     = i+1;
      return i;
    }
  }

  return -1;
}

void kr_destroy(kr_t kr)
{
  free(kr);
}
