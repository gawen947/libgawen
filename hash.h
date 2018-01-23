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

#ifndef _HASH_
#define _HASH_

/* key comparison */
bool htable_str_cmp(const void *k1, const void *k2);
bool htable_int_cmp(const void *k1, const void *k2);

/* strings */
uint32_t hash_str_djb2(const void *key);    /* Dan Berstein's hash (if you don't know, probably this one) */
uint32_t hash_str_sdbm(const void *key);    /* SDBM hash */
uint32_t hash_str_pjw(const void *key);     /* Peter Weinberger's hash */
uint32_t hash_str_elf(const void *key);     /* ELF hash */
uint32_t hash_str_knuth(const void *key);   /* Knuth's hash */
uint32_t hash_str_jenkins(const void *key); /* Robert Jenkins's sring hash */
uint32_t hash_str_kr(const void *key);      /* K&R lose lose hash (if you don't know, probably not this one) */

/* integer */
uint32_t hash_int_jenkins(const void *key);  /* Robert Jenkins's integer hash (if you don't know, probably this one) */
uint32_t hash_int_jacobson(const void *key); /* Van Jacobson's IPv4 hash */
uint32_t hash_int_knuth(const void *key);    /* Knuth's multiplicative hash (if you don't know, probably not this one) */

#endif /* _HASH_ */
