CC=gcc

all: server miniserver

server: server.o functions.o
	$(CC) server.o functions.o -o server

miniserver: miniserver.o functions.o
	$(CC) miniserver.o functions.o -o miniserver

server.o: server.c functions.h
	$(CC) -c server.c -o server.o

miniserver.o: miniserver.c functions.h
	$(CC) -c miniserver.c -o miniserver.o

functions.o: functions.h functions.c
	$(CC) -c functions.c

clean: 
	rm *.o
	rm server
	rm miniserver