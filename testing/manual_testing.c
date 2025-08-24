#include "../data-structures/data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int write_beloved_string(SinglyLinkedList list) {
    char str[] = "My Beloved String";
    list->append(list, str, AUTO, "char*");

    printf("Beloved  : %p\n", str);

    printf("I am so thankful I can store \"%s\" in a list ...\n",list->get(list, list->length-1, "char*"));

    return 1;
}

int main(int argc, char** argv) {
    printf("Testing data structures.\n");

    // Woooo pointer typedef works !
    SinglyLinkedList list = createSinglyLinkedList();

    printf("Created a list.\n");

    unsigned int* my_integer = malloc(sizeof(unsigned int));

    if(my_integer == NULL) {
        printf("Couldn't allocate a single unsigned int ... what in the world \
        is wrong with your heap?\n");
    }



    // Value of new integer
    *my_integer = 0xdeadbeef;

    // Inserting new integer into list at position 0 with internal function
    list->insert(list,0,my_integer,AUTO,"unsigned int*");



    void* my_pointer = list->get(list,0,"unsigned int*");

    // Check for if pointer we just inserted is within the bounds of the list
    // (it is)
    if(my_pointer == NULL) {
        printf("Tried to get a pointer outside of bounds...\n");
        return 1;
    }

    // Displaying integer we just inserted after retrieving it again
    printf("Added integer 0x%x\n", *(unsigned int*)list->get(list,0,"unsigned int*"));



    // Declaring a new integer
    unsigned int* new_int = malloc(sizeof(unsigned int));

    if(new_int == NULL) {
        printf("Couldn't allocate a single unsigned int ... what in the world \
        is wrong with your heap?\n");
    }



    // New int contents
    *new_int = 0xfeedface;



    // Adding int to list
    list->append(list,new_int,AUTO,"unsigned int*");



    // Testing a new type of list retrieval (testing if the null return
    // works)
    if ((my_pointer = list->get_or_default(list,2,NULL,"unsigned int*")) != NULL)
        printf("Another integer is 0x%x.\n",*(unsigned int*)my_pointer);



    // Or we could do it the easy (and dirty) way
    unsigned int* uint_pointer = get_or_default_type(list,1,NULL,unsigned int*);

    printf("Got here.\n");

    if (uint_pointer != NULL)
        printf("Another integer is 0x%x.\n",*uint_pointer);

    printf("Deleting node @ index 2.\n");



    // Testing if delete works
    list->delete(list,2);



    SLLNode cur = list->head;

    printf("Currently %d nodes remain.\n",list->length);


    // Printing contents of list through manual traversal
    for(int i = 0; i < list->length; i++) {
        printf("%p\n",cur->data);
        cur = cur->next;
    }



    // Demonstrate how stack data vanishes once it leaves scope
    //
    // We write a wonderful string at index 2
    write_beloved_string(list);
    
    // Okay, I think gcc has some stack protection to prevent this
    // from trivially smashing the stack, but the point is that
    // our beloved string was ruined
    //
    // Also, at the end, putting stack data directly into the list
    // will cause a corruption when we try to free a memory address
    // that is on the stack
    char despicable_string[] = "I hate this string!!!!!!!!!!";

    printf("Despised : %p\n", despicable_string);
    printf("%s ... NOOOOOOO!!!\n", list->get(list, list->length-1, "char*"));



    // Understandably tired after a long day of complicated pointer stuff
    printf("Man, that was pretty complicated ... I'm tired ...\n");

    // Use a macro instead to just get the desired type from the list
    // at the given position
    printf("0x%x\n", *get_type(list, 1, unsigned int*));
    printf("So much easier !\n");



    #define TYPE_AWARE

    // Declare a sensible stack string ...
    char string[] = "Goobergenius";
    // Add it using automatic size determination with sizeof()
    append_copy(list, string, AUTO, char*);
    // Get the contents of the list @2 as a char array
    printf("%s ... oh wow (takes picture).\n", get_type(list,list->length-1,char*));



    // Declare a mysterious stack string ...
    char* string_mystery = "Goobergenius ... SIZEOF HAS NO POWER HERE MUAHAHAHA";
    // Add it using manually supplied memory length
    append_copy(list, string_mystery, strlen(string_mystery), char*);
    printf("%s ... but carefully manually managed memory does 😎\n", get_type(list, list->length-1, char*));



    list->append(list, 420, LITERAL, "int");
    printf("%d ... the types can be whatever i want ...\n", list->get(list, list->length-1, "int"));



    // Get the number of bytes pointed to by string_mystery
    printf("Number of bytes in mystery : %d\n", sizeof(get_type(list, list->length-2, char*)));
    printf("Number of bytes in my log for mystery : %d\n", list->get_data_size(list, list->length-2));

    // Unfortunately, the size information is lost after the cast to void pointer :(

    

    // destroy the list
    list->teardown(list, NO_AUTO_FREE);
    //list->teardown(list);

    return 0;
}