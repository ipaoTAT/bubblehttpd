#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "request.h"
#include "event.h" 

extern int efd;

void deal_req(int fd, void *arg)
{
	char *data = (char *)arg;

	//http_req_t *req = (http_req_t *)malloc(sizeof(http_req_t));
	http_req_t req;
	req.fd = fd;
	req.method = (char *)malloc(sizeof(char) * METHOD_SIZE);
	memset(req.method, 0, METHOD_SIZE * sizeof(char));
	req.uri = (char *)malloc(sizeof(char) * URI_SIZE);
	memset(req.uri, 0, URI_SIZE * sizeof(char));
	req.version = (char *)malloc(sizeof(char) * VERSION_SIZE);
	memset(req.version, 0, VERSION_SIZE * sizeof(char));

	if(init_req(&req, data) < 0)
	{
		free_req(req);
		return;
	}

#ifdef DEBUG
	printf("method: %s\n",req.method);
	printf("uri: %s\n",req.uri);
	printf("version: %s\n",req.version);
	printf("connection: %d\n",req.keep_alive);
	printf("alive-time: %d\n",req.alive_time);
#endif

	if(response(req) < 0)
	{
		free_req(req);
		return;
	}

	free_req(req);
	
	return;
}

/* init the struct http_req by analysis req string */
int init_req(http_req_t *req, char *data)
{
	/****************************************\
	 *  http request header format:		*
	 *  	method| |URL| |version|CRLF	*
	 *	name|:| |value|CRLF		*
	 *	...				*
	 *	name|:| |value|CRLF		*
	 *	CRLF				*
	 *	{entity part(default no-use)} 	*
	\****************************************/
	int i = 0;
	char buf[100], name[20], value[20],*line;
	int buf_len = 100;
	line = data;

	req -> keep_alive = -1;

	if(sscanf(data, "%s %s %s",req -> method, req -> uri, req -> version) == 0)
	{
#ifdef DEBUG
		perror("sscanf() . m,u,v\n");
#endif
		return -1;
	}
	if(!strcmp(req -> version, "HTTP/1.0"))
	{
		/*http 1.0 ,no keep alive for default*/
		req -> keep_alive = 0;
	}
	if(!strncmp(req -> uri, "/", 1))
	{
		strcpy(req -> uri, (req -> uri) + 1);
		if(strlen(req -> uri) == 0)
			strncpy(req -> uri, "index.html", 10);
	}
	for(i = 0; line[i] != 13; i++);
	while(1)
	{
		line += i + 2;	/* CR LF*/
		for(i = 0; line[i] != 13 || line[i + 1] != 10 ; i++)
		{
			buf[i] = line[i];
		}
		buf[i] = '\0';
		if(strlen(buf) == 0)break;

		if(sscanf(buf, "%[^:]: %s", name, value) == 0)
		{
#ifdef DEBUG
			perror("sscanf(), name & value\n");
#endif
			return -1;
		}
		if(!strcmp(name, "Connection"))
		{
			if(!strcmp(value, "Keep-Alive"))req -> keep_alive = -1;
			else if(!strcmp(value, "closed"))req -> keep_alive = 0;
		}
		if(!strcmp(name, "Keep-Alive"))
		{
			if(sscanf(value, "%d", &(req -> keep_alive)) == 0)
			{
#ifdef DEBUG
				perror("sscanf(), alive_time\n");
#endif
				return -1;
			}
		}
		/* ...... */
	}
}

int free_req(http_req_t req)
{
	free(req.method);
	free(req.uri);
	free(req.version);
	return 0;
}
