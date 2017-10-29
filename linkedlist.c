#include <stdio.h> //remove it
#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>

void print_list(node_t * head) {
    node_t * current = head;

    while (current != NULL) {
        printf("%s\n", current->port);
        current = current->next;
    }
}

void push(node_t * head, char* p, char* pn) {
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = malloc(sizeof(node_t));
    strcpy(current-> part_number, pn);
    strcpy(current-> port, p);
    current->next->next = NULL;
}

char* find_port(node_t * head, char* pn){
    node_t * current = head;
    if(strcmp(current->part_number, pn) == 0)
        return current->port;
    while (current->next != NULL) {
        current = current->next;
        if(strcmp(current->part_number, pn) == 0)
            return current->port;
    }
    return "inval";
}