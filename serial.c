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
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>

#include "xatoi.h"

const struct termios tty_CS8 = {
  .c_cflag = (CS8 | CLOCAL | CREAD) & ~CRTSCTS,
  .c_iflag = IGNPAR,
};

speed_t int_to_baud(int speed)
{
  const struct {
    int     intval;
    speed_t baud;
  } *b, bauds[] = {
    { 921600, B921600 },
    { 460800, B460800 },
    { 230400, B230400 },
    { 115200, B115200 },
    { 57600, B57600 },
    { 38400, B38400 },
    { 19200, B19200 },
    { 9600, B9600 },
    { 4800, B4800 },
    { 2400, B2400 },
    { 1800, B1800 },
    { 1200, B1200 },
    { 300, B300 },
    { 200, B200 },
    { 150, B150 },
    { 134, B134 },
    { 110, B110 },
    { 75, B75 },
    { 50, B50 },
    { 0,  B0 }};

  for(b = bauds; b->intval ; b++)
    if(b->intval == speed)
      return b->baud;
  return B0;
}

speed_t str_to_baud(const char *arg)
{
  speed_t speed;
  int err;

  int arg_val = xatou(arg, &err);
  if(err)
    goto ERR;

  speed = int_to_baud(arg_val);
  if(speed != B0)
    return speed;

ERR:
  errx(EXIT_FAILURE, "unrecognized speed");
}

int set_serial_speed(struct termios *tty, int fd, speed_t baudrate)
{
  int r;

  tcgetattr(fd, tty);
  r = cfsetspeed(tty, baudrate);
  tcsetattr(fd, TCSANOW, tty);
  tcflush(fd, TCOFLUSH);

  if(r < 0)
    return r;
  return 0;
}

int serial_init(struct termios *tty, const char *dev, speed_t baudrate)
{
  int fd = open(dev, O_RDWR | O_NOCTTY);
  if(fd < 0)
    err(EXIT_FAILURE, "cannot open serial port");

  /* initial checks */
  if(!isatty(fd))
    err(EXIT_FAILURE, "invalid serial port");

  /* we only setup the speed if requested */
  if(baudrate != B0)
    cfsetspeed(tty, baudrate);

  if(tcsetattr(fd, TCSANOW, tty) < 0)
    err(EXIT_FAILURE, "cannot set tty attributes");

  /* Some operating systems (eg Linux) bufferise the UART input
     even when the file descriptor is not opened. This may be
     useful in a lot of ca ses. However we may lay with incomplete
     messages on the UART buffer. Therefore we have to flush the
     buffer ourself. However for a reason unknown to me, we have
     to wait a bit before actually flushing. Otherwise the flush
     command would have no effect. */
  usleep(500);
  tcflush(fd, TCIOFLUSH);

  return fd;
}
