#include "data_structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

// Brilliant little def provided by Mingye Wang (https://stackoverflow.com/questions/5867834/assert-with-message)
#define assertmsg(x, msg) assert(((void) msg, x))

// Handles case where first node in list is not initialized.
int handle_head_uninitialized(struct LinkedList* list) {

    // Handles case where first node in list is not initialized.
    if(list->head == NULL) {
        printf("Initializing head.\n");
        list->head = (struct Node*) malloc(sizeof(struct Node));
        if(list->head == NULL) {
            return 0; //couldn't allocate enough heap for the next node.
        }
        list->length++;
    }

    return 1;
}

int add(struct LinkedList* list, void* contents) {

    printf("Adding value\n");

    if(handle_head_uninitialized(list) == 0) {
        return 0;
    }

    struct Node* prev_node = list->head;
    
    for(int i = 1; i < list->length; i++) {
        printf("traversing node %d\n",i);
        prev_node = prev_node->next;
    }

    struct Node* new_node = malloc(sizeof(struct Node));
    new_node->contents = contents;

    printf("Reassigning old end to new new_node\n");

    prev_node->next = new_node;
    list->length++;

    printf("Added value to the list\n");

    return 1;
}

// insert a value into a list by its index
int insert(struct LinkedList* list, int index, void* contents) {

    assertmsg(list != NULL, "Tried to insert into a NULL Linked List.");

    assertmsg(index >= 0, "Tried to insert into Linked List at negative index.");

    if(handle_head_uninitialized(list) == 0) {
        return 0;
    }

    // Keep a pointer to the nodes which will come before and
    // after the node that we will insert.
    struct Node* next_node = list->head;

    struct Node* prev_node = NULL;

    for(int i = 1; i <= index; i++) {
        prev_node = next_node;

        if(i >= list->length) {

            // note that the contents for all of these new nodes will
            // be NULL !!!
            next_node->next = (struct Node*) malloc(sizeof(struct Node));

            if(next_node->next == NULL) {
                return 0; //couldn't allocate enough heap for the next node.
            }

            list->length++;
        }

        next_node = next_node->next;
    }
 
    // Create new node structure to be inserted into list.
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
    new_node->contents = contents;
    list->length++;

    // if we never advanced through the indices, then prev_node will still
    // be null
    if(prev_node == NULL) {
        list->head = new_node;
        new_node->next = next_node;
    }
    else {
        prev_node->next = new_node;
        new_node->next = next_node;
    }

    return 1;
}

// standard function to get a node's contents from its index (returns NULL on failure)
void* get(struct LinkedList* list, int index) {
    struct Node* current_node = list->head;
    for(int i = 1; i <= index; i++) {
        if(i >= list->length) {
            return NULL;
        }
        current_node = current_node->next;
    }

    return current_node->contents;
}

// get or default (function to return the value at an index or a default one if it doesn't exist)
void* get_or_default(struct LinkedList* list, int index, void* _default) {
    struct Node* current_node = list->head;
    for(int i = 1; i <= index; i++) {
        if(i >= list->length) {
            return _default;
        }
        current_node = current_node->next;
    }

    return current_node->contents;
}

// function to delete a node using its index. returns 0 on failure (list index out of bounds), 
// positive value on success.
int delete(struct LinkedList* list, int index) {
    struct Node* current_node = list->head; // current node will be the one we want to remove

    struct Node* previous_node = NULL;

    for(int i = 1; i <= index; i++) {
        previous_node = current_node;
        current_node = current_node->next;
    }

    if(previous_node == NULL) {
        list->head = current_node->next;
    }
    else {
        previous_node->next = current_node->next;
    }

    free(current_node);

    list->length--;

    return 1;
}

// function to teardown the list
int teardown(struct LinkedList* list) {

    int length = list->length;

    struct Node* current_node = list->head;
    struct Node* previous_node = NULL;

    free(list);

    for(int i = 1; i < length; i++) {
        free(previous_node);

        previous_node = current_node;
        current_node = current_node->next;
    }
    
    free(current_node);

    return 1;
}

struct LinkedList* createLinkedList() {
    struct LinkedList* list = (struct LinkedList*) malloc(sizeof(struct LinkedList));

    list->length = 0;
    list->add = add;
    list->insert = insert;
    list->get = get;
    list->get_or_default = get_or_default;
    list->delete = delete;
    list->teardown = teardown;

    return list;
}