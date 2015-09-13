
#include <strings.h>
#include "http.h"

static const char* get_file_type(const char *type);
static void parse_uri(char *uri, int length, char *filename, char *querystring);
static char *ROOT = NULL;

mime_type_t zxc_mime[] = 
{
    {".html", "text/html"},
    {".xml", "text/xml"},
    {".xhtml", "application/xhtml+xml"},
    {".txt", "text/plain"},
    {".rtf", "application/rtf"},
    {".pdf", "application/pdf"},
    {".word", "application/msword"},
    {".png", "image/png"},
    {".gif", "image/gif"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".au", "audio/basic"},
    {".mpeg", "video/mpeg"},
    {".mpg", "video/mpeg"},
    {".avi", "video/x-msvideo"},
    {".gz", "application/x-gzip"},
    {".tar", "application/x-tar"},
    {".css", "text/css"},
    {NULL ,"text/plain"}
};

void do_request(void *ptr) {
    zxc_http_request_t *r = (zxc_http_request_t *)ptr;
    int fd = r->fd;
    int rc;
    char filename[SHORTLINE];
    struct stat sbuf;
    int n;
    ROOT = r->root;

    
    for(;;) {
        n = read(fd, r->last, (uint64_t)r->buf + MAX_BUF - (uint64_t)r->last);
   

        if (n == 0) {   // EOF
          
            goto err;
        }

        if (n < 0) {
            if (errno != EAGAIN) {
             
                goto err;
            }
            break;
        }

        r->last += n;
       
        
   
        rc = zxc_http_parse_request_line(r);
        if (rc == ZXC_AGAIN) {
            continue;
        } else if (rc != ZXC_OK) {
         
            goto err;
        }

       
        rc  = zxc_http_parse_request_body(r);
        if (rc == ZXC_AGAIN) {
            continue;
        } else if (rc != ZXC_OK) {
           
            goto err;
        }
        
        /*
        *   handle http header
        */
        zxc_http_out_t *out = (zxc_http_out_t *)malloc(sizeof(zxc_http_out_t));
        if (out == NULL) {
      
            exit(1);
        }

        rc = zxc_init_out_t(out, fd);
       

        parse_uri(r->uri_start, r->uri_end - r->uri_start, filename, NULL);

        if(stat(filename, &sbuf) < 0) {
          
            continue;
        }

        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))
        {
           
            continue;
        }
        
        out->mtime = sbuf.st_mtime;

        zxc_http_handle_header(r, out);
        
        
        if (out->status == 0) {
            out->status = zxc_HTTP_OK;
        }

        serve_static(fd, filename, sbuf.st_size, out);

        free(out);
        if (!out->keep_alive) {
          
            goto close;
        }

    }
    
    return;

err:
close:
    close(fd);
}

void parse_uri(char *uri, int uri_length, char *filename, char *querystring) {
    char *question_mark = index(uri, '?');
    int file_length;
    if (question_mark) {
        file_length = (int)(question_mark - uri);
    } else {
        file_length = uri_length;
    }

    strcpy(filename, ROOT);
    strncat(filename, uri, file_length);

    char *last_comp = rindex(filename, '/');
    char *last_dot = rindex(last_comp, '.');
    if (last_dot == NULL && filename[strlen(filename)-1] != '/') {
        strcat(filename, "/");
    }
    
    if(uri[strlen(uri)-1] == '/' || filename[strlen(filename)-1] == '/') {
        strcat(filename, "index.html");
    }

 
    return;
}


void serve_static(int fd, char *filename, size_t filesize, zxc_http_out_t *out) {
    char header[MAXLINE];
    char buf[SHORTLINE];
    int n;
    struct tm tm;
    
    const char *file_type;
    const char *dot_pos = rindex(filename, '.');
    file_type = get_file_type(dot_pos);

    sprintf(header, "HTTP/1.1 %d %s\r\n", out->status, get_shortmsg_from_status_code(out->status));

    if (out->keep_alive) {
        sprintf(header, "%sConnection: keep-alive\r\n", header);
    }

    if (out->modified) {
        sprintf(header, "%sContent-type: %s\r\n", header, file_type);
        sprintf(header, "%sContent-length: %zu\r\n", header, filesize);
        localtime_r(&(out->mtime), &tm);
        strftime(buf, SHORTLINE,  "%a, %d %b %Y %H:%M:%S GMT", &tm);
        sprintf(header, "%sLast-Modified: %s\r\n", header, buf);
    } else {

    }

    sprintf(header, "%sServer: ZXC\r\n", header);
    sprintf(header, "%s\r\n", header);

    n = rio_writen(fd, header, strlen(header));
    if (n != strlen(header)) {
      
        goto out; 
    }

    if (!out->modified) {
        goto out;
    }

    int srcfd = open(filename, O_RDONLY, 0);
  
    // can use sendfile
    char *srcaddr = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
   
    close(srcfd);

    n = rio_writen(fd, srcaddr, filesize);


    munmap(srcaddr, filesize);

out:
    return;
}

const char* get_file_type(const char *type)
{
    if (type == NULL) {
        return "text/plain";
    }

    int i;
    for (i = 0; zxc_mime[i].type != NULL; ++i) {
        if (strcmp(type, zxc_mime[i].type) == 0)
            return zxc_mime[i].value;
    }
    return zxc_mime[i].value;
}
