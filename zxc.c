

#include <stdint.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include "util.h"
#include "http.h"
#include "epoll.h"
#include "threadpool.h"

#define CONF "zxc.conf"


extern struct epoll_event *events;



int main(int argc, char* argv[]) {
    int rc;
  
    char *conf_file = CONF;

   

    /*
    * read confile file
    */
    char conf_buf[BUFLEN];
    zxc_conf_t cf;
    rc = read_conf(conf_file, &cf, conf_buf, BUFLEN);


  
    /*
    * initialize listening socket
    */
    int listenfd;
    struct sockaddr_in clientaddr;
    socklen_t inlen = 1;
    memset(&clientaddr, 0, sizeof(struct sockaddr_in));  
    
    listenfd = open_listenfd(cf.port);
    rc = make_socket_non_blocking(listenfd);
   

    /*
    * create epoll and add listenfd to ep
    */
    int epfd = zxc_epoll_create(0);
    struct epoll_event event;
    
    zxc_http_request_t *request = (zxc_http_request_t *)malloc(sizeof(zxc_http_request_t));
    zxc_init_request_t(request, listenfd, &cf);

    event.data.ptr = (void *)request;
    event.events = EPOLLIN | EPOLLET;
    zxc_epoll_add(epfd, listenfd, &event);

    /*
    create thread pool
    */
    zxc_threadpool_t *tp = threadpool_init(cf.thread_num);
    
    /* epoll_wait loop */
    while (1) {
        int n;
        n = zxc_epoll_wait(epfd, events, MAXEVENTS, -1);
        
        int i, fd;
        for (i=0; i<n; i++) {
            zxc_http_request_t *r = (zxc_http_request_t *)events[i].data.ptr;
            fd = r->fd;
            
            if (listenfd == fd) {
                /* we hava one or more incoming connections */

                while(1) {
                  
                    int infd = accept(listenfd, (struct sockaddr *)&clientaddr, &inlen);
                    if (infd == -1) {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                            /* we have processed all incoming connections */
                            break;
                        } else {
                    
                            break;
                        }
                    }

                    rc = make_socket_non_blocking(infd);
                   
                    
                    zxc_http_request_t *request = (zxc_http_request_t *)malloc(sizeof(zxc_http_request_t));
                    if (request == NULL) {
                       
                        break;
                    }

                    zxc_init_request_t(request, infd, &cf);
                    event.data.ptr = (void *)request;
                    event.events = EPOLLIN | EPOLLET;

                    zxc_epoll_add(epfd, infd, &event);
                }   // end of while of accept

               
            } else {
                if ((events[i].events & EPOLLERR) ||
                    (events[i].events & EPOLLHUP) ||
                    (!(events[i].events & EPOLLIN))) {
                   
                    close(fd);
                    continue;
                }
                /*
                do_request(infd);
                close(infd);
                */
                
                rc = threadpool_add(tp, do_request, events[i].data.ptr);
               
            }
        }   //end of for
    }   // end of while(1)
    
    threadpool_destroy(tp, 1);

    return 0;
}
