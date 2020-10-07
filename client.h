#ifndef CLIENT_H
#define CLIENT_H

struct server_details {
    int server_socket;
};

struct in_addr getIP();
unsigned short getPort();
void* handleServer(void* server_info);

#endif
