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

#ifndef _SM_KR_H_
#define _SM_KR_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct kr * kr_t;

/* Create the string matching context. The pattern argument will be matched
   against the proposed strings. This pattern is not duplicated and should not
   be freed until the string matching context is destroyed. The string matching
   is done using the Karp-Rabin algorithm (1987). */
kr_t kr_create(const char *pattern);

/* Search for one occurence of the pattern in a text. The search is abandoned as
   soon as one occurence is found. The size of the text must be passed in
   argument. */
bool kr_match(kr_t kr, const char *text, size_t size);

/* Search for all occurences of the pattern in a text. The function will return
   the index of the match in the text or less than zero if the pattern was not
   found. Subsequent calls for next occurences on the same text must be called
   will NULL as the text argument. The size of the text must be passed in
   argument. */
int kr_matchall(kr_t kr, const char *text, size_t size);

/* Destroy the string matching context. */
void kr_destroy(kr_t kr);

#endif /* _SM_KR_H_ */
