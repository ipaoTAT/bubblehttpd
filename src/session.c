#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>
#include "config.h"
#include "event.h"
#include "request.h"
#include "threadpool.h"
#include "session.h"

extern int listen_fd;
extern int efd;

int deal(void *ptr)
{
	my_event_t *ev = (my_event_t *)ptr;

	pool_add_task(rdpool, ev -> callback, ev -> fd, ev -> argv);
	return 0;
}

void accept_conn(int fd, void *argv)
{
	int nfd;

	struct sockaddr_in sin;
	socklen_t len = sizeof(struct sockaddr);

	if((nfd = accept(fd, (struct sockaddr *)&sin, &len)) < 0)
	{
#ifdef DEBUG
		perror("accept() error . in http_session.c\n");
#endif
		return;
	}

	int i = nfd;
	//set_non_block(nfd);
	if(i > MAX_EVENTS || my_events[i].status == 1)
	{
		for(i = MAX_EVENTS; i > 0 && my_events[i].status == 1; i--);
		if(i == 0)
		{
			perror("events overflow!\n");
			return;	/*events full*/
		}
	}
	event_set(&my_events[i], nfd, recv_data, &my_events[i]);

	if(event_add(efd, EPOLLIN | EPOLLET, &my_events[i]) < 0)
	{
		return;
	}
	return;
}

void recv_data(int fd, void *arg)
{

//	char req_buf[RECV_BUFFER_SIZE];
	char *req_buf = (char *)malloc(sizeof(char) *RECV_BUFFER_SIZE);
	int char_num;
	char_num = recv(fd, req_buf, RECV_BUFFER_SIZE, 0);


	if(char_num < 0)
	{
#ifdef DEBUG
		fprintf(stderr, "[%d] : recv data error\n",fd);
#endif
		return;
	}
	else if(char_num == 0)
	{
#ifdef DEBUG
		printf("socket [ %d ] : closed\n",fd);
#endif
		if(event_del(efd, (my_event_t *)arg) == 0)close(fd);
		return;
	}
#ifdef DEBUG
	printf("#############################\n");
	printf("recved :\n%s\n", req_buf);
	printf("#############################\n");
#endif
	
	pool_add_task(procpool, deal_req, fd, req_buf);
	return;
}

int set_non_block(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	if(fcntl(fd, F_SETFL, flags) == -1)
	{
		perror("set non block error!\n");
		return -1;
	}
	return 0;
}
