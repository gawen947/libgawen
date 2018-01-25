/* Copyright (c) 2016, David Hauweele <david@hauweele.net>
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

#ifndef _LIBGAWEN_ALIGN_H_
#define _LIBGAWEN_ALIGN_H_

#include <stdio.h>

enum {
  ALC_DESC  = 1,
  ALC_VALUE = 2,
};

/* Align (push) a list of description/value fields and then write (commit) them in one go in
   a specified file. The provided strings can be automatically freed after commit by specifying
   the ALC_DESC or ALC_VALUE flags.

   example usage:

   push_aligned_display("v1", "value1", 0);
   push_aligned_display("v2", strdup(value2), ALC_VALUE);
   push_aligned_display("field3", "value3", 0);
   push_aligned_display(strdup("fld4"), strdup("value4"), ALC_DESC | ALC_VALUE);

   commit_aligned_display(stdout);

   result:

   v1     value1
   v2     value2
   fld4   value4
   field3 value3
*/
void push_aligned_display(const char *description, const char *value, unsigned int allocated);
void commit_aligned_display(FILE *f);

#endif /* _LIBGAWEN_ALIGN_H_ */
