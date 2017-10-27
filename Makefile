CC=gcc

all: server miniserver

server.o: server.c
	$(CC) -c server.c -o server.o

server: server.o
	$(CC) server.o -o server

miniserver.o: miniserver.c
	$(CC) -c miniserver.c -o miniserver.o

miniserver: miniserver.o
	$(CC) miniserver.o -o miniserver

functions.o: functions.c functions.h
	$(CC) -c functions.c

clean: 
	rm *.o
	rm server
	rm miniserver