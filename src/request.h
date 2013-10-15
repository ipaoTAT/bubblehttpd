#ifndef REQUEST_H
#define REQUEST_H


#define	METHOD_SIZE	50
#ifndef URI_SIZE	
#define URI_SIZE	100
#endif
#define	VERSION_SIZE	100

typedef struct http_req_t
{
	/*from and to socket*/
	int fd;
	/* request method*/
	char *method;
	/* pointer to request file uri*/
	char *uri;
	char *version;
	/* pointer to request file*/
//	char *filename;
//	char *content_type;
//	char *content_encoding;
	/* 0 : non keep alive, else the time of keep alive*/
	int keep_alive;
	/* if keep alive, set live-time ,-1: until client close*/
	int alive_time;
} http_req_t;


void deal_req(int, void *); 

int init_req(http_req_t *, char *);

int free_req(http_req_t);
#endif
