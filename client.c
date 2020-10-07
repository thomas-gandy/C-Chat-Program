#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "myString.h"
#include "client.h"

int main(int argc, char* argv[]) {
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Failed to create a socket connection\n");
        return -1;
    }
    printf("Created socket.\n");

    struct sockaddr_in server_address;
    unsigned long ip = getIP().s_addr;
    unsigned short port = getPort();

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = ip;
    server_address.sin_port = port;

    int err = connect(sockfd, (struct sockaddr*) &server_address, sizeof(server_address));
    if (err == -1) {
        printf("Failed to connect.\n");
        return -1;
    }
    printf("Connected.\n");

    struct server_details server_info;
    server_info.server_socket = sockfd;
    pthread_t threadID;
    err = pthread_create(&threadID, NULL, handleServer, &server_info);
    if (err == -1) {
        printf("Failed to create a thread.\n");
        close(sockfd);
        return -1;
    }

    const int BUFFER_SIZE = 10000;
    int bytes_sent;
    bool connect = true;
    char buffer[BUFFER_SIZE];
    while (connect) {
        fgets(buffer, BUFFER_SIZE, stdin);
        int stringLen = strlen(buffer);
        if (stringLen <= 1) {
            printf("Closing socket.\n");
            close(sockfd);
            connect = false;
        }

        bytes_sent = send(sockfd, buffer, stringLen, 0);
    }

    return 0;
}

struct in_addr getIP() {
    struct in_addr pin;
    char buffer[16];

    printf("Enter server IP4: ");
    fgets(buffer, 16, stdin);
    inet_aton(buffer, &pin);

    return pin;
}

unsigned short getPort() {
    char buffer[8];

    printf("Enter port number: ");
    fgets(buffer, 8, stdin);

    return htons(atoi(buffer));
}

void* handleServer(void* server_info) {
    printf("Listening for server response.\n");
    struct server_details sd = *((struct server_details*) server_info);
    struct myString s;
    const int BUFFER_SIZE = 10000;
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = recv(sd.server_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        myString_new(&s, "");
        myString_catn(&s, buffer, bytes_read);
        printf("%s", s.string);
        myString_free(&s);
    }
    return NULL;
}
