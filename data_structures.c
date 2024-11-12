/**
 * @file data_structures.c
 * 
 * @brief Provides implementations for function pointers stored in different 
 * data structures defined in "data_structures.h"
 * 
 * @author James Power
 * 
 * This file provides implementations for the function pointers in the data 
 * structures defined in "data_structures.h", as well as functions to construct 
 * the data structures. Currently, this includes :
 * 
 *     • LinkedList
 * 
 * These structs all are designed to accept void pointers to data as their 
 * contents. This means that any type of data can be stored in these data 
 * structure primitives, and it is up to the programmer to keep track of what 
 * type of data is being stored in which parts of the structure, and dereference 
 * the data properly.
 * 
 * These structures are designed to store heap data, allocated to pointers using 
 * malloc. For those of you who are not knowledgeable about program memory, be 
 * careful when trying to store stack variables in these structures. If the 
 * scope of a stack variable allocated in the list ends, you may not get the 
 * data that you expect when you access it again, as that memory address will 
 * very quickly be used for something else.
 */



#include "data_structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>


// Brilliant little def provided by Mingye Wang 
//     (https://stackoverflow.com/questions/5867834/assert-with-message)
#define assertmsg(x, msg) assert(((void) msg, x))



/**
 * @brief Handles the case during insertion or addition of node where the head 
 * is uninitialized, and has to be allocated separately before the rest of the 
 * nodes in the list can be traversed.
 * 
 * @remark Also increments the length of the list by 1 on success.
 * 
 * @param list - The list to check if its head is uninitialize it, and
 * initialize it if so.
 * 
 * @return 0 on failure (not enough heap to allocate head), 1 on success.
 */
int handle_head_uninitialized(struct LinkedList* list) {

    assertmsg(list != NULL, "Tried to allocate head for NULL Linked List.");

    if(list->head == NULL) {

        // Allocate new space on the heap for the head.
        list->head = (struct Node*) malloc(sizeof(struct Node));

        if(list->head == NULL) {
            return 0; //couldn't allocate enough heap for the next node.
        }

        list->length++;
    }

    return 1;
}

/**
 * @brief Adds a new node with contents "contents" to the end of the list. This
 * contents is a void pointer to some data in memory.
 * 
 * @remark Also increments the length of the list by 1 on success.
 * 
 * @param list - The list to add the new node to.
 * @param contents - The contents to include in the node.
 * 
 * @return 0 on failure (not enough heap to allocate new node), 1 on success.
 */
int add(struct LinkedList* list, void* contents) {

    assertmsg(list != NULL, "Tried to insert into a NULL Linked List.");

    // This Node will traverse through to the end of the list.
    struct Node* prev_node = list->head;
    
    // Traversing list over [1,length) (since we start at the head).
    for(int i = 1; i < list->length; i++) {
        prev_node = prev_node->next;
    }

    // Create new node for the data @ contents
    struct Node* new_node = malloc(sizeof(struct Node));

    // Check for NULL on malloc (no more space in heap)
    if(new_node == NULL)
        return 0;

    new_node->contents = contents;

    // If we never traversed any nodes, then prev_node will be NULL.
    // This means that the list is empty.
    if(prev_node == NULL) {
        list->head = new_node;
    }
    else {
        // Else, just link the node at the end of the list to the node
        // with our new contents
        prev_node->next = new_node;
    }

    // Increment size of list
    list->length++;

    // Return 1 (success)
    return 1;
}

/**
 * @brief Inserts a new node with contents "contents" into a list at a given
 * index. This contents is a void pointer to some data in memory.
 * 
 * @remark The user may insert past the end of the list, in which case new nodes
 * will be created up to the desired index. These nodes will have all NULL
 * pointers to data. Also increments the length of the list by 1 on success.
 * 
 * @param list - The list to insert the new node into.
 * @param index - The index at which to insert the new node.
 * @param contents - The contents to include in the node.
 * 
 * @return 0 on failure (not enough heap to allocate new node), 1 on success.
 */
int insert(struct LinkedList* list, int index, void* contents) {

    assertmsg(list != NULL, "Tried to insert into a NULL Linked List.");

    assertmsg(index >= 0, "Tried to insert into Linked List at negative \
    index.");

    // Keep a pointer to the nodes which will come before and after the node 
    // that we will insert.
    struct Node* next_node = list->head;

    struct Node* prev_node = NULL; // prev_node starts at null because nothing
                                   // is before the head

    // Traverses list over [0,index), because prev_node starts from the position
    // 0, before the head of the list.
    for(int i = 0; i < index; i++) {

        // Advance prev_node by 1
        prev_node = next_node;

        // If we have gone past the end of the list, keep trying to insert
        // more nodes to fill the gap
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

    // If malloc returns NULL, there is not enough space left in the heap
    if(new_node == NULL)
        return 0;

    new_node->contents = contents;
    list->length++;

    // If we never traversed any nodes, then prev_node will be NULL.
    // This means that the list is empty.
    if(prev_node == NULL) {
        list->head = new_node;
        new_node->next = next_node;
    }
    else {
        // Else, just link the Node before our desired index to the new Node
        // we created, and link our new Node to the next Node past our index.
        prev_node->next = new_node;
        new_node->next = next_node;
    }

    // Return 1 on success.
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
        if(i >= list->length)
            return 0;
        
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