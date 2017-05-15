#include <linkedlist.h>

void linkedlist_init(struct linkedlist* head){
	//not currently important
	head->ptr = NULL;
	head->next = NULL;
}

struct linkedlist* linkedlist_new(){
	struct linkedlist* list = malloc(sizeof(struct linkedlist));
	linkedlist_init(list); //initialize list properly
	return list;
}

void linkedlist_set_next(struct linkedlist* list, void* next_ptr){
	list->next = linkedlist_new();
	list->next->ptr = next_ptr;
}

struct linkedlist* linkedlist_get_next(struct linkedlist* list){
	return list->next;
}

void linkedlist_append(struct linkedlist* head, void* new_data){
	if(head->next == NULL){ //if this is the end of the list
		linkedlist_set_next(head, new_data); //Put the data in after the end
	}else{
		linkedlist_append(head->next, new_data);
	}
}

void linkedlist_free(struct linkedlist* head, void (*free_function)(void*)){
	if(head->next != NULL) //recurse to the end of the list freeing the memory (will break if list is circular)
		linkedlist_free(head->next, free_function);
	/*
	 * Actually free the memory now that we're at the end of what's left in the list
	 */
	if(free_function != NULL){
		(*free_function)(head->ptr); //call custom freeing function
	}else{
		free(head->ptr);
	}
	free(head);
}

//call with:
//	(*free_function)(head->ptr);
