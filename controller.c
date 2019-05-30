#include <stdlib.h>
#include "controller.h"
#include "parameters.h"
#include "queue.c"

#define INIT_ELEVATOR_STATUS {{0}, 's', 0, EL_CAPACITY}

void update_queuing_floors(status_t *status){
	for(int i=0; i<FLOORS; i++) {
		// Check if there's no queue
		if( !is_empty( &(status->floors_queues[i])) ) {
			// If there's queue, was someone going there?
			// If true, ignore, if false, set as 'to be visited'
			if (!status->queuing_floors[i]) status->queuing_floors[i] = 1;
		}
	}
}

// Search for the nearest call
void look_for_someone(status_t *status, int num_elevator) {
	int this_floor = status->elevators[num_elevator].current_floor;
	// Look which is the nearest non empty, but to be served floor
	int i,j; i=this_floor-1; j=this_floor+1;
	int found_i = 0; int found_j = 0; //TODO CODE REPETITION. see serve customers
	while(i>=0 && j<=FLOORS && !found_i && !found_j) {
		if( status->queuing_floors[i] ) found_i=1;
		if( status->queuing_floors[j] ) found_j=1;
		--i; ++j;
	}
	// If found someone upstairs, go up
	if(found_i) {
		status->elevators[num_elevator].inertia = 'u';
		// Signal that you plan to go there
		status->queuing_floors[++i] = 0;
	}
	// If someone was found down, go down
	else if(found_j) {
		status->elevators[num_elevator].inertia = 'd';
		// Signal that you plan to go there
		status->queuing_floors[--j] = 0;
	}
	// If nobody was found, stay still
}

// Serve customers inside the elevator
void serve_current_customers(status_t *status, int num_elevator) {
	// Which is the nearest destination?
	int this_floor = status->elevators[num_elevator].current_floor;
	int i,j; i=this_floor-1; j=this_floor+1;
	int found_i = 0; int found_j = 0;
	while(i>=0 && j<=FLOORS && !found_i && !found_j) {
		if( status->elevators[num_elevator].people_destinations[i] ) found_i=1;
		if( status->elevators[num_elevator].people_destinations[j] ) found_j=1;
		--i; ++j;
	}
	//If the nearest place to go is down, go there
	if(found_i) {
		status->elevators->inertia='d';
		status->queuing_floors[++i] = 0;
	} else { // else up
		status->elevators->inertia='u';
		status->queuing_floors[--j] = 0;
	}
}

void move_up(status_t *status, int num_elevator) {
	// Does it really makes sense to go up? Or someone stole my customers?
	int is_there_anybody = 0;
	for(int i=status->elevators[num_elevator].current_floor; i<FLOORS; i++)
		if(!status->queuing_floors[i]) is_there_anybody = 1;
	// If there's anybody upstairs, try to serve customers
	if(!is_there_anybody && status->elevators[num_elevator].empty_space != EL_CAPACITY) {
		serve_current_customers(status, status->elevators[num_elevator].current_floor);
	}
}

void move_down(status_t *status, int num_elevator) {
	// Does it really makes sense to go down? Or someone stole my customers?
	int is_there_anybody = 0;
	for(int i=status->elevators[num_elevator].current_floor; i>=0; i--)
		if(!status->queuing_floors[i]) is_there_anybody = 1;
	// If there's anybody upstairs, try to serve customers
	if(!is_there_anybody && status->elevators[num_elevator].empty_space != EL_CAPACITY) {
		serve_current_customers(status, status->elevators[num_elevator].current_floor);
	}
}

// Moves the elevators
void move_elevators(status_t *status) {
	// TODO check if need to change inertia -> empty, 0, 20
	for(int i=0; i<NUM_ELEVATORS; i++) {
		char intertia = status->elevators[i].inertia;
		switch(intertia) {
			// If was going up, continue going up
			case 'u':
				move_up(status, i);
				//status->elevators[i].current_floor++;
				break;
			// Continue going down
			case 'd':
				status->elevators[i].current_floor--; //TODO there's more to do
				break;
			// Is there some job to be done?
			case 's':
				look_for_someone(&status, i);
		}
	}
}

void people_get_out(status_t *status) { //TODO deve anche aggiornare queuing floors
	//TODO
}


status_t init_status() {
	// Initialize empty status
	status_t status = {};
	// Initialize elevators
	for(int i=0; i<NUM_ELEVATORS; i++)
		status.elevators[i] = (elevator_t) INIT_ELEVATOR_STATUS;
	// Initialize queues
	for(int i=0; i<FLOORS; i++)
		status.floors_queues[i] = init_queue();
	status.queuing_floors = {0};
	
	return status;
}


void add_customer(status_t *status, int from, int to) {
	enqueue(&status->floors_queues[from], to);
}

void time_step(status_t *status) {
	update_queuing_floors(status);
	move_elevators(status);
	people_get_out(status);
	// get on
	// if nobody's there, go where called
}

