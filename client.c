#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "functions.h"

#define CONNECTINGPORT "4000" // the port client will be connecting to

#define MAXDATASIZE 1024 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

char* make_info_for_server(const char* port,const char* part_number){
    int i;
    char *info;
    info = (char*) malloc(9 * sizeof(char));
    strcpy(info, port);
    strcat(info, "/");
    strcat(info, part_number);
    return info;
}

void get_server_port(char* port, char* ports){
    int i;
    char removed_substr[7];
    for(i = 0; i < strlen(ports); i++){
        if(ports[i] == '/')
            break;
        port[i] = ports[i];
    }
    port[i] = '\0';
    strcpy(removed_substr, port);
    strcat(removed_substr, "/");
    remove_substring(ports, removed_substr);
}


int main()
{
    int sockfd, numbytes, file_fd;
    char buf[MAXDATASIZE], info[8], ports[MAXDATASIZE], port[6], file_name[100];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    print("Enter File Name:\n");
    input(file_name);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo("localhost", CONNECTINGPORT, &hints, &servinfo)) != 0) {
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        perror("client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    print("client: connecting to ");
    print(s);
    print("\n");

    freeaddrinfo(servinfo); // all done with this structure

    strcpy(info, "file/");

    send(sockfd, info, strlen(info), 0);
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) < 0) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';
    print("client: received ");
    print(buf);
    print("\n");
    strcpy(ports, buf);
    close(sockfd);

    do{
        get_server_port(port, ports);
        if ((rv = getaddrinfo("localhost", port, &hints, &servinfo)) != 0) {
            return 1;
        }

        // loop through all the results and connect to the first we can
        for(p = servinfo; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype,
                    p->ai_protocol)) == -1) {
                perror("client: socket");
                continue;
            }

            if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(sockfd);
                perror("client: connect");
                continue;
            }

            break;
        }

        if (p == NULL) {
            perror("client: failed to connect\n");
            return 2;
        }

        inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
                s, sizeof s);
        print("client: connecting to ");
        print(s);
        print("\n");

        freeaddrinfo(servinfo); // all done with this structure

        strcpy(info, "file/");

        send(sockfd, info, strlen(info), 0);
        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) < 0) {
            perror("recv");
            exit(1);
        }
        // print("client: received ");
        // print(buf);
        // print("\n");
        buf[numbytes] = '\0';
        file_fd = open(file_name, O_WRONLY | O_APPEND | O_CREAT, 0666);
        write(file_fd, buf, strlen(buf));
        close(file_fd);
        close(sockfd);
    } while(strlen(ports) > 0);

    return 0;
}
