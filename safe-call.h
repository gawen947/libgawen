/* Copyright (c) 2011-2018, David Hauweele <david@hauweele.net>
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

#ifndef _SAFE_CALL_H_
#define _SAFE_CALL_H_

#include <sys/socket.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>

#ifdef __FreeBSD__
# include <sys/capsicum.h>
#endif

#define xfree(p) do { \
    if(p)             \
      free(p);        \
  } while(0)

/* Default error action. Just use err() to
   display the message, the error number as
   string and abort with EXIT_FAILURE. */
void safecall_act_err(const char *msg);

int xfork();
int xpipe(int pipefd[2]);
int xdup2(int oldfd, int newfd);
int xopen(const char *path, int flags, mode_t mode);
char * xgetcwd(char *buf, size_t size);
void * xmalloc(size_t size);
void * xrealloc(void *ptr, size_t size);
int xlisten(int sockfd, int backlog);
ssize_t xwrite(int fd, const void *buf, size_t count);
ssize_t xread(int fd, void *buf, size_t count);
int xstat(const char *path, struct stat *buf);
int xchown(const char *path, uid_t owner, gid_t group);
char * xreadlink_malloc_n(const char *filename, ssize_t *n);
int xutime(const char *filename, const struct utimbuf *times);
int xchdir(const char *path);
int xsocket(int domain, int type, int proto);
ssize_t xsend(int sockfd, const void *buf, size_t len, int flags);
int xaccept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
ssize_t xrecv(int sockfd, void *buf, size_t len, int flags);
int xbind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int xconnect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
char * xstrdup(const char *s);
void xstrcpy(char *dst, const char *src, size_t count); /* fail if strlen(src) > count */
#ifdef USE_THREAD
int xsem_init(sem_t *sem, int pshared, unsigned int value);
#endif /* USE_THREAD */

#ifdef __FreeBSD__
int xcap_rights_limit(int fd, const cap_rights_t *rights);
#endif

/* Default error action. */
void safecall_default_act(const char *msg);

/* Action done when an error occurs in one of the safe calls. */
extern void (*safecall_err_act)(const char *msg);

#endif /* _SAFE_CALL_H_ */
