#include <stdlib.h>
#include "controller.h"
#include "parameters.h"
#include "queue.c"

#define INIT_ELEVATOR_STATUS {{0}, 's', 0, EL_CAPACITY}

status_t init_status() {
	// Initialize elevators
	elevator_t elevator1 = (elevator_t) INIT_ELEVATOR_STATUS;
	elevator_t elevator2 = (elevator_t) INIT_ELEVATOR_STATUS;
	
	status_t status = (status_t) {{}, elevator1, elevator2};
	// Initialize queues
	for(int i=0; i<FLOORS; i++)
		status.floors_queues[i] = init_queue();
	
	return status;
}


void add_customer(status_t *status, int from, int to) {
	enqueue(&status->floors_queues[from], to);
}

void time_step() {
	
}

