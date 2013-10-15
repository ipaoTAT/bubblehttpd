#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "http_header.h"
#include "response.h"
#include "time.h"

int set_send_data(http_res_t *res)
{
	if(get_time_str(res -> time_buffer) == -1)
	{
		perror("get_time_str() error . in http_header.c");
		strncpy(res -> time_buffer, "GET TIME ERROR", 14);
	}
	
	register int index = 0;
	register int strsize = 0;
	switch(res -> statu)
	{
		
		case FILE_NO_FOUND:
			strcat((char *)res -> send_buffer, "HTTP/1.1 400 No Found\r\n");
			strcat((char *)res -> send_buffer, SERVER);
			strcat((char *)res -> send_buffer, res -> time_buffer);
			strcat((char *)res -> send_buffer, ALLOW);
			strcat((char *)res -> send_buffer, "Content-Type: text/plain\r\n");
			strcat((char *)res -> send_buffer, "Content-Length: 18\r\n");
			strcat((char *)res -> send_buffer, "\r\n");
			strcat((char *)res -> send_buffer, "400 File No Found!");
			return strlen(res -> send_buffer);
	
		case FILE_OK:
			memcpy(res -> send_buffer, "HTTP/1.1 200 OK\r\n", 17);	/* response header */
			strsize = strlen(SERVER);
			index += 17;
			memcpy(res -> send_buffer + index, SERVER, strsize);				/* Server field */
			index += strsize;
			strsize = strlen(res -> time_buffer);
			memcpy(res -> send_buffer + index, res -> time_buffer, strsize);			/* Date field */
			index += strsize;
		
			memcpy(res -> send_buffer + index, "Content-Type: ", 14);
			index += 14;
			strsize = strlen(res -> content_type);
			memcpy(res -> send_buffer + index, res -> content_type, strsize);
			index += strsize;

			memcpy(res -> send_buffer + index, "\r\n", 2);
			index += 2;
			memcpy(res -> send_buffer + index, "Content-Length: ", 16);
			index += 16;
			
			sprintf( (char *)res -> send_buffer + index, "%ld\r\n\r\n" , res -> file_size);
			
			index += strlen((char *)(res -> send_buffer + index));

			return index;
			
		/* the client browse requair a not surported content type */
		case UNSUPPORT_CONTENT_TYPE:
			strcat((char *)res -> send_buffer, "HTTP/1.1 415 UNSUPPORT_CONTENT_TYPE\r\n");
			strcat((char *)res -> send_buffer, SERVER);
			strcat((char *)res -> send_buffer, res -> time_buffer);
			strcat((char *)res -> send_buffer, ALLOW);
			strcat((char *)res -> send_buffer, "Content-Type: text/html\r\n");
			strcat((char *)res -> send_buffer, "Content-Length: 27\r\n");
			strcat((char *)res -> send_buffer, "\r\n");
			strcat((char *)res -> send_buffer, "415 Unsupport content type!");
			return strlen(res -> send_buffer);
		case BAD_REQUEST:
			strcat((char *)res -> send_buffer, "HTTP/1.1 400 BAD_REQUEST\r\n");
			strcat((char *)res -> send_buffer, SERVER);
			strcat((char *)res -> send_buffer, res -> time_buffer);
			strcat((char *)res -> send_buffer, ALLOW);
			strcat((char *)res -> send_buffer, "Content-Type: text/html\r\n");
			strcat((char *)res -> send_buffer, "Content-Length: 16\r\n");
			strcat((char *)res -> send_buffer, "\r\n");
			strcat((char *)res -> send_buffer, "400 Bad Request!");
			return strlen(res -> send_buffer);
			
		/* some unknown error happened on server */
		default:
			strcat((char *)res -> send_buffer, "HTTP/1.1 500 SERVER_ERROR\r\n");
			strcat((char *)res -> send_buffer, SERVER);
			strcat((char *)res -> send_buffer, res -> time_buffer);
			strcat((char *)res -> send_buffer, ALLOW);
			strcat((char *)res -> send_buffer, "Content-Type: text/html\r\n");
			strcat((char *)res -> send_buffer, "Content-Length: 17\r\n");
			strcat((char *)res -> send_buffer, "\r\n");
			strcat((char *)res -> send_buffer, "500 Server Error!");
			return strlen(res -> send_buffer);
			
	}		
	
	return 0;
}
