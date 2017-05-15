#include <stdlib.h>
#include <stdio.h> //temporary

#ifndef _linkedlist_h_
#define _linkedlist_h_

struct linkedlist{
	void* ptr;
	struct linkedlist* next;
};

typedef struct linkedlist LinkedList;

/**
 * Initialize the given linkedlist
 *
 * @param head: the linkedlist to initialize
 */
extern void linkedlist_init(struct linkedlist* head);

/**
 * Create a new linkedlist
 */
extern struct linkedlist* linkedlist_new();

/**
 * Set the pointer for the next piece of data (appropriate structures will be created)
 *
 * @param list: LinkedList to add after
 * @param next_ptr: pointer to data to set as the next piece of data
 */
extern void linkedlist_set_next(struct linkedlist* list, void* next_ptr);

/**
 * Get the next list object
 *
 * @param list: LinkedList whose next pointer to get
 */
extern struct linkedlist* linkedlist_get_next(struct linkedlist* list);

/**
 * Append data onto the end of the list
 *
 * @param head: head of the linkedlist to append onto
 * @param new_data: the data to add to the end
 */
extern void linkedlist_append(struct linkedlist* head, void* new_data);

/**
 * Free the linkedlist, optionally providing the function used to free the data structures being contained.
 *
 * Note: If no free_function is provided (it is NULL), then the stdlib's free() function will be used
 *
 * @param head: The linkedlist to free from memory
 * @param free_function: the function to use to free the data held inside the linkedlist. Must have the following declaration:
 * 			void my_free_function(struct my_custom_type* data)
 */
extern void linkedlist_free(struct linkedlist* head, void (*free_function)(void*)); 

//call with:
//	(*free_function)(head->ptr);

#endif
