#include <stdio.h>
#include <string.h>
#include "config.h"

// the follow global values are default web server configs 
#define WEB_ROOT_PATH_LEN	30			// web server root directory is length , default 
#define	WEB_CACH_PATH_LEN	30			// web server cache directory is length , default 
#define CONFIG_PATH                     "etc/bubble.conf"


short  PORT = 80;						// web server listening port , default 

int  BACKLOG = 10;						// web server tcp listen queue length , default 

size_t RECV_BUFFER_SIZE = 1024;			// web server receive buffer size , default 

size_t SEND_BUFFER_SIZE = 1024;		// web server send buffer size , default 

size_t TIME_BUFFER_SIZE = 40;			// web server time buffer size , default 

size_t URI_SIZE = 128;					// web server uri buffer size , default 

size_t MIME_BUFFER_SIZE = 20;			// web server mime type buffer size , default 

size_t CONTENT_ENCODING_SIZE = 40;		// web server content-encoding buffer size , default 

size_t FILE_MAX_SIZE = 1048576;			// web server file buffer size , default 

size_t CPU_NUM = 2;			// size of thread pool

size_t THREAD_POOL_SIZE = 1;

char WEB_ROOT_PATH[WEB_ROOT_PATH_LEN + 1];	// neb server root directory 

int config()
{
	FILE *fp = NULL;
	if( (fp = fopen(CONFIG_PATH, "r")) == NULL)
	{
		printf("could not to open %s\n", CONFIG_PATH);
		return -1;
	}
	char line_buf[CONFIG_LINE_MAX_LEN];
	memset(line_buf, '\0', CONFIG_LINE_MAX_LEN);
	char *line = NULL;
	while((line = fgets(line_buf, CONFIG_LINE_MAX_LEN, fp)) != NULL)
	{
		if(*line == '#')
			continue;
			
		if(!strncmp(line, "PORT=", 5))
		{
			if(sscanf(line + 5, "%hd;", &PORT) != 1)
			{
#ifdef DEBUG
				printf("sscanf PORT,use default");
#endif
			}
#ifdef DEBUG
			printf("Read config: PORT=%d\n",(int)PORT);
#endif
			continue;
		}
		if(!strncmp(line, "BACKLOG=", 8))
		{
			if(sscanf(line + 8, "%u;", &BACKLOG) != 1)
			{
#ifdef DEBUG
				printf("sscanf BACKLOG,use default");
#endif
			}
			continue;
		}
		if(!strncmp(line, "RECV_BUFFER_SIZE=", 17))
		{
			if(sscanf(line + 17, "%u;", &RECV_BUFFER_SIZE) != 1)
			{
#ifdef DEBUG
				printf("sscanf RECV_BUFFER_SIZE,use default");
#endif
			}
			continue;
		}
		if(!strncmp(line, "SEND_BUFFER_SIZE=", 17))
		{
			if(sscanf(line + 17, "%u;", &SEND_BUFFER_SIZE) != 1)
			{
#ifdef DEBUG
				printf("sscanf SEND_BUFFER_SIZE,use default");
#endif
			}
			continue;
		}
		if(!strncmp(line, "TIME_BUFFER_SIZE=", 17))
		{
			if(sscanf(line + 17, "%u;", &TIME_BUFFER_SIZE) != 1)
			{
#ifdef DEBUG
				printf("sscanf TIME_BUFFER_SIZE,use default");
#endif
			}
			continue;
		}
		if(!strncmp(line, "URI_SIZE=", 9))
		{
			if(sscanf(line + 9, "%u;", &URI_SIZE) != 1)
			{
#ifdef DEBUG
				printf("sscanf URI_SIZE,use default");
#endif
			}
			continue;
		}
		if(!strncmp(line, "MIME_BUFFER_SIZE=", 17))
		{
			if(sscanf(line + 17, "%u;", &MIME_BUFFER_SIZE) != 1)
			{
#ifdef DEBUG
				printf("sscanf MIME_BUFFER_SIZE,use default");
#endif
			}
			continue;
		}
		if(!strncmp(line, "CONTENT_ENCODING_SIZE=", 22))
		{
			if(sscanf(line + 22, "%u;", &CONTENT_ENCODING_SIZE) != 1)
			{
#ifdef DEBUG
				printf("sscanf CONTENT_ENCODING_SIZE,use default");
#endif
			}
			continue;
		}
		if(!strncmp(line, "FILE_MAX_SIZE=", 14))
		{
			if(sscanf(line + 14, "%u;", &FILE_MAX_SIZE) != 1)
			{
#ifdef DEBUG
				printf("sscanf FILE_MAX_SIZE,use default");
#endif
			}
			continue;
		}
		if(!strncmp(line, "WEB_ROOT_PATH=", 14))
		{
			memset(WEB_ROOT_PATH, 0, sizeof(WEB_ROOT_PATH));
			if(sscanf(line + 14, "%s", WEB_ROOT_PATH) != 1)	
			{
#ifdef DEBUG
				printf("sscanf WEB_ROOT_PATH, use default");	
#endif
				strcpy(WEB_ROOT_PATH, "../webroot");			//set to default 
			}

			continue;
		}
		
		if(!strncmp(line, "THREAD_POOL_SIZE=", 17))
		{
			if(sscanf(line + 17, "%u;", &THREAD_POOL_SIZE) != 1)
			{
#ifdef DEBUG
				printf("sscanf THREAD_POOL_SIZE, use default");
#endif
			}
			continue;
		}

		else continue;
	}
	
	fclose(fp);
	return 0;
}
