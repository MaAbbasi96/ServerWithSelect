#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
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

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE], ms_port[6], ms_part_number[3], file_name[100], info[9];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 5 || strlen(argv[2]) > 5 || strlen(argv[3]) > 2) { //argv[1]: connecting to argv[2]: Miniserver Port argv[3]: Part No. argv[4]: file name
        perror("Bad Arguments\n");
        exit(1);
    }

    strcpy(ms_port, argv[2]);
    strcpy(ms_part_number, argv[3]);
    strcpy(file_name, argv[4]);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], CONNECTINGPORT, &hints, &servinfo)) != 0) {
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

    strcpy(info, make_info_for_server(ms_port, ms_part_number));
    send(sockfd, info, strlen(info), 0);
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) < 0) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    print("client: received ");
    print(buf);
    print("\n");

    close(sockfd);

    return 0;
}