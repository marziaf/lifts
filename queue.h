#ifndef QUEUE_H
#define QUEUE_H

#include "parameters.h"

/** Person is a node of the list
 */
typedef struct person{
	int dest;
	struct person *next;
} person_t;

/** Returns an empty list with a dummy head=tail
 * @return : head
 * */
person_t *init_queue();

/** Adds a person in queue
 * @param queue_tail: last element of queue
 * @param dest: destination floor of customer
 * @return : new tail
 * */
person_t *enqueue(person_t *queue_tail, int dest);

/** Removes a customer from the queue
 * @param queue_head: first element of the queue
 * @return : the customer removed
 * */
person_t *dequeue(person_t *queue_head);




#endif //QUEUE_H
