#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "elevator.h"
#include "queue.h"
#include "parameters.h"

typedef struct status {
	// Each index represents a floor, and is a pointer to a queue
	person_t* floors_queues[FLOORS];
	// Two elevators
	elevator_t elevator1;
	elevator_t elevator2;
} status_t;

/** @return : status
 * */
status_t *init_status();

/** Inserts a customer in the system
 * @param status: the system the customer has to be inserted into
 * @param from: where the customer comes from
 * @param to: where the customer is going to
 * */
void add_customer(person_t *status, int from, int to);

/** Make the elevators advance
 * */
void time_step();

/** Displays current status
 * */
void print_system_status();

#endif
