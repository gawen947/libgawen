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

#ifndef _BST_H_
#define _BST_H_

typedef struct bst * bst_t;

/* Create a new binary sort tree. The compare function compares two data and
   return an integer less than, equal to, or greater than zero if the first
   argument is found, respectively to be less than, to match, or be greater than
   the second argument. And the last function destroy the data when
   necessary. */
bst_t bst_create(int  (*compare)(const void *, const void *),
                 void (*destroy)(void *));

/* Insert the specifid data into the tree. This function return the specified
   data or NULL if it cannot be inuserted. The only reason why it can go wrong
   is an out of memory error on the heap. */
void * bst_insert(bst_t bst, void *data);

/* Search for data inside the tree. This function will return the specified data
   if it was found in the tree. Otherwise it will return NULL. */
void * bst_search(bst_t bst, void *data);

/* In-order walk through the binary sort tree and apply action function on
   each entry, passing data to this function. Beware that this function may
   provoke a stack overflow. */
void bst_walk(bst_t bst, void (*action)(void *));

/* Delete the entry specified from the binary sort tree. The destroy function
   passed at creation is used to destroy the data if necessary. */
void bst_delete(bst_t bst, void *data);

/* Destroy each entry from the binary search sort and then destroy the tree
   itself. Beware that this function may provoke a stack overflow with a very
   deep tree. */
void bst_destroy(bst_t bst);

#endif /* _BST_H_ */
