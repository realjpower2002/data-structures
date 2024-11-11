#include "data_structures.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    printf("Testing data structures.\n");

    struct LinkedList* list = createLinkedList();

    printf("Created a list.\n");

    unsigned int* my_integer = malloc(sizeof(unsigned int));

    if(my_integer == NULL) {
        printf("Couldn't allocate a single unsigned int ... what in the world is wrong with your heap?\n");
    }

    *my_integer = 0xdeadbeef;

    list->insert(list,0,my_integer);

    printf("My integer is 0x%x\n", *(unsigned int*)list->get(list,0));

    printf("Adding a value ... \n");

    unsigned int* new_int = malloc(sizeof(unsigned int));

    if(new_int == NULL) {
        printf("Couldn't allocate a single unsigned int ... what in the world is wrong with your heap?\n");
    }

    *new_int = 0xfeedface;

    list->add(list,new_int);

    printf("Another integer is 0x%x.\n",*(unsigned int*)list->get(list,2));

    printf("Deleting node @ index 2.\n");

    list->delete(list,2);

    struct Node* cur = list->head;

    printf("Currently %d nodes remain.\n",list->length);

    for(int i = 0; i < list->length; i++) {
        printf("0x%x\n",cur->contents);
        cur = cur->next;
    }

    // destroy the list
    list->teardown(list);
}