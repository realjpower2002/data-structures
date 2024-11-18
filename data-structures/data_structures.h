// Assert function used everywhere

// A brilliant little def provided by Mingye Wang 
//     (https://stackoverflow.com/questions/5867834/assert-with-message)
// (I have modified it somewhat to pretty print error messages)
#define assertf(x, msg, ...)({\
    if(!(x))\
        fprintf(stderr, "ASSERT : "msg __VA_OPT__(, __VA_ARGS__));\
    assert(x);\
})

// Checks if a given string literal is in the ascii range - this is
// useful for detecting if a type was not supplied.
#include <string.h>
#define IS_ASCII(type_str) ({\
    int is_ascii = 1;\
    for(int i = 0; i < strlen(type_str); i++) {\
        if(type_str[i] < 0x20 || type_str[i] > 0x7E) {\
            is_ascii = 0;\
            break;\
        }\
    }\
    \
    is_ascii;\
})

// Define used to indicate optional type 
// safety
#define TYPE_AWARE


/* ========================================================================== */
/*                       INCLUDE ALL DATA STRUCTURE APIS                      */
/* ========================================================================== */

#include "./lists/lists.h"
