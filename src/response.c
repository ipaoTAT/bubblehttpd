#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "config.h"
#include "request.h"
#include "threadpool.h"
#include "http_header.h"
#include "response.h"
#include "event.h"
int response(http_req_t req)
{
	/******** http response data ************\
	 * version| |statu_no| |description|CRLF*	
	 * name|:| |value|CRLF			*
	 * ...					*
	 * name|:| |value|CRLF			*
	 * CRLF					*
	 * {entity(some response no use)}	*
	\****************************************/

	http_res_t *res ;
	if((res = (http_res_t *)malloc(sizeof(http_res_t))) == NULL)
	{
		printf("malloc error\n");
		return -1;
	}
	res -> fd = req.fd;
	res -> statu = FILE_OK;
	res -> keep_alive = req.keep_alive;

	res -> content_type = (char *)malloc(sizeof(char) * CONTENT_TYPE_SIZE);
	memset(res -> content_type, 0, CONTENT_TYPE_SIZE * sizeof(char));
	
	res -> description = (char *)malloc(sizeof(char) * DESCRIPTION_SIZE);
	memset(res -> description, 0, DESCRIPTION_SIZE * sizeof(char));
	
	res -> time_buffer = (char *)malloc(sizeof(char) * TIME_SIZE);
	memset(res -> time_buffer, 0, TIME_SIZE * sizeof(char));

	res -> send_buffer = (char *)malloc(sizeof(char) * SEND_BUFFER_SIZE);
	memset(res -> send_buffer, 0, SEND_BUFFER_SIZE * sizeof(char));
	
	res -> version = (char *)malloc(sizeof(char) * VERSION_SIZE);
	memset(res -> version, 0, VERSION_SIZE * sizeof(char));

	strcpy(res -> version , req.version);

	int f;

	int flag = set_content_type(req.uri, res -> content_type);
	switch(flag)
	{
		case -1:
			res -> statu = UNSUPPORT_CONTENT_TYPE;
			strncpy(res -> description, "Unsupport ContentType\0", 22);
			strncpy(res -> content_type, "text/html\0", 10);
			break;
		case -2:
			res -> statu = BAD_REQUEST;
			strncpy(res -> description, "Bad Request\0", 22);
			strncpy(res -> content_type, "text/html\0", 10);
			break;
		default:

#ifdef DEBU
			printf("content-type : %s\n", res -> content_type);
#endif

			if((f = open(req.uri, O_RDONLY)) == -1)
			{
				res -> statu = FILE_NO_FOUND;
				strncpy(res -> description, "No Found", 8);
				strncpy(res -> content_type, "text/html\0", 10);
			}
	}
	if(res -> statu == FILE_NO_FOUND && (f = open(FILE_NO_FOUND_PAGE, O_RDONLY)) != -1)res -> statu = FILE_OK;
	if(res -> statu == UNSUPPORT_CONTENT_TYPE && (f = open(UNSUPPORT_CONTENT_TYPE_PAGE, O_RDONLY)) != -1)res -> statu = FILE_OK;
	if(res -> statu == BAD_REQUEST && (f = open(BAD_REQUEST_PAGE, O_RDONLY)) != -1)res -> statu = FILE_OK;

	if(res -> statu == FILE_OK)
	{
		res -> statu = FILE_OK;
		strncpy(res -> description, "OK", 2);

		struct stat st;
		if(fstat(f, &st) == -1)
		{
			fprintf(stderr, "fstat() error . in response.c . file[%s]\n",req.uri);
			free_res(res);
			return -1;
		}

		long file_size = st.st_size;
		if(file_size > FILE_MAX_SIZE )
		{
#ifdef DEBUG
			perror("file_size too long\n");
#endif
			free_res(res);
			return -1;
		}

		res -> filefd = f;
		res -> file_size = file_size;
	}


	if((res -> head_size = set_send_data(res)) < 0)
	{
#ifdef DEBUG
		printf("set header error\n");
#endif
		free_res(res);
		return -1;
	}

	pool_add_task(wtpool, send_data, req.fd, res);
	return 0;
}

void send_data(int fd, void *argv)
{
	http_res_t *res = (http_res_t *)argv;

#ifdef DEBUG
	printf("send data: \n%s\n", res -> send_buffer);
#endif 
	if(send(fd, res -> send_buffer, res -> head_size, 0) == -1)
	{
		free_res(res);
		fprintf(stderr, "[%d] : send data error\n", fd);
		return;
	}

	if(res -> statu == FILE_OK)
	{
		off_t offset = 0;
		sendfile(fd, res -> filefd, &offset, res -> file_size);
		close(res -> filefd);
	}

	if(res -> keep_alive == 0)
	{
#ifdef DEBUG
		printf("keep_alive = 0, close fd [%d]\n",fd);
#endif
		close(fd);
		if(my_events[fd].fd == fd)event_del(efd, &my_events[fd]);
		else{
			fprintf(stderr,"Cannot find event of fd[%d]\n",fd);
		}
	}

	free_res(res);

	return;
}

int set_content_type(char *uri, char *ct)
{
	/* file type decide content type*/
	char *type = NULL;
	int i;
	int l = strlen(uri);
	for(i = 0; i < l; i++)
	{
		if(uri[i] == '.' && uri[i+1] == '.')
		{
#ifdef DEBUG
			printf("Bad request\n");
#endif
			return -2;
		}
	}
	for(i = i - 1; uri[i] != '/'  && i >= 0; i--);
	for(; uri[i] != '.' && i < l ; i++);
	i ++;
	type = uri + i;

	/*  text/html */
	if(!strncmp(type, "html", 4) || !strncmp(type, "htm", 3))
	{
		strncpy(ct, "text/html", 9);
	}
	/* text/css */
	else if(!strncmp(type, "css", 3))
	{
		strncpy(ct, "text/css", 8);
	}
	/* text/javascript */
	else if(!strncmp(type, "js", 2))
	{
		strncpy(ct, "text/javascript", 15);
	}
	/* test/plain */
	else if(!strncmp(type, "txt", 3))
	{
		strncpy(ct, "text/plain", 10);
	}
	/* image/jpg */
	else if(!strncmp(type, "jpg", 3))
	{
		strncpy(ct, "image/jpeg", 10);
	}
	/* image/png */
	else if(!strncmp(type, "png", 3))
	{
		strncpy(ct, "image/png", 9);
	}
	/* image/ico */
	else if(!strncmp(type, "ico", 3))
	{
		strncpy(ct, "image/ico", 9);
	}
	/* unsupport type */
	else
	{
#ifdef DEBUG
		perror("unsupport content \n");
#endif 
		return -1;
	}

	return 0;
}

int free_res(http_res_t *res)
{
	free(res -> version);
	free(res -> content_type);
	free(res -> description);
	free(res -> send_buffer);
	free(res -> time_buffer);
	free(res);
	return 0;
}

