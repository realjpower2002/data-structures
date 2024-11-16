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

#ifndef DATA_STRUCTURES
#define DATA_STRUCTURES

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



// A brilliant little def provided by Mingye Wang 
//     (https://stackoverflow.com/questions/5867834/assert-with-message)
// (I have modified it somewhat to pretty print error messages)
#define assertf(x, msg, ...)({\
    if(!(x))\
        fprintf(stderr, "ASSERT : "msg __VA_OPT__(,) __VA_ARGS__);\
    assert(x);\
})



struct LinkedList {
    // Stores length of list
    int length;

    // Stores pointer to head node - the first node in the list
    struct Node* head;

    // Keyword to ignore num_bytes in add and insert (keep as
    // -1)
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
    int (*add)(struct LinkedList*, void*, int);

    // Insert a new node with contents "contents" pointing to 
    // heap memory allocation with num bytes "num_bytes", or
    // const quad value, into a given index in the list. Also
    // has optional argument - pointer to stringized type.
    int (*insert)(struct LinkedList*, int, void*, int);

    // Get a pointer to the contents of a node from its index in 
    // the list (returns NULL on failure)
    void* (*get)(struct LinkedList*, int);

    // Get a pointer to the contents of a node from its index in 
    // the list (returns pointer to default data on failure).
    void* (*get_or_default)(struct LinkedList*, int, void*);

    // Get the number of bytes pointed to by the contents pointer
    // of the Node at a given index in a given list.
    long (*get_data_size)(struct LinkedList*, int);

    // Sets index in list to be type of given string literal
    int (*set_stringized_type)(struct LinkedList*, int);

    // Gets pointer to the stringized type token of the data of the node
    // at a given index in the list
    char* (*get_stringized_type)(struct LinkedList*, int, char*);

    // Delete a node at a given index from the list.
    int (*delete)(struct LinkedList*, int, ...);

    // Defines flag variable to contraindicate automatic freeing of data
    // stored in list during teardown and delete
    #define NO_AUTO_FREE 0x29167d497c28d39d

    // Free the list, all of its nodes, AND ALL OF THEIR CONTENTS, by
    // default. Can also use flag value NO_AUTO_FREE to disable automatic
    // freeing of the Nodes' contents pointers.
    int (*teardown)(struct LinkedList*, ...);
};

// The standard way a LinkedList is manipulated and traversed
// is by using pointers with arrow operators.
typedef struct LinkedList* LinkedList;



struct Node {
    // The number of bytes that piece of memory is pointing to
    long num_bytes;

    // A pointer to a part of memory containing data
    void* contents;

    // Stores a string literal - a stringized version of the type
    // optionally passed into add_copy. Default is NULL.
    char* type;

    struct Node* next;
};

// This type definition makes it easier to declare and iterate 
// over nodes in a list manually.
typedef struct Node* Node;



// Create a linked list using a set of functions defined for the
// pointers in the above struct
LinkedList createLinkedList();


// This allows for a pointer with a given type to be obtained
// from the array and automatically returned with the correct
// type
#define get_type(list, index, type)({
    #ifdef TYPE_AWARE
    \
    assertf(strcmp(list->get_stringized_type(list,index), #type) == 0, "Type \"%s\" of Node %d in list does not match expected type \"%s\".\n", list->get_stringized_type(list,index),index,#type);\
    
    #endif\
    \
    ((type) list->get(list, index));\
})

// This allows for a pointer with a given type to be obtained
// from the array and automatically returned with the correct
// type - or a pointer to a default value to be returned with
// the correct type.
#define get_or_default_type(list, index, default, type)({
    #ifdef TYPE_AWARE
    \
    char* type_string = list->get_stringized_type(list,index);\
    assertf(type_string == NULL ^ strcmp(type_string, #type) == 0, "Type \"%s\" of Node %d in list does not match expected type \"%s\".\n", list->get_stringized_type(list,index),index,#type);\
    
    #endif\
    \
    ((type) list->get_or_default(list, index, default));\
})


// This is used for getting the number of arguments passed to 
int get_num_args(char*);



#define add_copy(list, contents, ...) ({\
    int num_va_args = get_num_args(#__VA_ARGS__);\
    \
    assertf(num_va_args <= 1, "Too many arguments (>1) passed to list macro add_copy().\n");\
    \
    int num_bytes = AUTO;\
    \
    __VA_OPT__(num_bytes = __VA_ARGS__); /*Evaluate and expand the expression here*/\
    \
    if(num_bytes == AUTO) {\
        num_bytes = sizeof(contents); /*Cast to void pointer actually makes it so just putting AUTO here won't work*/\
    }\
    \
    assertf(num_bytes != 0, "Tried to add value literal onto list using list macro add_copy().\n", num_va_args, num_bytes);\
    \
    void* copy = malloc(num_bytes);\
    memcpy(copy, contents, num_bytes);\
    \
    list->add(list,copy,num_bytes);\
})



#define add_copy_type(list, contents, type, ...) ({\
    int num_va_args = get_num_args(#__VA_ARGS__);\
    \
    assertf(num_va_args <= 1, "Too many arguments (>1) passed to list macro add_copy().\n");\
    \
    int num_bytes = AUTO;\
    \
    __VA_OPT__(num_bytes = __VA_ARGS__); /*Evaluate and expand the expression here*/\
    \
    if(num_bytes == AUTO) {\
        num_bytes = sizeof(contents); /*Cast to void pointer actually makes it so just putting AUTO here won't work*/\
    }\
    \
    assertf(num_bytes != 0, "Tried to add value literal onto list using list macro add_copy().\n", num_va_args, num_bytes);\
    \
    void* copy = malloc(num_bytes);\
    memcpy(copy, contents, num_bytes);\
    \
    list->add(list,copy,num_bytes);\
    list->set_stringized_type(list,list->length-1,#type);\
})



#define add_lit(list, literal) ({\
    list->add(list,(void*)literal,LITERAL)\
})



#define add_lit_type(list, literal, type) ({\
    list->add(list,(void*)literal,LITERAL,#type)\
    list->set_stringized_type(list,list->length-1,#type);\
})



#define insert_copy(list, index, contents, ...) ({\
    int num_va_args = get_num_args(#__VA_ARGS__);\
    \
    assertf(num_va_args <= 1, "Too many arguments (>1) passed to list macro insert_copy().\n");\
    \
    int num_bytes = AUTO;\
    \
    __VA_OPT__(num_bytes = __VA_ARGS__); /*Evaluate and expand the expression here*/\
    \
    if(num_bytes == AUTO) {\
        num_bytes = sizeof(contents); /*Cast to void pointer actually makes it so just putting AUTO here won't work*/\
    }\
    \
    void* copy = malloc(num_bytes);\
    memcpy(copy, contents, num_bytes);\
    \
    list->insert(list,index,copy,num_bytes);\
})



#define insert_copy_type(list, index, contents, type, ...) ({\
    int num_va_args = get_num_args(#__VA_ARGS__);\
    \
    assertf(num_va_args <= 1, "Too many arguments (>1) passed to list macro insert_copy().\n");\
    \
    int num_bytes = AUTO;\
    \
    __VA_OPT__(num_bytes = __VA_ARGS__); /*Evaluate and expand the expression here*/\
    \
    if(num_bytes == AUTO) {\
        num_bytes = sizeof(contents); /*Cast to void pointer actually makes it so just putting AUTO here won't work*/\
    }\
    \
    void* copy = malloc(num_bytes);\
    memcpy(copy, contents, num_bytes);\
    \
    list->insert(list,index,copy,num_bytes,#type);\
    list->set_stringized_type(list,index,#type);\
})



#define insert_lit(list, index, literal) ({\
    list->insert(list,index,(void*)literal,LITERAL);\
})



#define insert_lit_type(list, index, literal, type) ({\
    list->insert(list,index,(void*)literal,LITERAL,#type);\
    list->set_stringized_type(list,index,#type);\
})



#endif