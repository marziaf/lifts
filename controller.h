#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "elevator.h"
#include "queue.h"
#include "parameters.h"

/* Here are defined the functions responsible for the management
 * of the system
 * */


/** Struct status
 * Contains the context of the system, that is:
 * - The queues at each floor
 * - A boolean vector to keep track of floors that need to be
 *   served by an elevator
 * - An array containing the elevators
 * */
typedef struct status {
	// Each index represents a floor, and is a pointer to a queue
	list_identifier_t floors_queues[FLOORS];

	// Floors which need to be served
	char to_serve_floors[FLOORS];

	// Elevators
	elevator_t elevators[NUM_ELEVATORS];

} status_t;


/** Returns a new status initialized to default
 * @return : status
 * */
status_t init_status();


/** Inserts a customer in the system
 * @param status: the system the customer has to be inserted into
 * @param from: where the customer comes from
 * @param to: where the customer is going to
 * */
void add_customer(status_t *status, int from, int to);


/** Make the elevators advance
 * */
void time_step(status_t *status);


/** Displays current status
 * */
void print_system_status(status_t *status);

#endif
