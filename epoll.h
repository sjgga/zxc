#ifndef _EPOLL_H
#define _EPOLL_H

#include <sys/epoll.h>

#define MAXEVENTS 1024

int zxc_epoll_create(int flags);
void zxc_epoll_add(int epfd, int fs, struct epoll_event *event);
void zxc_epoll_mod(int epfd, int fs, struct epoll_event *event);
void zxc_epoll_del(int epfd, int fs, struct epoll_event *event);
int zxc_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

#endif
