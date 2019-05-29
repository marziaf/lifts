#include <stdlib.h>
#include "queue.h"
#include "parameters.h"

/** Creates a new person
 * @param dest: destination of customer
 * */
person_t *new_person(int destination) { //TODO limit visibility
	// Allocate space
	person_t *new_p = (person_t *) malloc(sizeof(person_t)); //TODO check if fails
	// Initialize person
	new_p->dest = destination;
	new_p->next = NULL;

	return new_p;
}


person_t *init_queue() {
	// Dummy head is characterized by a destination value of -1
	return new_person(-1);
}


person_t *enqueue(person_t *queue_tail, int dest) {
	// Append a person to the list by setting tail's next element
	queue_tail->next = new_person(dest);
	return queue_tail->next;
}


person_t *dequeue(person_t *queue_head) {
	// Save first non-dummy element
	person_t *old_head = queue_head->next;
	// Remove link dummy head-element to dequeue
	queue_head = old_head->next;
	// Remove link element to dequeue-new first element
	old_head->next = NULL;
	// Return removed element
	return old_head;
}
