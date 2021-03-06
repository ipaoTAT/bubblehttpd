#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include "init_socket.h"


int init_socket(int *listen_fd, struct sockaddr_in *server_addr)
{
	extern short PORT;
	extern int BACKLOG;
	if((*listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)	
	{
		perror("socket() error. in init_socket.c");
		return -1;
	}

	/* set reuse the port on server machine  */
	int opt = SO_REUSEADDR;
	if(setsockopt(*listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		perror("setsockopt() error.  in init_socket.c");
		return -1;
	}
	
	
	
	server_addr->sin_family = AF_INET;
	server_addr->sin_port = htons(PORT);
	server_addr->sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(*listen_fd, (struct sockaddr *)server_addr, sizeof(struct sockaddr_in)) == -1)
	{
		if(errno == EACCES)
		{
			perror("Permission deny, You must be ROOT to bind this port.\nIf you are not, please modify the webconfig.conf ,change the port to a new number greater than 1024\n");
			return -1;
		}
		perror("bind() error.  in init_socket.c");
		return -1;
	}

	return 0;
}
