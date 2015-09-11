#include "epoll.h"


struct epoll_event *events;

int zxc_epoll_create(int flags) {
    int fd = epoll_create1(flags);

    events = (struct epoll_event *)malloc(sizeof(struct epoll_event) * MAXEVENTS);
    return fd;
}

void zxc_epoll_add(int epfd, int fd, struct epoll_event *event) {
    int rc = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, event);
    return;
}

void zxc_epoll_mod(int epfd, int fd, struct epoll_event *event) {
    int rc = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, event);
    return;
}

void zxc_epoll_del(int epfd, int fd, struct epoll_event *event) {
    int rc = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, event);
    return;
}

int zxc_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout) {
    int n = epoll_wait(epfd, events, maxevents, timeout);
    return n;
}
