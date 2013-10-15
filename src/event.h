#include<sys/epoll.h>

#define MAX_EVENTS 100000
#define BUF_SIZE 128
#define EPOLL_ACC 0
#define EPOLL_RECV 1

struct my_event_t
{
	int fd;
	void (*callback)(int fd, void *argv);
	int events;		//1: recv data, 0: accept connections
	void *argv;
	int status; 		//1:be in epoll wait; 0: be not in
	//char buf[BUF_SIZE];
	//int len, s_offset; 	// ?
	long lastact;	//last active time
	int keep_alive;		//is keep-alive? 0:no, else: alive-time
};

typedef struct my_event_t my_event_t;

extern int efd;
my_event_t my_events[MAX_EVENTS];

int listen_fd;

int event_add(int , int ,my_event_t *);

int event_del(int , my_event_t *);

int event_set(my_event_t *, int , void (*call_back)(int, void*), void*);
