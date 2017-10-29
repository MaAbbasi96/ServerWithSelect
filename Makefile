CC=gcc

all: server miniserver client

server: server.o functions.o linkedlist.o
	$(CC) server.o functions.o linkedlist.o -o server

miniserver: miniserver.o functions.o
	$(CC) miniserver.o functions.o -o miniserver

client: client.o functions.o
	$(CC) client.o functions.o -o client

server.o: server.c functions.h
	$(CC) -c server.c -o server.o

miniserver.o: miniserver.c functions.h
	$(CC) -c miniserver.c -o miniserver.o

client.o: client.c functions.h
	$(CC) -c client.c -o client.o

functions.o: functions.h functions.c
	$(CC) -c functions.c

linkedlist.o: linkedlist.h linkedlist.c functions.h
	$(CC) -c linkedlist.c

clean: 
	rm *.o
	rm server
	rm miniserver
	rm client