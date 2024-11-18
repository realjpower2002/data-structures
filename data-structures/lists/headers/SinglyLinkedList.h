/**
 * @file data_structures.h
 * 
 * @brief Provides struct and type definitions for different data structure 
 * primitives (Lists, Maps, etc).
 * 
 * @author James Power
 * 
 * This file provides struct and type definitions for different data structure 
 * primitives. Currently, this includes :
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
 * data thatyou expect when you access it again, as that memory address will 
 * very quickly be used for something else.
 */

#ifndef SINGLY_LINKED_LIST
#define SINGLY_LINKED_LIST

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



struct SinglyLinkedList {

    /* ====================================================================== */
    /*                          LIST STANDARD FIELDS                          */
    /* ====================================================================== */

    // Stores length of list
    int length;

    // Keyword to ignore num_bytes in append and insert (keep as -1)
    #define IGNORE -1

    // Keyword to log num_bytes in add and insert using sizeof
    #define AUTO -2

    // Keyword arg to write value onto void* instead of
    // heap memory location
    #define LITERAL 0 

    // Add a new node with contents "contents" pointing to 
    // heap memory allocation with num bytes "num_bytes", or
    // const quad value, to the end of the list. Also has 
    // optional argument - pointer to stringized type.
    int (*append)(struct SinglyLinkedList* list, void* data, long num_bytes, ...);

    // Insert a new node with contents "contents" pointing to 
    // heap memory allocation with num bytes "num_bytes", or
    // const quad value, into a given index in the list. Also
    // has optional argument - pointer to stringized type.
    int (*insert)(struct SinglyLinkedList* list, int index, void* data, long num_bytes, ...);

    // Get a pointer to the contents of a node from its index in 
    // the list (returns NULL on failure)
    void* (*get)(struct SinglyLinkedList* list, int index, ...);

    // Get a pointer to the contents of a node from its index in 
    // the list (returns pointer to default data on failure).
    void* (*get_or_default)(struct SinglyLinkedList*, int index, void* _dafault, ...);

    // Get the number of bytes pointed to by the contents pointer
    // of the Node at a given index in a given list.
    long (*get_data_size)(struct SinglyLinkedList* list, int index);

    // Defines flag variable to contraindicate automatic freeing of data
    // stored in list during teardown and delete
    #define NO_AUTO_FREE 0x29167d497c28d39d

    // Delete a node at a given index from the list. (And optionally, you
    // can choose to simply unlink a Node without freeing its contents by 
    // passing NO_AUTO_FREE as a variadic argument)
    int (*delete)(struct SinglyLinkedList* list, int index, ...);

    // Free the list, all of its nodes, AND ALL OF THEIR CONTENTS, by
    // default. Can also use flag value NO_AUTO_FREE to disable automatic
    // freeing of the Nodes' contents pointers.
    int (*teardown)(struct SinglyLinkedList* list, ...);



    /* ====================================================================== */
    /*                LINKEDLIST IMPLEMENTATION-SPECIFIC FIELDS               */
    /* ====================================================================== */

    #ifdef TYPE_AWARE
    // Gets pointer to the stringized type token of the data of the node
    // at a given index in the list
    char* (*get_stringized_type)(struct SinglyLinkedList*, int, char*);
    #endif

    // Stores pointer to head node - the first node in the list
    struct SLLNode* head;
};

// The standard way a LinkedList is manipulated and traversed
// is by using pointers with arrow operators.
typedef struct SinglyLinkedList* SinglyLinkedList;



struct SLLNode {
    // The number of bytes that piece of memory is pointing to
    long num_bytes;

    // A pointer to a part of memory containing data
    void* data;

    // Stores a string literal - a stringized version of the type
    // optionally passed into add_copy. Default is NULL.
    #ifdef TYPE_AWARE
    char* type;
    #endif

    struct SLLNode* next;
};

// This type definition makes it easier to declare and iterate 
// over nodes in a list manually.
typedef struct SLLNode* SLLNode;



// Create a linked list using a set of functions defined for the
// pointers in the above struct
SinglyLinkedList createSinglyLinkedList();



#endif