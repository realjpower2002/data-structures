#include "data_structures.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// Brilliant little def provided by Mingye Wang (https://stackoverflow.com/questions/5867834/assert-with-message)
#define assertmsg(x, msg) assert(((void) msg, x))

void test_create_empty_list() {
    printf("Running test_create_empty_list...");

    struct LinkedList* list = createLinkedList();
    assertmsg(list != NULL, "Failed to create an empty list.");
    assertmsg(list->length == 0, "List length should be 0 on creation.");
    printf("passed.\n");

    list->teardown(list);
}

void test_add_to_empty_list() {
    printf("Running test_add_to_empty_list...");

    struct LinkedList* list = createLinkedList();
    
    int data = 42;
    list->add(list, &data);

    assertmsg(list->length == 1, "List length should be 1 after adding an element.");
    assertmsg(*(int*)list->head->contents == 42, "Added data is not correct.");
    
    printf("passed.\n");

    list->teardown(list);
}

void test_insert_at_specific_index() {
    printf("Running test_insert_at_specific_index...");

    struct LinkedList* list = createLinkedList();
    
    int data1 = 10;
    list->add(list, &data1);

    int data2 = 20;
    list->insert(list, 1, &data2); // Insert at index 1

    assertmsg(list->length == 2, "List length should be 2 after insertion.");
    assertmsg(*(int*)list->get(list, 0) == 10, "First element is incorrect.");
    assertmsg(*(int*)list->get(list, 1) == 20, "Second element is incorrect.");

    printf("passed.\n");
}

void test_insert_at_head() {
    printf("Running test_insert_at_head...");

    struct LinkedList* list = createLinkedList();

    int data1 = 10;
    list->add(list, &data1);  // Adding the first element
    
    int data2 = 5;
    list->insert(list, 0, &data2);  // Insert at index 0 (head)

    assertmsg(list->length == 2, "List length should be 2 after inserting at head.");
    assertmsg(*(int*)list->get(list, 0) == 5, "First element should be 5.");
    assertmsg(*(int*)list->get(list, 1) == 10, "Second element should be 10.");

    printf("passed.\n");
}

void test_insert_at_end() {
    printf("Running test_insert_at_end...");

    struct LinkedList* list = createLinkedList();

    int data1 = 10;
    list->add(list, &data1);  // Add first element
    
    int data2 = 20;
    list->insert(list, 1, &data2);  // Insert at index 1 (end of the list)

    assertmsg(list->length == 2, "List length should be 2 after insertion.");
    assertmsg(*(int*)list->get(list, 0) == 10, "First element should be 10.");
    assertmsg(*(int*)list->get(list, 1) == 20, "Second element should be 20.");

    printf("passed.\n");
}

void test_invalid_index_access() {
    printf("Running test_invalid_index_access...");

    struct LinkedList* list = createLinkedList();

    int data = 10;
    list->add(list, &data);

    assertmsg(list->get(list, 1) == NULL, "Should return NULL for index out of bounds.");
    assertmsg(list->get_or_default(list, 1, NULL) == NULL, "Should return default value for index out of bounds.");

    printf("passed.\n");
}

void test_delete_by_index() {
    printf("Running test_delete_by_index...");

    struct LinkedList* list = createLinkedList();

    int data1 = 10;
    list->add(list, &data1);

    int data2 = 20;
    list->add(list, &data2);

    list->delete(list, 0);  // Delete element at index 0

    assertmsg(list->length == 1, "List length should be 1 after deletion.");
    assertmsg(*(int*)list->get(list, 0) == 20, "First element should be 20 after deletion.");

    printf("passed.\n");
}

void test_delete_last_element() {
    printf("Running test_delete_last_element...");

    struct LinkedList* list = createLinkedList();

    int data1 = 10;
    list->add(list, &data1);
    
    int data2 = 20;
    list->add(list, &data2);

    list->delete(list, 1);  // Delete last element

    assertmsg(list->length == 1, "List length should be 1 after deleting last element.");
    assertmsg(*(int*)list->get(list, 0) == 10, "Last remaining element should be 10.");

    printf("passed.\n");
}

void test_teardown() {
    printf("Running test_teardown...");

    struct LinkedList* list = createLinkedList();

    int data = 10;
    list->add(list, &data);

    list->teardown(list);  // Teardown and free memory

    // If teardown is correct, the list should be null, and memory should be freed.

    printf("passed.\n");
}

int main(int argc, char** argv) {
    test_create_empty_list();
    test_add_to_empty_list();
    test_insert_at_specific_index();
    test_insert_at_head();
    test_insert_at_end();
    test_invalid_index_access();
    test_delete_by_index();
    test_delete_last_element();
    test_teardown();
    
    return 0;
}