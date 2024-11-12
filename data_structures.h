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

// A brilliant little def provided by Mingye Wang 
//     (https://stackoverflow.com/questions/5867834/assert-with-message)
#define assertmsg(x, msg) assert(((void) msg, x))

struct LinkedList {
    // Stores length of list
    int length;

    // Stores pointer to head node - the first node in the list
    struct Node* head;

    // Add a new node with contents "contents" to the end of the 
    // list
    int (*add)(struct LinkedList*, void* contents);

    // Insert a new node with contents "contents" into a given
    // index in the list
    int (*insert)(struct LinkedList*, int, void* contents);

    // Get a pointer to the contents of a node from its index in 
    // the list (returns NULL on failure)
    void* (*get)(struct LinkedList*, int);

    // Get a pointer to the contents of a node from its index in 
    // the list (returns pointer to default data on failure).
    void* (*get_or_default)(struct LinkedList*, int, void*);

    // Delete a node at a given index from the list.
    int (*delete)(struct LinkedList*, int);

    // Free the list, all of its nodes, AND ALL OF THEIR CONTENTS.
    int (*teardown)(struct LinkedList*);
};

// The standard way a LinkedList is manipulated and traversed
// is by using pointers with arrow operators.
typedef struct LinkedList* LinkedList;

struct Node {
    void* contents;

    struct Node* next;
};

// This type definition makes it easier to declare and iterate 
// over nodes in a list manually.
typedef struct Node* Node;



// ... What kind of weirdo wouldn't use GNUC ? 
#ifdef __GNUC__

// This allows for a value to be copied to a list from the stack
// (or anywhere in memory) in one line, by doing type inference 
// and heap allocation at runtime.
#define add_auto(list, pointer_to_data) ({\
    typeof(pointer_to_data)* pointer_to_copy = malloc(sizeof(pointer_to_data));\
    memcpy(pointer_to_copy, pointer_to_data, sizeof(pointer_to_data));\
    \
    list->add(list,pointer_to_copy);\
})

// This allows for a value to be copied to a list from the stack
// (or anywhere in memory) in one line, by doing type inference
// and heap allocation at runtime.
#define insert_auto(list, index, pointer_to_data) ({\
    typeof(pointer_to_data)* pointer_to_copy = malloc(sizeof(pointer_to_data));\
    memcpy(pointer_to_copy, pointer_to_data, sizeof(pointer_to_data));\
    \
    list->insert(list,index,pointer_to_copy);\
})

#endif



// This allows for a pointer with a given type to be obtained
// from the array and automatically returned with the correct
// type
#define get_type(list, index, type)({\
    ((type) list->get(list, index));\
})

// This allows for a pointer with a given type to be obtained
// from the array and automatically returned with the correct
// type - or a pointer to a default value to be returned with
// the correct type.
#define get_or_default_type(list, index, default, type)({\
    ((type) list->get_or_default(list, index, default));\
})

LinkedList createLinkedList();

#endif