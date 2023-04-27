//
//  list.c
//  Game
//
//  Created by Thomas Foster on 9/30/22.
//

#include "list.h"
#include <string.h>

typedef struct list_node list_node_t;
struct list_node {
    void * data;
    list_node_t * next;
};

struct list {
    size_t entry_size;
    list_node_t * head;
};

list_t NewList(size_t entry_size)
{
    list_t list;
    list.entry_size = entry_size;

    return list;
}

void ListPush(list_t * list, void * data)
{
    list_node_t * node = calloc(1, sizeof(*node));
    node->data = malloc(list->entry_size);
    memcpy(node->data, data, list->entry_size);
    node->next = list->head;
    list->head = node;
}

// TODO: pop, remove, free etc...
