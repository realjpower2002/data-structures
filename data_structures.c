#include "data_structures.h"

// insert a value into a list by its index
int insert(struct* LinkedList list, int index, void* contents) {
    struct* Node current_node = list->head;

    struct* Node previous_node = NULL;

    for(int i = 0; i <= index; i++) {
        previous_node = current_node;

        if(i >= list->length) {

            // note that the contents for all of these new nodes will
            // be NULL !!!
            current_node->next = malloc(sizeof(struct Node));
            list->length++;

            current_node = current_node->next;
        }
    }

    struct* Node node_to_insert = malloc(sizeof(struct Node));
    node_to_insert->contents = contents;

    list->length = malloc(sizeof(struct Node));

    previous_node->next = node_to_insert;
    node_to_insert->next = current_node;
}

// standard function to get a node's contents from its index (returns NULL on failure)
void* get(struct* LinkedList list, int index) {
    struct* Node current_node = list->head;
    for(int i = 1; i <= index; i++) {
        if(i >= list->length) {
            return NULL;
        }
        current_node = current_node->next;
    }

    return current_node->contents;
}

// get or default (function to return the value at an index or a default one if it doesn't exist)
void* get_or_default(struct* LinkedList list, int index, void* default) {
    struct* Node current_node = list->head;
    for(int i = 1; i <= index; i++) {
        if(i >= list->length) {
            return default;
        }
        current_node = current_node->next;
    }

    return current_node->contents;
}

// function to delete a node using its index. returns 0 on failure (list index out of bounds), 
// positive value on success.
int remove(struct* LinkedList, int) {
    struct* Node current_node = list->head; // current node will be the one we want to remove

    struct* Node previous_node = NULL;

    for(int i = 0; i <= index; i++) {
        previous_node = current_node;

        if(i >= list->length) {
            return NULL;
        }

        current_node = current_node->next;
    }

    previous_node->next = current_node->next;

    free(current_node);

    list->length--;

    return 1;
}

// function to teardown the list
int teardown(struct* LinkedList list) {

    int length = list->length;

    free(list);

    struct* Node current_node = list->next;
    struct* Node previous_node = NULL;

    for(int i = 0; i < length; i++) {
        free(previous_node);

        previous_node = current_node;
        current_node = current_node->next;
    }
    
    free(current_node);

    return 1;
}

struct LinkedList* createLinkedList() {
    struct* LinkedList list = malloc(sizeof(struct LinkedList));

    list->length = 0;
    list->insert = insert;
    list->get = get;
    list->get_or_default = get_or_default;
    list->remove = remove;
    list->teardown = teardown;

    return list;
}