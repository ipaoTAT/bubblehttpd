#include <stdio.h>
#ifndef CONFIG_H
#define CONFIG_H

// the webconfig.conf file each line's max length 
#define  CONFIG_LINE_MAX_LEN 128
#define  WEB_ROOT_PATH_LEN   30


extern short PORT ;	// 16 bits port

extern int  BACKLOG ;

extern size_t RECV_BUFFER_SIZE ;

extern size_t SEND_BUFFER_SIZE ;

extern size_t TIME_BUFFER_SIZE ;

extern size_t URI_SIZE ;

extern size_t MIME_BUFFER_SIZE ;

extern size_t CONTENT_ENCODING_SIZE ;

extern size_t FILE_MAX_SIZE ;

extern size_t CPU_NUM ;

extern size_t THREAD_POOL_SIZE ;

extern char WEB_ROOT_PATH[WEB_ROOT_PATH_LEN + 1];       // web server root directory 

// read the web server config file -- 'webconfig.conf' ,and set it into the server
int config(void);

#endif
