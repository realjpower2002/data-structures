#ifndef DATA_STRUCTURES
#define DATA_STRUCTURES

struct LinkedList {
    // stores length of list
    int length;

    // stores pointer to next (first) node in list
    struct Node* head;

    // add a new node with contents "contents" to the
    // end of the list
    int (*add)(struct LinkedList*, void* contents);

    // insert a value into a list by its index
    int (*insert)(struct LinkedList*, int, void* contents);

    // standard function to get a node's contents from its index (returns NULL on failure)
    void* (*get)(struct LinkedList*, int);

    // get or default (function to return the value at an index or a default one if it doesn't exist)
    void* (*get_or_default)(struct LinkedList*, int, void*);

    // function to delete a node using its index
    int (*delete)(struct LinkedList*, int);

    // function to teardown the list
    int (*teardown)(struct LinkedList*);
};

struct Node {
    void* contents;

    struct Node* next;
};

struct LinkedList* createLinkedList();

#endif