CFLAG = -O3 -g -fno-stack-protector

CC = gcc

object = server.o session.o response.o request.o init_socket.o threadpool.o event.o config.o time.o http_header.o daemon.o

bubblebin : ${object}
	${CC} ${CFLAG} -pthread ${object} -o bubblebin

webserve.o : server.c init_socket.h session.h
	${CC} ${CFLAG} -c server.c	

session.o : session.c session.h request.h threadpool.h config.h
	${CC} ${CFLAG} -c session.c

response.o : response.c response.h request.h http_header.h
	${CC} ${CFLAG} -c response.c

http_header.o : http_header.c http_header.h response.h time.h

time.o : time.c time.h
	${CC} ${CFLAG} -c time.c

request.o : request.c request.h config.h
	${CC} ${CFLAG} -c request.c

init_socket.o : init_socket.c init_socket.h config.h
	${CC} ${CFLAG} -c init_socket.c 

threadpool.o : threadpool.c threadpool.h
	${CC} ${CFLAG} -c threadpool.c

event.o : event.c event.h
	${CC} ${CFLAG} -c event.c

config.o : config.c config.h
	${CC} ${CFLAG} -c config.c

daemon.o : daemon.c daemon.h
	${CC} ${CFLAG} -c daemon.c


.PHONY : clean
clean :
	-rm -f ${object} bubblebin
	-rm -f ../bin/bubblebin

.PHONY : install
install :
	mkdir ../bin
	mv bubblebin ../bin/

