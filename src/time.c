#include <time.h>
#include <stdio.h>
#include <string.h>
#include "time.h"



/* get the time on server,
   return: the length of ascii string of time , -1 on error
   argument: time_buf the buffer to store time_string
   time format:	Thu, 08 Oct 2009 08:49:28 UTC
*/

int get_time_str(char *time_buf)
{
	extern size_t TIME_BUFFER_SIZE;
	time_t	now_sec;
	struct tm	time_now;
	setenv("TZ","GMT-8",1);
	tzset();
	if(time(&now_sec) == -1)
	{
#ifdef DEBUG
		printf("time() in get_time.c");
#endif
		return -1;
	}
	if((gmtime_r(&now_sec, &time_now)) == NULL)
	{
#ifdef DEBUG
		printf("localtime in get_time.c");
#endif
		return -1;
	}
	int strlen = -1;
	/* strftime maybe not thread safe,but i am not sure */
	if((strlen = strftime(time_buf, TIME_BUFFER_SIZE, "Date:%a, %d %b %Y %X UTC\r\n", &time_now)) == 0)	
	{
#ifdef DEBUG
		printf("strftime in get_time.c");
#endif
		return -1;
	}
	return strlen;
}

int get_local_time(char *time_buf)
{
	extern size_t TIME_BUFFER_SIZE;
	time_t	now_sec;
	struct tm time_now;
	setenv("TZ","GMT-8",1);
	tzset();
	if(time(&now_sec) == -1)
	{
#ifdef DEBUG
		printf("time() in get_time.c");
#endif
		return -1;
	}
	if((localtime_r(&now_sec, &time_now)) == NULL)
	{
#ifdef DEBUG
		printf("localtime in get_time.c");
#endif
		return -1;
	}
	int strlen = -1;
	/* strftime maybe not thread safe,but i am not sure */
	if((strlen = strftime(time_buf, TIME_BUFFER_SIZE, "Date:%a, %d %b %Y %X GMT+8\r\n", &time_now)) == 0)	
	{
#ifdef DEBUG
		printf("strftime in get_time.c");
#endif
		return -1;
	}
	return strlen;
}
