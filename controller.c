#include <stdlib.h>
#include "controller.h"
#include "parameters.h"
#include "queue.c"

#define INIT_ELEVATOR_STATUS {{0}, 's', 0, EL_CAPACITY}

status_t *init_status() {
	status_t *status = (status_t *) malloc(sizeof(status_t)); //TODO check if fails //TODO free

	// Initialize elevators
	status->elevator1 = (elevator_t) INIT_ELEVATOR_STATUS;
	status->elevator2 = (elevator_t) INIT_ELEVATOR_STATUS;

	// Initialize queues
	for(int i=0; i<FLOORS; i++)
		status->floors_queues[i] = init_queue(); //TODO free
	
	return status;
}

