#include <stdlib.h>
#include <stdbool.h>

#include "linkedList.h"

struct socket_node* socketExists(struct list_handler* lh, int socket) {
    struct socket_node* node = lh->head;
    while (node) {
        if (node->socketd == socket) return node;
        node = node->next;
    }
    return NULL;
}

struct socket_node* addSocket(struct list_handler* lh, int socket) {
    struct socket_node* temp = (struct socket_node*) malloc(sizeof(struct socket_node));
    temp->socketd = socket;
    temp->prev = lh->tail;
    temp->next = NULL;

    if (lh->tail) lh->tail->next = temp;
    else lh->head = temp;
    lh->tail = temp;

    return temp;
}

bool removeSocket(struct list_handler* lh, int socket) {
    struct socket_node* node = lh->head;
    while (node) {
        if (node->socketd == socket) {
            if (node == lh->head) {
                lh->head = node->next;
                if (node->next) node->next->prev = NULL;
            }
            if (node == lh->tail) {
                lh->tail = node->prev;
                if (node->prev) node->prev->next = NULL;
            }
            free(node);
            return true;
        }
    }
    return false;
}

void freeList(struct list_handler* lh) {
    struct socket_node* node = lh->head;
    while (node) {
        struct socket_node* temp = node->next;
        free(node);
        node = temp;
    }
    lh->nodeCount = 0;
    lh->head = NULL;
    lh->tail = NULL;
}
