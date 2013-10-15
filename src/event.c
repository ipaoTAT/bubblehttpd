#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include "event.h"

int efd; 

int event_set(my_event_t *ev, int fd, void (*call_back)(int , void*), void *arg)
{
	ev -> fd = fd;
	ev -> callback = call_back;
	ev -> events = 0;
	ev -> argv = arg;
	ev -> status = 0;
	ev -> lastact = time(NULL);
	ev -> keep_alive = 0;
}
int event_add(int efd, int events, my_event_t *ev)
{
	struct epoll_event *epv = (struct epoll_event *)malloc(sizeof(struct epoll_event));
	(epv -> data).fd = ev -> fd;
	(epv -> data).ptr = ev;
	epv -> events = events;
	ev -> events = events;
	int op;
	if(ev -> status == 1)op = EPOLL_CTL_MOD;		//add to the wait list
	else {
		op = EPOLL_CTL_ADD;
	}
	if(epoll_ctl(efd, op, ev -> fd, epv) < 0)
	{
#ifdef DEBUG
		printf("!!Event of fd[%d] add error\n",ev -> fd);
#endif
		return -1;
	}
#ifdef DEBUG
	ev -> status = 1;
	printf("!!Event of fd[%d] add OK\n",ev -> fd);
#endif
	return 0;
}

int event_del(int efd, my_event_t *ev)
{
	struct epoll_event *epv = (struct epoll_event *)malloc(sizeof(struct epoll_event));
	(epv -> data).fd = ev -> fd;
	if(ev -> status != 1)return 0;
	(epv -> data).ptr = ev;
	int op = EPOLL_CTL_DEL;
	if(epoll_ctl(efd, op, ev -> fd, epv) < 0)
	{
#ifdef DEBUG
		printf("Event of fd[%d] delete error\n", ev -> fd);
#endif
		return -1;
	}
	ev -> status = 0;
#ifdef DEBUG
	printf("!!Event of fd[%d] delete OK\n",ev -> fd);
#endif
	return 0;

}

