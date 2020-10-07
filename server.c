#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#include "myString.h"
#include "linkedList.h"


struct list_handler lh;

int main(int argc, char* argv[]) {
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Failed to create a socket connection\n");
        return -1;
    }
    printf("Created socket.\n");

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr = getIP();
    server_address.sin_port = getPort();

    int bind_val = bind(sockfd, (struct sockaddr*) &server_address, sizeof(server_address));
    if (bind_val == -1) {
        printf("Failed to bind to socket.\n");
        return -1;
    }
    printf("Bound to socket.\n");

    int err = listen(sockfd, BACKLOG);
    if (err == -1) {
        printf("Failed to listen.\n");
        return -1;
    }
    printf("Listening.\n");

    struct sockaddr_in client_address;
    bool shouldDisconnect = false;
    lh.nodeCount = 0;
    lh.head = NULL;
    lh.tail = NULL;
    while (!shouldDisconnect) {
        socklen_t socketStructSize = sizeof(client_address);
        int client_socket = accept(sockfd, (struct sockaddr*) &client_address, &socketStructSize);
        if (client_socket == -1) {
            printf("Failed to accept client.\n");
            continue;
        }
        printf("Accepted client.\n");

        addSocket(&lh, client_socket);

        struct client_details client_info;
        client_info.client_socket = client_socket;
        pthread_t threadID;
        int err = pthread_create(&threadID, NULL, handleClient, &client_info);
        if (err == -1) {
            printf("Failed to create a thread.\n");
            close(client_socket);
            continue;
        }
    }

    printf("Closing socket.\n");
    close(sockfd);

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

void* handleClient(void* client_info) {
    struct client_details cd = *((struct client_details*) client_info);
    struct myString s;
    const int BUFFER_SIZE = 10000;
    char buffer[BUFFER_SIZE];
    int bytes_read;
    char* anonymous = "ANONYMOUS: ";

    while ((bytes_read = recv(cd.client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        struct socket_node* node = lh.head;
        while (node) {
            if (node->socketd != cd.client_socket) {
                send(node->socketd, anonymous, strlen(anonymous), 0);
                send(node->socketd, buffer, bytes_read, 0);
            }
            node = node->next;
        }
    }
    printf("Client disconnected.\n");
    close(cd.client_socket);
    removeSocket(&lh, cd.client_socket);
    return NULL;
}
