#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "elevator.h"
#include "queue.h"
#include "parameters.h"

typedef struct status {
	// Each index represents a floor, and is a pointer to a queue
	list_identifier_t floors_queues[FLOORS];
	// Floors which need to be served
	int to_serve_floors[FLOORS];
	// NUM_ELEVATORS elevators
	elevator_t elevators[NUM_ELEVATORS];
} status_t;

/** @return : status
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
