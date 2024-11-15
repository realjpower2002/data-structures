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
 * @returns 0 on failure (not enough heap to allocate head), 1 on success.
 */
int handle_head_uninitialized(struct LinkedList* list) {

    assertf(list != NULL, "Tried to allocate head for NULL Linked List.\n");

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
 * @param list - The list to add the new Node to.
 * @param contents - The contents to include in the node.
 * 
 * @returns 0 on failure (not enough heap to allocate new node), 1 on success.
 */
int add(struct LinkedList* list, void* contents) {

    assertf(list != NULL, "Tried to insert into a NULL Linked List.\n");

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
 * @param list - The list to insert the new Node into.
 * @param index - The index at which to insert the new node.
 * @param contents - The contents to include in the node.
 * 
 * @returns 0 on failure (not enough heap to allocate new node), 1 on success.
 */
int insert(struct LinkedList* list, int index, void* contents) {

    assertf(list != NULL, "Tried to insert into a NULL Linked List.\n");

    assertf(index >= 0, "Tried to insert into Linked List at negative index.\n");

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



/**
 * @brief Returns the contents, a void pointer, of a the node at the given
 * index.
 * 
 * @remark This function will crash the program upon trying to obtain an
 * index outside the bounds of the list, which is (-inf,0)U[length,inf) .
 * 
 * @param list - The list to obtain the desired Node from.
 * @param index - The index from which the desired Node will be obtained.
 * 
 * @returns NULL on failure, void* to contents of desired Node on success.
 */
void* get(struct LinkedList* list, int index) {
    assertf(list != NULL, "Tried to get data from a NULL Linked List.\n");

    assertf(index >= 0 && index < list->length, "Tried to get data from Node at invalid index in Linked List.\n");
    
    // We declare a new node, which will traverse the list up to the index
    struct Node* current_node = list->head;

    // We traverse the list starting from 1 (since we start at the head)
    for(int i = 1; i <= index; i++) {
        current_node = current_node->next;
    }

    // Return contents of desired node
    return current_node->contents;
}



/**
 * @brief Returns the contents, a void pointer, of a the node at the given
 * index. If there is no node at the given index, returns a default value
 * instead.
 * 
 * @param list - The list to obtain the desired Node from.
 * @param index - The index from which the desired Node will be obtained.
 * @param _default - A default value to return if the index does not exist in
 * the list
 * 
 * @returns A void pointer to the contents of the node at the desired index in 
 * the list, default otherwise.
 */
void* get_or_default(struct LinkedList* list, int index, void* _default) {

    assertf(list != NULL, "Tried to get data from a NULL Linked List.\n");

    // Return default if index does not exist
    if(index < 0 || index >= list->length)
        return _default;

    // Declare new node to traverse list
    struct Node* current_node = list->head;

    // Traverse list starting at 1 (since we start at the head)
    for(int i = 1; i <= index; i++) {
        current_node = current_node->next;
    }

    return current_node->contents;
}



/**
 * @brief Deletes the Node from the list at a given index.
 * 
 * @remark This function will decrement the size of the list by 1 on success.
 * This function also may result in a double free if two nodes are removed one
 * after another which have contents that are identical.
 * 
 * @param list - The list to delete desired Node from.
 * @param index - The index of the Node to be deleted.
 * 
 * @returns 0 on failure (index does not exist in list), 1 on success.
 */
int delete(struct LinkedList* list, int index, ...) {

    // Quick check to see if the no auto free is set

    int auto_free = 1; // auto free is true by default

    va_list args;
    va_start(args, index);
    if(va_arg(args, long long) == NO_AUTO_FREE) {
        auto_free = 0;
    }

    // Return 0 if index is out of bounds for the list
    if(index < 0 || index >= list->length)
        return 0;

    // Current node will be the one we want to remove. We keep previous to
    // eventually link around current.
    struct Node* current_node = list->head;

    struct Node* previous_node = NULL;

    // Traverse list starting from 1 (current_node starts at the head, and
    // current_node is the one that we will eventually delete)
    for(int i = 1; i <= index; i++) {
        previous_node = current_node;
        current_node = current_node->next;
    }

    // If we have not traversed any nodes, then we must delete the head
    // and link around it.
    if(previous_node == NULL) {
        list->head = current_node->next;
    }
    else {
        // Else, we delete current and link previous_node around it
        previous_node->next = current_node->next;
    }

    // Free the contents of this node.
    if(current_node->contents != NULL && auto_free)
        free(current_node->contents);

    // Free current node after unlinking it
    free(current_node);

    // Decrement length
    list->length--;

    // Return 1 on success
    return 1;
}



/**
 * @brief Frees the list, all of its nodes, and all of the nodes' contents.
 * 
 * @remark This function may result in a double free if there are two identical
 * pointers in different contents in different nodes in the list.
 * 
 * @param list - The list to tear down.
 * 
 * @returns 1 on success.
 */
int teardown(struct LinkedList* list, ...) {

    // Quick check to see if the no auto free is set

    int auto_free = 1; // auto free is true by default

    va_list args;
    va_start(args, list);
    if(va_arg(args, long long) == NO_AUTO_FREE) {
        auto_free = 0;
    }

    // Get the length of the list (number of nodes to traverse)
    int length = list->length;

    // Traverse list starting from head
    struct Node* current_node = list->head;
    struct Node* previous_node = NULL;

    // Free the list itself
    free(list);

    // Traverse starting from index 1 (since we have already seen head)
    for(int i = 1; i < length; i++) {

        // Move to next node
        previous_node = current_node;
        current_node = current_node->next;

        // If the contents of the node is not NULL, free it.
        //     Note, this can result in a double free if there are
        //     two identical pointers in the contents fields of two
        //     nodes in the list !
        if(previous_node->contents != NULL && auto_free)
            free(previous_node->contents);
        
        // Then, free the node
        free(previous_node);
    }
    
    // Free the last node in the list
    if(current_node->contents != NULL && auto_free)
        free(current_node->contents);
    free(current_node);

    // Return 1 on success
    return 1;
}



// This is used for getting the number of arguments passed to the copy
// macros
int get_num_args(char* macro_va_args) {
    if(strlen(macro_va_args) == 0) {
        return 0;
    }

    // If there were arguments passed to one of the macros
    int num_macro_va_args = 1;

    // We retrieve the number of bytes to allocate (the first argument 
    // passed to the macro)
    char* num_bytes_string = strtok(macro_va_args, ",");
    
    while(strtok(NULL, ",") != NULL) {
        num_macro_va_args++; // This means we have more than one arg
    }

    return num_macro_va_args;
}



/**
 * @brief Allocates, instantiates, and returns a new LinkedList, with length
 * 0 and function pointers to all of the above functions. 
 * 
 * @returns New LinkedList with 0 length and default function pointers.
 */
LinkedList createLinkedList() {
    LinkedList list = (LinkedList) malloc(sizeof(struct LinkedList));

    list->length = 0;
    list->add = add;
    list->insert = insert;
    list->get = get;
    list->get_or_default = get_or_default;
    list->delete = delete;
    list->teardown = teardown;

    return list;
}