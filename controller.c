#include <stdlib.h>
#include "controller.h"
#include "parameters.h"
#include "queue.c"

#define INIT_ELEVATOR_STATUS {{0}, 's', 0, EL_CAPACITY}



status_t init_status() {
	// Initialize empty status
	status_t status = {};
	// Initialize elevators
	for(int i=0; i<NUM_ELEVATORS; i++)
		status.elevators[i] = (elevator_t) INIT_ELEVATOR_STATUS;
	// Initialize queues
	for(int i=0; i<FLOORS; i++)
		status.floors_queues[i] = init_queue();
	status.to_serve_floors = {0}; //TODO
	
	return status;
}

// Check were people are queuing
void check_calls(status_t *status) {
	// Check in which floor there are people
	for(int i=0; i<FLOORS; i++) {
		if( !is_empty( &(status->floors_queues[i]) ) )
			status->to_serve_floors[i] = 1; // If there is someone, mark as to be visited
	}
}


// Get the nearest busy floor to serve
int get_nearest_to_serve_floor(int *floors_to_be_served, int this_floor) { //TODO così più ascensori vanno nella stessa direzione
	int found_at_d = -1;
	int found_at_u = -1;
	// Search the nearest floor whith people queuing
	// ... looking the nearest down
	for(int i=this_floor-1; (i>=0 && found_at_d!=-1); --i)
		if(floors_to_be_served[i]) found_at_d = i;
	// ... and up
	for(int i=this_floor+1; (i<FLOORS && found_at_u!=-1); ++i)
		if(floors_to_be_served[i]) found_at_u = i;
	
	// If there were no busy floors, say there's nobody
	if(found_at_u==-1 && found_at_d==-1) return -1;
	// If there's nobody upstairs, say the first one up
	if(found_at_u==-1) return found_at_d;
	// If there's nobody down, vice versa
	if(found_at_d==-1) return found_at_u;
	// If there's people up and down, return the nearest
	return (found_at_u-this_floor < this_floor-found_at_d) ? found_at_u : found_at_d;
}


// Check if it would be better to change inertia
void evalutate_inertia_change(status_t *status, int lift) { //TODO this should ensure not to go -1 or 20, check
	elevator_t *elev = &status->elevators[lift];
	// Current inertia
	int inertia = elev->inertia;
	// Current floor
	int floor = elev->current_floor;

	// If there are not passengers, go to serve people in queue
	int nearest_to_serve_floor = -1;
	if( elev->empty_space == 0) {
		nearest_to_serve_floor = get_nearest_to_serve_floor(status->to_serve_floors, elev->current_floor);
		// If nobody's queuing, stay still
		if(nearest_to_serve_floor==-1) elev->inertia = 's';
		// Otherwise get there
		elev->inertia = (nearest_to_serve_floor > floor) ? 'u' : 'd';
		return;
	}
	
	// If there's someone inside elevator
	// Give priority to passengers: if nobody needs to go were the lift is going, invert direction
	if(inertia == 'u') {
		int someone_goes_up = 0;
		// If nobody is going up, get down
		for(int i=floor; i<FLOORS; i++)
			if (elev->people_destinations[i]!=0)
				someone_goes_up=1;
		if(!someone_goes_up) elev->inertia = 'd';

	}
	// The same in opposite direction
	if(inertia == 'd') {
		int someone_goes_down=0;
		for(int i=floor; i>=0; --i)
			if (elev->people_destinations[i]!=0)
				someone_goes_down=1;
		if(!someone_goes_down) elev->inertia = 'u';
	}
	
}


// Move elevator of one floor, according to inertia
void move_elevators(status_t *status, int lift) {
	elevator_t *elevator = &status->elevators[lift];
	if(elevator->inertia=='u') elevator->current_floor++;
	else if(elevator->inertia=='d') elevator->current_floor--;
}



void add_customer(status_t *status, int from, int to) {
	enqueue(&status->floors_queues[from], to);
}

void time_step(status_t *status) {
	// Check who's arrived
	check_calls(status);
	// For each elevator
	for(int i=0; i<NUM_ELEVATORS; i++) {
		// Decide which elevator goes where
		evalutate_inertia_change(status, i);
		move_elevator(i);
	}
	//move_elevators(status);
	//people_get_out(status);
	// get on
	// if nobody's there, go where called
	//update_queuing_floors(status);
}

