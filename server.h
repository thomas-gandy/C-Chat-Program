#ifndef SERVER_H
#define SERVER_H

#define MAX_CLIENT_CONNECTIONS 16
#define BACKLOG 10

struct client_details {
    int client_socket;
};

struct in_addr getIP();
unsigned short getPort();
void* handleClient(void* client_info);

#endif
