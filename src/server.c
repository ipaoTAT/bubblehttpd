#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include "config.h"
#include "init_socket.h"
#include "session.h"
#include "event.h"
#include "threadpool.h"
#include "time.h"
#include "daemon.h"

extern int efd;
extern int listen_fd;

int main(int argc, char *argv[])
{

	init_daemon();

	signal(SIGPIPE, SIG_IGN);

	/* initial log */
	/*done in init_daemon()*/
//	FILE *f = freopen("log", "a+", stdout);
//	freopen("../log/error", "a+", stderr);


	char start_time[TIME_BUFFER_SIZE];
	get_local_time(start_time);

	printf("Server Start @ %s",start_time);
//	fflush(f);
	fprintf(stderr, "Server Start @ %s",start_time);
	
	/* read configure file from web server config path */
	if(config() == -1)
	{
#ifdef DEBUG
		printf("Read webconfig.conf, use the default values [error]\n");
#endif
	}
	
#ifdef DEBUG
	printf("Read webconfig.conf [ok]\n");
#endif
	
	/*  */
	extern char WEB_ROOT_PATH[];
#ifdef DEBUG
	printf("WEB_ROOT_PATH in main: %s\n", WEB_ROOT_PATH);
#endif
	if(chdir(WEB_ROOT_PATH) == -1)
	{
		perror("chdir error");	
		return -1;
	}
	
	/* initial the thread pool */
	rdpool = create_pool(CPU_NUM * 5);
	wtpool = create_pool(CPU_NUM * 5);
	procpool = create_pool(CPU_NUM * 5);

	/*********************************************************
	* create the server , and prepare to accept client connect
	**********************************************************/
	struct sockaddr_in server_addr;
	//struct sockaddr_in client_addr;
	bzero(&server_addr, sizeof(struct sockaddr_in));
	bzero(&server_addr, sizeof(struct sockaddr_in));

	if(init_socket(&listen_fd, &server_addr) == -1)
	{
		perror("init_socket() error. in server.c [error]\n");
		exit(EXIT_FAILURE);
	}

#ifdef DEBUG
	printf("initilize server socket [ok]\n");
#endif
	//----------dxf: init epoll-----------------------

	set_non_block(listen_fd);
		
	efd = epoll_create(MAX_EVENTS);
	event_set(&my_events[0], listen_fd, accept_conn, NULL);
	event_add(efd, EPOLLIN | EPOLLET, &my_events[0]);

	if(listen(listen_fd, BACKLOG) == -1)
	{
		perror("listen() error.  in init_socket.c");
		return -1;
	}

	printf("Listening at Port %d ...\n",PORT);

	struct epoll_event events[MAX_EVENTS];

	int i;

	while(1)
	{
		int fds = epoll_wait(efd, events, MAX_EVENTS, 2);

		if(fds < 0)
		{
			perror("epoll_wait() error . in server.c");
			break;
		}

		for(i = 0; i < fds; i++)
		{
//			printf("%d\n",tfd);
			deal(events[i].data.ptr);
		}
		
		// timeout logic
	}

	printf("abnormal exit!\n");
}
