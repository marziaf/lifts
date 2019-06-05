#include <stdlib.h>
#include "queue.h"
#include "parameters.h"

/** Creates a new person
 * @param dest: destination of customer
 * */
person_t *new_person(int destination) { //TODO limit visibility
	// Allocate space
	person_t *new_p = (person_t *) malloc(sizeof(person_t)); //TODO free
	// Initialize person
	new_p->dest = destination;
	new_p->next = NULL;
	return new_p;
}


list_identifier_t init_queue() {
	// Head = tail at beginning
	person_t *head = new_person(-1);
	return (list_identifier_t) {head, head};
}


void enqueue(list_identifier_t *list_id, int dest) { //TODO check id not null
	// Append a person to the list by setting tail's next element
	list_id->tail->next = new_person(dest);
	// Set last element as tail
	list_id->tail = list_id->tail->next;
}


person_t *dequeue(list_identifier_t *list_id) { //TODO check if head=tail
	// Check if queue is empty
	if(list_id->head == list_id->tail) return NULL;
	// Save first non-dummy element
	person_t *old = list_id->head->next;
	// Remove link dummy head-element to dequeue
	list_id->head->next = old->next;
	// Remove link element to dequeue-new first element
	old->next = NULL;
	// Return removed element
	return old; //TODO poi fare free
}


int is_empty(list_identifier_t *id) {
	return id->head == id->tail ? 1 : 0;
}
