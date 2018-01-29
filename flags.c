/* Copyright (c) 2013-2018, David Hauweele <david@hauweele.net>
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

#ifdef __linux__
# define _POSIX_C_SOURCE 200809L
#endif

#include <string.h>

#include "flags.h"

enum priv_flgstr_mode {
  PRIV_FLGSTR_SET       = 0x1,  /* display set flags */
  PRIV_FLGSTR_CLR       = 0x2,  /* display clr flags */
  PRIV_FLGSTR_SETPREFIX = 0x4,  /* display set prefixes */
  PRIV_FLGSTR_CLRPREFIX = 0x8   /* display clr prefixes */
};

static void do_flag(flags_t *flags, char type, int bit)
{
  flags_t bitval = (flags_t)1 << (bit % FLAGS_CHUNK_SIZE);
  int idx        = bit / FLAGS_CHUNK_SIZE;

  switch(type) {
  case '+':
    flags[idx] |= bitval;
    break;
  case '-':
    flags[idx] &= ~bitval;
    break;
  default:
    break;
  }
}

int getflg(const char *argument, const struct flag_option *opts, flags_t *flags)
{
  const struct flag_option *o;
  char type;
  int count = 0;

  /* check for empty string */
  if(*argument == '\0')
    return 0;

  /* set or clear */
  switch(*argument) {
  case '+':
  case '-':
    type = *argument++;
    break;
  default:
    type = '+';
    break;
  }

  /* check for long option */
  for(o = opts ; o->name ; o++) {
    if(!strcmp(argument, o->name)) {
      do_flag(flags, type, o->bit);
      return 1;
    }
  }

  /* check for short option */
  for(; *argument ; argument++) {
    for(o = opts ; o->name ; o++) {
      if(*argument == o->short_name) {
        do_flag(flags, type, o->bit);
        count++;
        goto FOUND;
      }
    }

    /* not found */
    return -1;

  FOUND:
    continue;
  }

  return count;
}

int any_flag_set(const struct flag_option *opts, const flags_t *flags)
{
  const struct flag_option *o;

  for(o = opts ; o->name ; o++) {
    if(GET_BIT(flags, o->bit))
      return 1;
  }
  return 0;
}

int any_flag_clr(const struct flag_option *opts, const flags_t *flags)
{
  const struct flag_option *o;

  for(o = opts ; o->name ; o++) {
    if(!GET_BIT(flags, o->bit))
      return 1;
  }
  return 0;
}

int count_flag_set(const struct flag_option *opts, const flags_t *flags)
{
  const struct flag_option *o;
  unsigned int n = 0;

  for(o = opts ; o->name ; o++)
    n += GET_BIT(flags, o->bit) ? 1 : 0;

  return n;
}

int count_flag_clr(const struct flag_option *opts, const flags_t *flags)
{
  const struct flag_option *o;
  unsigned int n = 0;

  for(o = opts ; o->name ; o++)
    n += GET_BIT(flags, o->bit) ? 0 : 1;

  return n;
}

static char * long_flag(char *b, const char *name, int val, unsigned int mode, unsigned int priv_mode)
{
  if(!(mode & FLGSTR_NOPREFIX)) {
    if(val) {
      if(priv_mode & PRIV_FLGSTR_SETPREFIX)
        *b++ = '+';
    } else {
      if(priv_mode & PRIV_FLGSTR_CLRPREFIX)
        *b++ = '-';
    }
  }

  b    = stpcpy(b, name);
  *b++ = ','; /* will be removed on last */

  return b;
}

static int compute_priv_mode(unsigned int mode)
{
  unsigned int priv_mode = 0;

  /* configure private mode */
  if(mode & FLGSTR_CLRONLY)
    priv_mode |= PRIV_FLGSTR_CLR | PRIV_FLGSTR_SETPREFIX;
  else if(mode & FLGSTR_SETCLR)
    priv_mode |= PRIV_FLGSTR_SET | PRIV_FLGSTR_CLR | \
                 PRIV_FLGSTR_SETPREFIX | PRIV_FLGSTR_CLRPREFIX;
  else
    priv_mode |= PRIV_FLGSTR_SET;
  if(mode & FLGSTR_NOPREFIX)
    priv_mode |= 0; /* just explicity "nah..."" */
  else if(mode & FLGSTR_SETCLR)
    priv_mode |= PRIV_FLGSTR_SETPREFIX | PRIV_FLGSTR_CLRPREFIX;
  else if(mode & FLGSTR_CLRONLY)
    priv_mode |= PRIV_FLGSTR_CLRPREFIX;
  else
    priv_mode |= PRIV_FLGSTR_SETPREFIX;

  return priv_mode;
}

int flgstr(char *buf, const struct flag_option *opts, const flags_t *flags, unsigned int mode)
{
  unsigned int priv_mode = compute_priv_mode(mode);
  const struct flag_option *o;
  char *b = buf;

  /* handle long options */
  if(mode & FLGSTR_LONG) {
    for(o = opts ; o->name ; o++)
      b = long_flag(b, o->name, GET_BIT(flags, o->bit), mode, priv_mode);

    /* remove last ',' */
    if(b > buf)
      b--; /* will be set to '\0' on exit */
  }
  /* handle short options */
  else {
    if((priv_mode & PRIV_FLGSTR_SET) && any_flag_set(opts, flags)) {
      if(priv_mode & PRIV_FLGSTR_SETPREFIX)
        *b++ = '+';
      for(o = opts ; o->name ; o++) {
        if(GET_BIT(flags, o->bit))
          *b++ = o->short_name;
      }
    }

    if((priv_mode & PRIV_FLGSTR_CLR) && any_flag_clr(opts, flags)) {
      if(priv_mode & PRIV_FLGSTR_CLRPREFIX)
        *b++ = '-';
      for(o = opts ; o->name ; o++) {
        if(!GET_BIT(flags, o->bit))
          *b++ = o->short_name;
      }
    }
  }

  *b = '\0';

  return b - buf;
}

int flgstr_maxlen(const struct flag_option *opts, unsigned int mode)
{
  /* get it there, we don't mind losing a bunch of bytes */
  const struct flag_option *o;
  unsigned int n = 0;

  if(mode & FLGSTR_LONG) {
    for(o = opts ; o->name ; o++)
      n += strlen(o->name) + 2 /* [+-], */;
  } else {
    for(o = opts ; o->name ; o++)
      n++;
    n += 2; /* +- */
  }

  n++; /* '\0' */

  return n;
}
