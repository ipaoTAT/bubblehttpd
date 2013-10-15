#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

int deal(void *);
void accept_conn(int, void *);
void recv_data(int, void *);
int set_non_block(int);

#endif
