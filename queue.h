#ifndef QUEUE_H
#define QUEUE_H

#include "parameters.h"

/** Person is a node of the list
 */
typedef struct person{
	int dest;
	struct person *next;
} person_t;

/** Allows to identify a list by its head and tail
 * */
typedef struct list_identifier{
	person_t *head;
	person_t *tail;
} list_identifier_t;


/** Returns an empty list with a dummy head=tail
 * @return : list_identifier
 * */
list_identifier_t init_queue();

/** Adds a person in queue
 * @param list_id: identifier of list
 * @param dest: destination floor of customer
 * */
void enqueue(list_identifier_t *list_id, int dest);

/** Removes a customer from the queue
 * @param list_id: identifier of list
 * @return : the person deleted, or NULL if queue was empty
 * */
person_t *dequeue(list_identifier_t *list_id);

/** @ param id : the list identifier
 * @ return : 1 if empty, 0 otherwise
 * */
int is_empty(list_identifier_t *id);


#endif //QUEUE_H
