#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "parameters.h"

// Returns a new person
 person_t *new_person(int destination) {
	// Allocate space
	person_t *new_p = (person_t *) malloc(sizeof(person_t));
	// Check if allocation was successfull
	if( !new_p ) {
		fprintf(stderr, "ERROR in queue: Memory allocation failed in creating a new person\n");
		return NULL;
	}

	// Initialize person with given destination
	new_p->dest = destination;
	// and NULL pointer to next person
	new_p->next = NULL;

	return new_p;
}


// Verify the existence of the id to prevent errors
int check_queue_existence(list_identifier_t *list_id) {
	// If list_id is not initialized, error
	if(!list_id) {
		fprintf(stderr, "ERROR in queue: list identifier wasn't initialized\n");
		return 1;
	}
	return 0;
}


list_identifier_t init_queue() {
	// Head = tail at beginning
	// Dummy head -> destination value is not a valid one
	person_t *head = new_person(-1);
	return (list_identifier_t) {head, head};
}


void enqueue(list_identifier_t *list_id, int dest) {
	// If list_id is not initialized, don't do anything, but signal
	if(check_queue_existence(list_id)) return;

	// Append a person to the list by setting tail's next element
	list_id->tail->next = new_person(dest);
	// Set last element as tail
	list_id->tail = list_id->tail->next;
}


person_t *dequeue(list_identifier_t *list_id) {
	// If list_id is not initialized, return NULL
	if(check_queue_existence(list_id)) return NULL;

	// Check if queue is empty
	if(is_empty(list_id)) return NULL;

	// Save first non-dummy element (element to be deleted)
	person_t *old = list_id->head->next;
	// If it was the tail (so there was just one element), update tail
	if(old == list_id->tail) list_id->tail = list_id->head;
	// Remove link between dummy head and the element to dequeue
	list_id->head->next = old->next;
	// Remove link between element to dequeue and new first element
	old->next = NULL;
	// Return removed element
	return old;
}


int is_empty(list_identifier_t *id) {
	// If head and tail coincide, the list is empty
	return id->head == id->tail ? 1 : 0;
}
