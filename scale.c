/* Copyright (c) 2018, David Hauweele <david@hauweele.net>
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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "string.h"

#define SCALE_BUFFER_SIZE UINT8_MAX

#define NANO_MICROS  1000LLU
#define NANO_MILLIS  1000000LLU
#define NANO_SECONDS 1000000000LLU
#define NANO_MINUTES (60LLU  * NANO_SECONDS)
#define NANO_HOURS   (60LLU  * NANO_MINUTES)
#define NANO_DAYS    (24LLU  * NANO_HOURS)
#define NANO_WEEKS   (7LLU   * NANO_DAYS)
#define NANO_MONTHS  (31LLU  * NANO_DAYS)
#define NANO_YEARS   (365LLU * NANO_DAYS)

/* forward */
#define METRIC_EXA    1000000000000000000LLU
#define METRIC_PETA   1000000000000000LLU
#define METRIC_TERA   1000000000000LLU
#define METRIC_GIGA   1000000000LLU
#define METRIC_MEGA   1000000LLU
#define METRIC_KILO   1000LLU
#define METRIC_HECTO  100LLU

/* downard */
#define METRIC_DECI   10LLU
#define METRIC_CENTI  100LLU
#define METRIC_MILLI  1000LLU
#define METRIC_MICRO  1000000LLU
#define METRIC_NANO   1000000000LLU
#define METRIC_PICO   1000000000000LLU
#define METRIC_FEMTO  1000000000000000LLU
#define METRIC_ATTO   1000000000000000000LLU

static char buffer[SCALE_BUFFER_SIZE];

const char * scale_metric_u64(uint64_t value, const char *unit)
{
  const char *prefix = "";
  uint64_t factor = 1;

  if(value > METRIC_EXA) {
    prefix = "E";
    factor = METRIC_EXA;
  }
  else if(value > METRIC_PETA) {
    prefix = "P";
    factor = METRIC_PETA;
  }
  else if(value > METRIC_TERA) {
    prefix = "T";
    factor = METRIC_TERA;
  }
  else if(value > 1000000000LLU) {
    prefix = "G";
    factor = 1000000000LLU;
  }
  else if(value > 1000000LLU) {
    prefix = "M";
    factor = 1000000LLU;
  }
  else if(value > 1000LLU) {
    prefix = "k";
    factor = 1000LLU;
  }

  snprintf(buffer, SCALE_BUFFER_SIZE, "%.2f %s%s", (double)value / factor, prefix, unit);

  return buffer;
}

const char * scale_nsec_u64(uint64_t nsec)
{
  const char *unit = "ns";
  uint64_t factor = 1;

  if(nsec > NANO_YEARS) {
    unit   = "years";
    factor = NANO_YEARS;
  }
  else if(nsec > NANO_DAYS) {
    unit   = "days";
    factor = NANO_DAYS;
  }
  else if(nsec > NANO_HOURS) {
    unit   = "hours";
    factor = NANO_HOURS;
  }
  else if(nsec > NANO_MINUTES) {
    unit   = "minutes";
    factor = NANO_MINUTES;
  }
  else if(nsec > NANO_SECONDS) {
    unit   = "s";
    factor = NANO_SECONDS;
  }
  else if(nsec > NANO_MILLIS) {
    unit   = "ms";
    factor = NANO_MILLIS;
  }
  else if(nsec > NANO_MICROS) {
    unit   = "us";
    factor = NANO_MICROS;
  }

  snprintf(buffer, SCALE_BUFFER_SIZE, "%.2f %s", (double)nsec / factor, unit);

  return buffer;
}

double unscale_metric_d(const char *s, const char *unit)
{
  double result;
  char *endptr;

  /* read double */
  result = strtod(s, &endptr); /* FIXME: use xatod when available */

  /* skip spaces until metric prefix */
  for(; isspace(*endptr) ; endptr++);

  /* scale according to unit */
  switch(*endptr++) {
  case 'E':
    result *= METRIC_EXA;
    break;
  case 'P':
    result *= METRIC_PETA;
    break;
  case 'T':
    result *= METRIC_TERA;
    break;
  case 'G':
    result *= METRIC_GIGA;
    break;
  case 'M':
    result *= METRIC_MEGA;
    break;
  case 'k':
    result *= METRIC_KILO;
    break;
  case 'h':
    result *= METRIC_HECTO;
    break;
  case 'd':
    result /= METRIC_DECI;
    break;
  case 'c':
    result /= METRIC_CENTI;
    break;
  case 'm':
    result /= METRIC_MILLI;
    break;
  case 'u':
    result /= METRIC_MICRO;
    break;
  case 'p':
    result /= METRIC_PICO;
    break;
  case 'f':
    result /= METRIC_FEMTO;
    break;
  case 'a':
    result /= METRIC_ATTO;
    break;
  }

  /* If *unit* is not precised,
     we don't check anything
     and return directly. */
  if(!unit)
    return result;

  /* If *unit* is specified,
     we check that it matches,
     otherwise we return NaN. */
  if(!strcmp(endptr, unit))
    return result;
  else
    return NAN;
}

uint64_t unscale_time_nsec_u64(const char *s)
{
  double result;
  char *endptr;

  /* read double */
  result = strtod(s, &endptr); /* FIXME: use xatod when available */

  /* skip spaces until metric prefix */
  for(; isspace(*endptr) ; endptr++);

  /* scale according to unit (one letter) */
  if(*(endptr+1) == '\0') {
    switch(*endptr) {
    case 'n':
      return (uint64_t)result;
    case 'u':
      return (uint64_t)(result * NANO_MICROS);
    case 'm':
      return (uint64_t)(result * NANO_MILLIS);
    case 's':
    case '\0':
      return (uint64_t)(result * NANO_SECONDS);
    case 'M':
      return (uint64_t)(result * NANO_MINUTES);
    case 'H':
    case 'h':
      return (uint64_t)(result * NANO_HOURS);
    case 'd':
    case 'D':
      return (uint64_t)(result * NANO_DAYS);
    case 'w':
    case 'W':
      return (uint64_t)(result * NANO_WEEKS);
    case 'y':
    case 'Y':
      return (uint64_t)(result * NANO_YEARS);
    default:
      goto ERROR;
    }
  }

  /* scale according to unit (word) */
  if(!strcmp(endptr, "ns") || !strcmp(endptr, "nanoseconds") || !strcmp(endptr, "nanos"))
    return (uint64_t)result;
  else if(!strcmp(endptr, "us") || !strcmp(endptr, "microseconds") || !strcmp(endptr, "micros"))
    return (uint64_t)(result * NANO_MICROS);
  else if(!strcmp(endptr, "ms") || !strcmp(endptr, "milliseconds") || !strcmp(endptr, "millis"))
    return (uint64_t)(result * NANO_MILLIS);
  else if(!strcmp(endptr, "seconds") || !strcmp(endptr, "secs"))
    return (uint64_t)(result * NANO_SECONDS);
  else if(!strcmp(endptr, "minutes") || !strcmp(endptr, "mins"))
    return (uint64_t)(result * NANO_MINUTES);
  else if(!strcmp(endptr, "hours"))
    return (uint64_t)(result * NANO_HOURS);
  else if(!strcmp(endptr, "days"))
    return (uint64_t)(result * NANO_DAYS);
  else if(!strcmp(endptr, "weeks"))
    return (uint64_t)(result * NANO_DAYS);
  else if(!strcmp(endptr, "months"))
    return (uint64_t)(result * NANO_DAYS);
  else if(!strcmp(endptr, "years"))
    return (uint64_t)(result * NANO_DAYS);

  /* in any other case return NaN */
ERROR:
  return NAN;
}
