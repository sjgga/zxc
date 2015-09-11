#ifndef _WRITEN_H
#define _WRITEN_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

ssize_t writen(int fd, void *usrbuf, size_t n);

#endif
