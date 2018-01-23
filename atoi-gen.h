/* Copyright (c) 2013-2016, David Hauweele <david@hauweele.net>
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

#ifndef _ATOI_GEN_H_
#define _ATOI_GEN_H_

#include <stdbool.h>

/* This generic atoi function understand number written in different bases. By
   default it will assume a decimal base. If the string is prefixed with 0x or
   0X it will assume an hexadecimal base. If the string is prefixed with 0b or
   0B it will assume a binary base. If the string is prefixed with a 0 it will
   assume an octal base. Leading and trailing spaces are ignored. There isn't
   any error checking and the result may be incorrect in case of overflow or
   incorrect symbols for the choosen base. For an error aware generic converter
   you may use strtol_gen instead (not implemented yet). */
int atoi_gen(const char *s);

/* This function will parse an hexadecimal string stopping at the first
   occurence of a character in the delim string. It will then the value parsed
   until this delimiter in the v argument and return a pointer to the delimiter
   in the string. If the accept_zero flag is specified, the null terminator will
   be accepted as a valid delimiter. Otherwise an error will be reported. This
   function will also check for valid hexadecimal characters and will report an
   error if the character is invalid. */
const char * parse_hex_until(const char *s, const char *delim,
                             unsigned int *v, const char *error_message,
                             bool accept_zero);

#endif /* _ATOI_GEN_H_ */
