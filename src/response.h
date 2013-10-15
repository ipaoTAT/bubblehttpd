#ifndef RESPONSE_H
#define RESPONSE_H

#include "request.h"


#define CONTENT_TYPE_SIZE	50
#define	STATU_SIZE	5
#define	DESCRIPTION_SIZE	50
#define ENTITY_BUF_SIZE	100
#define TIME_SIZE 100

#define SERVER_INFO ""
#define FILE_NO_FOUND_PAGE "404.html"
#define UNSUPPORT_CONTENT_TYPE_PAGE "415.html"
#define BAD_REQUEST_PAGE "400.html"
#define SERVER_ERROR_PAGE "500.html"

typedef struct http_res_t
{
	int fd;
	int filefd;
	long head_size;
	long file_size;
	char *content_type;
	/* statu: 200-succeed, 404-no found, 500-server error*/
	int statu;
	char *description;
	char *version;

	char *time_buffer;

	char *send_buffer;

	int keep_alive;
} http_res_t;

int response(http_req_t req);

void send_data(int , void *);

int set_content_type(char *, char *);

int free_res(http_res_t *);

#endif
