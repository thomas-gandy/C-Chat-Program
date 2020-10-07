#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

#include "server.h"

struct socket_node {
    int socketd;
    struct socket_node* prev;
    struct socket_node* next;
};

struct list_handler {
    int nodeCount;
    struct socket_node* head;
    struct socket_node* tail;
};

// check if a socket exists in the list
struct socket_node* socketExists(struct list_handler* lh, int socket);
// add a socket to the list
struct socket_node* addSocket(struct list_handler* lh, int socket);
//remove node from the list
bool removeSocket(struct list_handler* lh, int socket);
// free the list from memory
void freeList(struct list_handler* lh);

#endif
