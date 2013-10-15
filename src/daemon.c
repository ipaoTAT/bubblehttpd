#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "daemon.h"

FILE *log_file; 
FILE *err_file;

int init_daemon()
{
	int pid;
	int i;
	if((pid = fork()) < 0)
	{
		printf("fork() error . daemon.c");
		exit (1);
	}
	if(pid)
	{
		/*parent process exit*/
		exit(0);
	}
	/*child*/
	setsid();

	if((pid = fork()) < 0)
	{
		printf("fork() error . daemon.c");
		exit(1);
	}
	if(pid)
	{
	//	printf("server running , pid = %d\n",pid);
		exit(0);
	}

	signal(SIGUSR1, sig_exit);

	if((log_file = freopen("log/log", "a+", stdout)) == NULL ) 
	{
		exit(1);
	}
	if((err_file = freopen("log/error", "a+", stderr)) == NULL)
	{
		exit(1);
	}

	printf("Start Http Server\n");

}

void sig_exit(int signo)
{
	signal(signo, sig_exit);
	printf("Stop Http Server\n");
	fflush(log_file);
	fflush(err_file);
	fclose(log_file);
	fclose(err_file);
	exit(0);
}
