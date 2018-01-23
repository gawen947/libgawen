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

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <err.h>

#include "atoi-gen.h"

static unsigned int symbol_value(char c)
{
  /* digit */
  if(c <= '9')
    return c - '0';

  /* lowercase */
  if(c > 'Z')
    return c - 'a' + 10;

  /* uppercase */
  return c - 'A' + 10;
}

int atoi_gen(const char *s)
{
  int sgn = 1;
  int val = 0;
  unsigned int base = 10;

#define ZERO_END(c) if(c == '\0') goto RESULT

  /* Skip leading spaces */
  for(; isspace(*s) ; s++)
    ZERO_END(*s);

  ZERO_END(*s);

  if(*s == '-') {
    sgn = -1;
    ZERO_END(*++s);
  }

  if(*s == '0') {
    ZERO_END(*++s);

    switch(*s) {
    case 'x':
    case 'X':
      base = 16;
      s++;
      break;
    case 'b':
    case 'B':
      base = 2;
      s++;
      break;
    default:
      base = 8;
    }
  }

  /* Convert from base */
  for(; !isspace(*s) ; s++) {
    ZERO_END(*s);

    val *= base;
    val += symbol_value(*s);
  }

  for(; isspace(*s) ; s++)
    ZERO_END(*s);

RESULT:
  return val * sgn;
}

static bool ishex(char c)
{
  if((c >= '0' && c <= '9') ||
     (c >= 'A' && c <= 'F') ||
     (c >= 'a' && c <= 'f'))
    return true;
  return false;
}

const char * parse_hex_until(const char *s, const char *delim,
                             unsigned int *v, const char *error_message,
                             bool accept_zero)
{
  unsigned int val = 0;
  const char *d;

  if(!ishex(*s))
    errx(EXIT_FAILURE,"%s: expect an hexadecimal value", error_message);

  for(; *s != '\0' ; s++) {
    d = strchr(delim, *s);
    if(d)
      goto EXIT;
    if(!ishex(*s))
      errx(EXIT_FAILURE, "%s: expect an hexadecimal value", error_message);

    val <<= 4;
    val += symbol_value(*s);
  }

  if(!accept_zero)
    errx(EXIT_FAILURE, "%s: premature ending", error_message);

EXIT:
  *v = val;
  return s;
}
