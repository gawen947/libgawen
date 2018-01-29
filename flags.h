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

#ifndef _LIBGAWEN_FLAGS_H_
#define _LIBGAWEN_FLAGS_H_

#include <stdlib.h>
#include <stdint.h>

/* We have to specify manually wether we want 64-bit or 32-bit wide flags. */
#ifdef FLAGS64
# define FLAGS_CHUNK_SIZE 64
typedef uint64_t flags_t;
#else
# define FLAGS_CHUNK_SIZE 32
typedef uint32_t flags_t;
#endif /* FLAGS64 */

#define GET_BIT(flags, bit) \
  ((flags)[bit / FLAGS_CHUNK_SIZE]  &  ((flags_t)1 << (bit % FLAGS_CHUNK_SIZE)))

#define SET_BIT(flags, bit) \
  ((flags)[bit / FLAGS_CHUNK_SIZE] |=   (flags_t)1 << (bit % FLAGS_CHUNK_SIZE))

#define CLEAR_BIT(flags, bit) \
  ((flags)[bit / FLAGS_CHUNK_SIZE] &= ~((flags_t)1 << (bit % FLAGS_CHUNK_SIZE)))

enum flgstr_mode {
  FLGSTR_LONG     = 0x1,  /* display as long string */
  FLGSTR_SETCLR   = 0x2,  /* display both set and cleared flags */
  FLGSTR_CLRONLY  = 0x4,  /* only display cleared flags */
  FLGSTR_NOPREFIX = 0x8,  /* do not dislay +/- prefix */
  FLGSTR_UNKNOWN  = 0x10  /* report unknown flags */
};


struct flag_option {
  const char *name; /* long option name */
  char short_name;  /* short option name */
  int bit;          /* position of the flag bit */
};

/* This functions parse a string for several flags parameters and setup these
   flags accordingly in an array of integer. The flags can be either long or
   short. When a string is parsed it can reference only one long flag or
   multiple short flags. If there is any ambiguity between the short and one
   long flag the longest one will be choosen first.

   The function returns the number of flags found, or zero if the string is empty,
   or -1 if no flag was found.

   example:

   static flags_t global_flags;

   (...)

   enum flag_opt {
    FLAG_PENDING,
    FLAG_ACK,
    FLAG_PANCOMP
   };

   struct flag_option flag_opts[] = {
    { "pending", 'p', FLAG_PENDING },
    { "ack", 'a', FLAG_ACK },
    { "pan-comp", 'c', FLAG_PANCOMP },
    { NULL, 0, 0 }
   };

   getflg("+pending", flag_opts, &global_flags);
   -> enable pending flag in global_flags.

   getflg("-pac", flag_opts, &global_flags);
   -> disable pending, ack and pan-comp flags. */
int getflg(const char *argument, const struct flag_option *opts, flags_t *flags);

/* Return 1 if any flag specified in opts is set/cleared, 0 otherwise. */
int any_flag_set(const struct flag_option *opts, const flags_t *flags);
int any_flag_clr(const struct flag_option *opts, const flags_t *flags);

/* Return the number flags specified in opts that are set/cleared. */
int count_flag_set(const struct flag_option *opts, const flags_t *flags);
int count_flag_clr(const struct flag_option *opts, const flags_t *flags);

/* Convert bitflags into its string representation according to the flag definition opts.
   The string creation can be modified with mode flags. By default the string will be
   created with short option, prefix, and only display set flags.

   The function returns the length of the resulting string. */
int flgstr(char *buf, const struct flag_option *opts, const flags_t *flags, unsigned int mode);

/* Compute the maximum possible size of a string representation of the flags described
   in opts and for the mode specified. */
int flgstr_maxlen(const struct flag_option *opts, unsigned int mode);

#endif /* _LIBGAWEN_FLAGS_H_ */
