#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct node {
    char port[6], part_number[3];
    struct node * next;
} node_t;

void push(node_t * head, char* p, char* pn);
char* find_port(node_t * head, char* pn);
void print_list(node_t * head);

#endif