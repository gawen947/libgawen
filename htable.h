/* Copyright (c) 2011-2016, David Hauweele <david@hauweele.net>
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

#ifndef _HTABLE_H_
#define _HTABLE_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct htable * htable_t;

/* Create a new hash table. The number of buckets should
   be a power of two. The hash function take the key
   and return a 32 bit hash. Comparison is done on key and
   return true if they are equals, false otherwise. And the
   last function destroy data when necessary. It should be
   used to destroy the key too as long as it is stored
   with the data (for example inside a structure). Not useful
   though when the key is only an integer.

   You can also free the htable entries manually using ht_walk2().
   However you would still have to call ht_destroy() after that. */
htable_t ht_create(unsigned int nbuckets,
                   uint32_t (*hash)(const void *),
                   bool (*compare)(const void *, const void *),
                   void (*destroy)(void *));

/* Those function search, insert or replace an entry inside
   the hash table. If data is NULL, all those functions
   revert to a simple lookup in the hash table. If an entry
   was found for the key, it returns the entry's data,
   otherwise it returns NULL.

   If data is specified, the three functions will have
   different behaviors. Anytime an entry is replaced, the old
   entry is destroyed and both the entry's data and entry's
   key are replaced.

   ht_insert_or_replace (aka ht_search) replaces the entry
   if it is present or insert it otherwise. It always return
   the new data.

   ht_replace replaces the entry if it is present. Otherwise
   it does nothing and return NULL.

   ht_insert insert the entry if it is not present. Otherwise,
   if it is present, it returns the entry's data.

   NOTE: In a future MAJOR version, ht_search will only search
   for an entry without modifying the data. Thus its data
   argument will disappear. */
#define ht_insert_or_replace ht_search
void * ht_search(htable_t htable, const void *key, void *data);
void * ht_replace(htable_t htable, const void *key, void *data);
void * ht_insert(htable_t htable, const void *key, void *data);

/* Lookup inside the hash table for an entry specified by key.
   If this entry is found, this function simply return the
   associated data. Otherwise it will use the retrieve function
   to insert a new entry with its return value as data and return
   this data. This retrieve function will be passed the key as its
   first argument and the optarg as its second argument. The key
   point with this function is that is that it will not call the
   retrieve function as long as the requested entry is already
   inside the hash table.

   NOTE: This will probably change name in a future MAJOR version. */
void * ht_lookup(htable_t htable, const void *key,
                 void * (*retrieve)(const void *, void *),
                 void *optarg);

/* Walk through the hash table and apply the action function
   on each entry, passing the entry key, data and an extra data pointer.
   The second version, ht_walk2, also pass the entry key.
   The action is in the form:
     For ht_walk : void action(void *entry_data, void *data);
     For ht_walk2: void action(const void *entry_key, void *entry_data, void *data);
*/
void ht_walk(htable_t htable, void (*action)(void *, void *), void *data);
void ht_walk2(htable_t htable, void (*action)(const void *, void *, void *), void *data);

/* Delete the entry specified by key from the hash table.
   The destroy function used at creation is used to destroy
   the data and the key if necessary. */
void ht_delete(htable_t htable, const void *key);

/* Destroy each entry from the hash table and then destroy
   the hash table itself. */
void ht_destroy(htable_t htable);

#endif /* _HTABLE_H_ */
