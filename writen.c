#include "writen.h"


ssize_t writen(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = (char *)usrbuf;
        
    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR)  /* interrupted by sig handler return */
                nwritten = 0;    /* and call write() again */
            else {
           
                return -1;       /* errorno set by write() */
            }
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    
    return n;
}


