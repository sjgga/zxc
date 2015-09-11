#ifndef _UTIL_H
#define _UTIL_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>



// max number of listen queue
#define LISTENQ     1024

#define BUFLEN      8192

#define DELIM       "="



struct zxc_conf_s {
    void *root;
    int port;
    int thread_num;
};

typedef struct zxc_conf_s zxc_conf_t;

int open_listenfd(int port);
int make_socket_non_blocking(int fd);

int read_conf(char *filename, zxc_conf_t *cf, char *buf, int len);
#endif
