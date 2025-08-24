// The different lists have different implementations, but all of them have
// the same standard API, with functions in the same order, albeit potentially
// with different arguments. All function pointers are 8 bytes.

// Standard List Fields : 
//     int length
//     int append()
//     int insert()
//     void* get()
//     long get_data_size()
//     int delete()
//     int teardown()

// All fields in list structs defined after these are specific to the
// implementations of those lists.



#ifndef LISTS
#define LISTS
/* ========================================================================== */
/*               INCLUDE ALL LIST API IMPLEMENTATIONS AND MACROS              */
/* ========================================================================== */


#include "./src/SinglyLinkedList.c"



/* ========================================================================== */
/*                STANDARD DEFINES - GENERIC FOR EACH LIST TYPE               */
/* ========================================================================== */

// This allows for a pointer with a given type to be obtained
// from the array and automatically returned with the correct
// type
//
// If the list is type-aware, then this will also fire an
// assertion

// assertf(strcmp(list->get_stringized_type(list,index), #type) == 0, "Type \"%s\" of Node %d in list does not match expected type \"%s\".\n", list->get_stringized_type(list,index),index,#type);
#define get_type(list, index, type)({\
    ((type) list->get(list, index, #type));\
})

// This allows for a pointer with a given type to be obtained
// from the array and automatically returned with the correct
// type - or a pointer to a default value to be returned with
// the correct type.
//
// If the list is type-aware, then this will also fire an
// assertion

//char* type_string = list->get_stringized_type(list,index);
//assertf(type_string == NULL ^ strcmp(type_string, #type) == 0, "Type \"%s\" of Node %d in list does not match expected type \"%s\".\n", list->get_stringized_type(list,index),index,#type);
    
#define get_or_default_type(list, index, default, type)({\
    ((type) list->get_or_default(list, index, default, #type));\
})



// Optional parameter here is the type, which is ignored
// unless the internal list append happens to be type-aware
#define append_copy(list, contents, num_bytes, ...) ({\
    assertf(num_bytes != IGNORE, "Tried to add data copy onto list with IGNORE option for num_bytes using macro add_copy().\n");\
    assertf(num_bytes != LITERAL, "Tried to add value literal onto list using list macro add_copy().\n");\
    \
    int bytes_to_allocate = (num_bytes == AUTO) ? sizeof(contents) : num_bytes;\
    \
    void* copy = malloc(bytes_to_allocate);\
    \
    memcpy(copy, contents, bytes_to_allocate);\
    \
    list->append(list, copy, bytes_to_allocate __VA_OPT__(, #__VA_ARGS__));\
})



// Optional parameter here is the type, which is ignored
// unless the internal list insert happens to be type-aware
#define insert_copy(list, index, contents, num_bytes, ...) ({\
    assertf(num_bytes != IGNORE, "Tried to insert data copy into list with IGNORE option for num_bytes using macro insert_copy().\n");\
    assertf(num_bytes != LITERAL, "Tried to insert value literal into list using list macro insert_copy().\n");\
    \
    int bytes_to_allocate = (num_bytes == AUTO) ? sizeof(contents) : num_bytes;\
    \
    void* copy = malloc(bytes_to_allocate);\
    \
    memcpy(copy, contents, bytes_to_allocate);\
    \
    list->insert(list, index,copy, bytes_to_allocate __VA_OPT__(, #__VA_ARGS__));\
})



#endif