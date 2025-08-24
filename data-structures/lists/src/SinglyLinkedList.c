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


#include "../headers/SinglyLinkedList.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>




/* ========================================================================== */
/*                             SINGLY LINKED LIST                             */
/* ========================================================================== */

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
int handle_head_uninitialized(struct SinglyLinkedList* list) {

    assertf(list != NULL, "Tried to allocate head for NULL Linked List.\n");

    if(list->head == NULL) {

        // Allocate new space on the heap for the head.
        list->head = (struct SLLNode*) malloc(sizeof(struct SLLNode));

        if(list->head == NULL) {
            return 0; //couldn't allocate enough heap for the next node.
        }

        list->length++;
    }

    return 1;
}



/**
 * @brief Appends a new node with contents "contents" to the end of the list. 
 * This contents is a void pointer to some data in memory.
 * 
 * @remark Also increments the length of the list by 1 on success.
 * 
 * @param list - The list to append the new Node to.
 * @param contents - The contents to include in the node.
 * 
 * @returns 0 on failure (not enough heap to allocate new node), 1 on success.
 */
int append(struct SinglyLinkedList* list, void* data, long num_bytes, ...) {

    assertf(list != NULL, "Tried to insert into a NULL Linked List.\n");

    // This Node will traverse through to the end of the list.
    struct SLLNode* prev_node = list->head;
    
    // Traversing list over [1,length) (since we start at the head).
    for(int i = 1; i < list->length; i++) {
        prev_node = prev_node->next;
    }

    // Create new node for the data @ contents
    struct SLLNode* new_node = malloc(sizeof(struct SLLNode));

    // Check for NULL on malloc (no more space in heap)
    if(new_node == NULL)
        return 0;

    // Add contents parameter to new node
    if(num_bytes == IGNORE) {
        new_node->data = data;
        new_node->num_bytes = IGNORE;
    }
    else if(num_bytes == AUTO) {
        new_node->data = data;
        new_node->num_bytes = sizeof(data);
    }
    else if(num_bytes == LITERAL) {
        new_node->data = (void*) data;
        new_node->num_bytes = LITERAL;
    }
    else { // if the number of bytes is a valid integer value
        new_node->data = data;
        new_node->num_bytes = num_bytes;
    }

    // If type awareness is enabled, then also include the type information
    #ifdef TYPE_AWARE
    va_list args;
    va_start(args, num_bytes);
    new_node->type = va_arg(args, char*);
    #endif

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
int insert(struct SinglyLinkedList* list, int index, void* data, long num_bytes, ...) {

    assertf(list != NULL, "Tried to insert into a NULL Linked List.\n");

    assertf(index >= 0, "Tried to insert into Linked List at negative index.\n");

    // Keep a pointer to the nodes which will come before and after the node 
    // that we will insert.
    struct SLLNode* next_node = list->head;

    // prev_node starts at null because nothing is before the head
    struct SLLNode* prev_node = NULL;

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
            next_node->next = (struct SLLNode*) malloc(sizeof(struct SLLNode));

            if(next_node->next == NULL) {
                return 0; //couldn't allocate enough heap for the next node.
            }

            list->length++;
        }

        next_node = next_node->next;
    }
 
    // Create new node structure to be inserted into list.
    struct SLLNode* new_node = (struct SLLNode*) malloc(sizeof(struct SLLNode));

    // If malloc returns NULL, there is not enough space left in the heap
    if(new_node == NULL)
        return 0;

    // Add contents parameter to new node
    if(num_bytes == IGNORE) {
        new_node->data = data;
        new_node->num_bytes = IGNORE;
    }
    else if(num_bytes == AUTO) {
        new_node->data = data;
        new_node->num_bytes = sizeof(data);
    }
    else if(num_bytes == LITERAL) {
        new_node->data = (void*) data;
        new_node->num_bytes = LITERAL;
    }
    else { 
        // if the number of bytes is a valid integer value
        new_node->data = data;
        new_node->num_bytes = num_bytes;
    }

    // If type awareness is enabled, then also include the type information
    #ifdef TYPE_AWARE
    va_list args;
    va_start(args, num_bytes);
    new_node->type = va_arg(args, char*);
    #endif

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



// Notice, we only check ASCII on failure. This makes it so, while the program
// is running properly, we only loop through the type string once to inform
// the user of a type mismatch and debugging information.
char* getTypeErrorString(char* output, char* actual_type, char* expected_type,
                         int index) {
    char* actual_unassigned = !IS_ASCII(actual_type) ? "(not printable - type "
                                                      "was likely not "
                                                      "provided when inserting "
                                                      "data into list)" : 
                                                      "";

    char* expected_unassigned = !IS_ASCII(expected_type) ? "(not printable - type "
                                                      "was likely not provided "
                                                      "when retrieving data "
                                                      "from list)" : 
                                                      "";

    puts("Got here (GetTypeErrorString)\n");

    sprintf(output,"Type check failed - tried to read \"%s\" %s at index %d in "
    "Singly Linked List as \"%s\" %s.", actual_type, actual_unassigned,
    index, expected_type, expected_unassigned);

    if(!strcmp(actual_unassigned, expected_unassigned)) {
        sprintf(output, "%s The type strings are identical. The type system "
        "works using string literals. Do not attempt to use a stack or heap "
        "string to specify the type!", output);
    }

    puts("Got here!");

    puts(output);

    return output; 
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
void* get(struct SinglyLinkedList* list, int index, ...) {
    assertf(list != NULL, "Tried to get data from a NULL Linked List.\n");

    assertf(index >= 0 && index < list->length, "Tried to get data from Node at invalid index in Linked List.\n");
    
    // We declare a new node, which will traverse the list up to the index
    struct SLLNode* current_node = list->head;

    // We traverse the list starting from 1 (since we start at the head)
    for(int i = 1; i <= index; i++) {
        current_node = current_node->next;
    }

    #ifdef TYPE_AWARE
    va_list args;
    va_start(args, index);
    char* expected = va_arg(args, char*);
    if(current_node->type != expected) {
        char* error = getTypeErrorString(error, current_node->type, expected,
                                        index);
        assertf(current_node->type == expected, "%s", error);
    }
    #endif

    // Return contents of desired node
    return current_node->data;
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
void* get_or_default(struct SinglyLinkedList* list, int index, void* _default, ...) {

    assertf(list != NULL, "Tried to get data from a NULL Linked List.\n");

    // Return default if index does not exist
    if(index < 0 || index >= list->length)
        return _default;

    // Declare new node to traverse list
    struct SLLNode* current_node = list->head;

    // Traverse list starting at 1 (since we start at the head)
    for(int i = 1; i <= index; i++) {
        current_node = current_node->next;
    }

    printf("Got here.\n");

    #ifdef TYPE_AWARE
    va_list args;
    va_start(args, _default);
    char* expected = va_arg(args, char*);
    printf("Got here.\n");
    if(current_node->data == NULL || current_node->type == expected) {
        printf("Got here.\n");
        char* error = getTypeErrorString(error, current_node->type, expected,
                                        index);
        printf("Got here.\n");
        printf("Types : %s, %s\n", current_node->type, expected);
        printf("%s",error);
        assertf(current_node->type == expected, "%s", error);
    }
    #endif

    return current_node->data;
}



/**
 * @brief Returns the number of bytes pointed to by the data for a Node
 * at a given index.
 * 
 * @remark This value is not guaranteed to be correct! The user can choose
 * to not log the amount of bytes stored at given nodes in node->num_bytes,
 * by using the IGNORE keyword, and the sizeof() operater invoked when using
 * the AUTO keyword is not always reliable. Be sure to always be scrupulous
 * when keeping track of memory usage. Also, it is possible for literal values
 * to be written onto the contents pointer, in which case this function will
 * return 0 bytes (corresponds to keyword LITERAL).
 * 
 * @param list - The list to obtain the number of bytes pointed to by a given
 * Node's contents void*.
 * @param index - The index in the list at which the desired Node resides.
 * 
 * @returns The value stored in the long at Node->num_bytes. Should be checked
 * against IGNORE and LITERAL.
 */
long get_data_size(struct SinglyLinkedList* list, int index) {
    assertf(list != NULL, "Tried to get num_bytes from a NULL Linked List.\n");

    assertf(index >= 0 && index < list->length, "Tried to get num_bytes from Node at invalid index in Linked List.\n");
    
    // We declare a new node, which will traverse the list up to the index
    struct SLLNode* current_node = list->head;

    // We traverse the list starting from 1 (since we start at the head)
    for(int i = 1; i <= index; i++) {
        current_node = current_node->next;
    }

    // Return num_bytes pointed to by contents of desired node
    return current_node->num_bytes;
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
int delete(struct SinglyLinkedList* list, int index, ...) {

    // Quick check to see if the no auto free is set

    // auto free is true by default
    int auto_free = 1;

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
    struct SLLNode* current_node = list->head;

    struct SLLNode* previous_node = NULL;

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
    if(current_node->data != NULL && auto_free)
        free(current_node->data);

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
int teardown(struct SinglyLinkedList* list, ...) {

    // Quick check to see if the no auto free is set

    // auto free is true by default
    int auto_free = 1;

    va_list args;
    va_start(args, list);
    if(va_arg(args, long long) == NO_AUTO_FREE) {
        auto_free = 0;
    }

    // Get the length of the list (number of nodes to traverse)
    int length = list->length;

    // Traverse list starting from head
    struct SLLNode* current_node = list->head;
    struct SLLNode* previous_node = NULL;

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
        if(previous_node->data != NULL && auto_free)
            free(previous_node->data);
        
        // Then, free the node
        free(previous_node);
    }
    
    // Free the last node in the list
    //
    // (The only case where current_node is NULL is if we have an empty list).
    if(current_node != NULL) {
        if(current_node->data != NULL && auto_free) {
            free(current_node->data);
        }
        free(current_node);
    }

    // Return 1 on success
    return 1;
}



#ifdef TYPE_AWARE
/**
 * @brief Returns the stringized literal of the type token optionally passed
 * into the append() or insert() functions for a node.
 * 
 * @remark This function returns NULL if the data field is undeclared. This is
 * a workaround so that the type is still not a required parameter for the 
 * append() and insert() functions, but so that the get_or_default_type macro
 * in type-aware mode can eliminate the type check or not before making its
 * assertion.
 * 
 * @param list - The list to obtain the data type of the desired Node from.
 * @param index - The index in the list at which the desired Node resides.
 * 
 * @returns A stringized version of the type token passed for a value appended
 * or inserted into the list, or NULL if the value's
 */
long get_stringized_type(struct SinglyLinkedList* list, int index) {
    assertf(list != NULL, "Tried to get stringized type from a Node in a NULL Linked List.\n");

    assertf(index >= 0 && index < list->length, "Tried to get stringized type from Node at invalid index in Linked List.\n");
    
    // We declare a new node, which will traverse the list up to the index
    struct SLLNode* current_node = list->head;

    // We traverse the list starting from 1 (since we start at the head)
    for(int i = 1; i <= index; i++) {
        current_node = current_node->next;
    }

    // Return num_bytes pointed to by contents of desired node
    return current_node->num_bytes;
}
#endif



/**
 * @brief Allocates, instantiates, and returns a new LinkedList, with length
 * 0 and function pointers to all of the above functions. 
 * 
 * @returns New LinkedList with 0 length and default function pointers.
 */
SinglyLinkedList createSinglyLinkedList() {
    SinglyLinkedList list = (SinglyLinkedList) malloc(sizeof(struct SinglyLinkedList));

    list->length = 0;
    list->append = append;
    list->insert = insert;
    list->get = get;
    list->get_or_default = get_or_default;
    list->get_data_size = get_data_size;
    list->delete = delete;
    list->teardown = teardown;

    return list;
}