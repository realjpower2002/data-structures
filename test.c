#include "data_structures.h"
#include <stdio.h>

int main(int argc, char** argv) {
    printf("Testing data structures.\n");

    struct* LinkedList list = createLinkedList();

    int* my_integer = 0xdeadbeef;

    list->insert(list,1,my_integer);

    printf("My integer %x\n", list->get(list,0));
}