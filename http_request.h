


#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <time.h>
#include "http.h"

#define ZXC_AGAIN    EAGAIN
#define ZXC_OK       0

#define ZXC_HTTP_PARSE_INVALID_METHOD        10
#define ZXC_HTTP_PARSE_INVALID_REQUEST       11
#define ZXC_HTTP_PARSE_INVALID_HEADER        12

#define ZXC_HTTP_UNKNOWN                     0x0001
#define ZXC_HTTP_GET                         0x0002
#define ZXC_HTTP_HEAD                        0x0004
#define ZXC_HTTP_POST                        0x0008

#define ZXC_HTTP_OK                          200

#define ZXC_HTTP_NOT_MODIFIED                304

#define ZXC_HTTP_NOT_FOUND                   404

#define MAX_BUF 8124

typedef struct zxc_http_request_s {
    void *root;
    int fd;
    char buf[MAX_BUF];
    void *pos, *last;
    int state;
    void *request_start;
    void *method_end;   /* not include method_end*/
    int method;
    void *uri_start;
    void *uri_end;      /* not include uri_end*/ 
    void *path_start;
    void *path_end;
    void *query_start;
    void *query_end;
    int http_major;
    int http_minor;
    void *request_end;

    struct list_head list;  /* store http header */
    void *cur_header_key_start;
    void *cur_header_key_end;
    void *cur_header_value_start;
    void *cur_header_value_end;

} zxc_http_request_t;

typedef struct {
    int fd;
    int keep_alive;
    time_t mtime;       /* the modified time of the file*/
    int modified;       /* compare If-modified-since field with mtime to decide whether the file is modified since last time*/

    int status;
} zxc_http_out_t;

typedef struct zxc_http_header_s {
    void *key_start, *key_end;          /* not include end */
    void *value_start, *value_end;
    list_head list;
} zxc_http_header_t;

typedef int (*zxc_http_header_handler_pt)(zxc_http_request_t *r, zxc_http_out_t *o, char *data, int len);

typedef struct {
    char *name;
    zxc_http_header_handler_pt handler;
} zxc_http_header_handle_t;

extern void zxc_http_handle_header(zxc_http_request_t *r, zxc_http_out_t *o);
extern int zxc_init_request_t(zxc_http_request_t *r, int fd, zxc_conf_t *cf);
extern int zxc_free_request_t(zxc_http_request_t *r);

extern int zxc_init_out_t(zxc_http_out_t *o, int fd);
extern int zxc_free_out_t(zxc_http_out_t *o);

extern const char *get_shortmsg_from_status_code(int status_code);

extern zxc_http_header_handle_t     zxc_http_headers_in[];

#endif

