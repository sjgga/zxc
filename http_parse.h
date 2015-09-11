

#ifndef HTTP_PARSE_H
#define HTTP_PARSE_H

#define CR '\r'
#define LF '\n'
#define CRLFCRLF "\r\n\r\n"

extern int zxc_http_parse_request_line(zxc_http_request_t *r);
extern int zxc_http_parse_request_body(zxc_http_request_t *r);

#endif
