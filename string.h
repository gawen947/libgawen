/* Copyright (c) 2016-2017, David Hauweele <david@hauweele.net>
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

#ifndef _LIBGAWEN_STRING_H_
#define _LIBGAWEN_STRING_H_

#include <ctype.h>

/* Remove leading and trailing spaces.
   The rtrim() variant only remove trailing spaces. */
#define trim(s)       strip(s, isspace)
#define rtrim(s, len) rstrip(s, isspace, len)

/* Transform macro argument as string literal. */
#define _stringify(s) #s
#define stringify(s) _stringify(s)

/* Compare two byte strings from the end.
   Useful for comparing string suffixes. */
#define memrevcmp(s, d, len) memcmp(s - len, d - len, len)

/* Equivalent of the Unix basename command. */
const char * basename(const char *s);

/* Remove characters according to istrim(),
   at the beginning and end of the string,
   and return the start of the new string. */
char * strip(char *s, int (*istrim)(int));

/* Remove trailing characters from a string.
   Size must be given in strlen() term.
   The function returns the new size of the string
   also in strlen() term. */
unsigned int rstrip(char *buf, int (*istrim)(int), unsigned int size);
unsigned int rstrip_s(char *buf, const char *remove, unsigned int size);

/* Put double quotes around a string and escape any " or \ character with \", \\.
   Returns the length of the escaped string. */
unsigned int stresc(char *buf, const char *str);

/* Parse an escaped string of the form "...<escaped>..." and return the character
   just after " position. Within the "...", any \ is replaced by the character
   directly following. Return NULL in case of a parsing error. */
const char * strunesc(char *buf, const char *str);

#endif /* _LIBGAWEN_STRING_H_ */
