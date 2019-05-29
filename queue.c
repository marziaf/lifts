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


list_identifier_t *init_queue() {
	// Allocate space for identifier
	list_identifier_t *list_id = (list_identifier_t *) malloc(sizeof(list_identifier_t)); //TODO free
	// Head = tail at beginning
	list_id->head = new_person(-1);
	list_id->tail = list_id->head;
	return list_id;
}


void enqueue(list_identifier_t *list_id, int dest) { //TODO check id not null
	// Append a person to the list by setting tail's next element
	list_id->tail->next = new_person(dest);
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
