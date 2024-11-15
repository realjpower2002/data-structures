#include "data_structures.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void test_create_empty_list() {
    printf("Running test_create_empty_list...");

    LinkedList list = createLinkedList();
    assertf(list != NULL, "Failed to create an empty list.");
    assertf(list->length == 0, "List length should be 0 on creation.");
    printf("passed.\n");

    list->teardown(list);
}

void test_add_to_empty_list() {
    printf("Running test_add_to_empty_list...");

    LinkedList list = createLinkedList();
    
    int data = 42;
    add_copy(list, &data, 4);

    // printf("Int at end of list %d\n", *(get_type(list, list->length-1, int*)));

    assertf(list->length == 1, "List length should be 1 after adding an element.");
    assertf(*(int*)list->head->contents == 42, "Added data is not correct.");
    
    printf("passed.\n");

    list->teardown(list);
}

void test_insert_at_specific_index() {
    printf("Running test_insert_at_specific_index...");

    LinkedList list = createLinkedList();
    
    int data1 = 10;
    list->add(list, &data1);

    int data2 = 20;
    list->insert(list, 1, &data2); // Insert at index 1

    assertf(list->length == 2, "List length should be 2 after insertion.");
    assertf(*(int*)list->get(list, 0) == 10, "First element is incorrect.");
    assertf(*(int*)list->get(list, 1) == 20, "Second element is incorrect.");

    printf("passed.\n");
}

void test_insert_at_head() {
    printf("Running test_insert_at_head...");

    LinkedList list = createLinkedList();

    int data1 = 10;
    list->add(list, &data1);  // Adding the first element
    
    int data2 = 5;
    list->insert(list, 0, &data2);  // Insert at index 0 (head)

    assertf(list->length == 2, "List length should be 2 after inserting at head.");
    assertf(*get_type(list, 0, int*) == 5, "First element should be 5.");
    assertf(*get_type(list, 1, int*) == 10, "Second element should be 10.");

    printf("passed.\n");
}

void test_insert_at_end() {
    printf("Running test_insert_at_end...");

    LinkedList list = createLinkedList();

    int data1 = 10;
    list->add(list, &data1);  // Add first element
    
    int data2 = 20;
    list->insert(list, 1, &data2);  // Insert at index 1 (end of the list)

    assertf(list->length == 2, "List length should be 2 after insertion.");
    assertf(*get_type(list, 0, int*) == 10, "First element should be 10.");
    assertf(*get_type(list, 1, int*) == 20, "Second element should be 20.");

    printf("passed.\n");
}

void test_invalid_index_access() {
    printf("Running test_invalid_index_access...");

    LinkedList list = createLinkedList();

    int data = 10;
    list->add(list, &data);

    assertf(list->get_or_default(list, 1, NULL) == NULL, "Should return default value for index out of bounds.");

    printf("passed.\n");
}

void test_delete_by_index() {
    printf("Running test_delete_by_index...");

    LinkedList list = createLinkedList();
    LinkedList list2 = createLinkedList();

    int data1[] = {10};

    // add_copy(list2, data1);

    // add_copy(list, list2);
    add_copy(list, data1);

    int data2 = 20;
    add_copy(list, &data2, 4);

    list->delete(list, 0);  // Delete element at index 0

    assertf(list->length == 1, "List length should be 1 after deletion.");
    assertf(*get_type(list, 0, int*) == 20, "First element should be 20 after deletion.");

    printf("passed.\n");
}

void test_delete_last_element() {
    printf("Running test_delete_last_element...");

    LinkedList list = createLinkedList();

    int data1 = 10;
    add_copy(list, &data1, 4);
    
    int data2 = 20;
    add_copy(list, &data2, 4);

    list->delete(list, 1);  // Delete last element

    assertf(list->length == 1, "List length should be 1 after deleting last element.");
    assertf(*get_type(list, 0, int*) == 10, "Last remaining element should be 10.");

    printf("passed.\n");
}

void test_teardown() {
    printf("Running test_teardown...");

    LinkedList list = createLinkedList();

    int data = 10;
    add_copy(list, &data, 4);

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