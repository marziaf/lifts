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
int get_nearest_to_serve_floor(int *floors_to_be_served, int this_floor) {
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
void evalutate_inertia_change(status_t *status, int lift) {
	// If there are not passengers, go to serve people in queue
	int nearest_to_serve_floor = -1;
	if( status.elevators[lift].empty_space == 0) {
		nearest_to_serve_floor = get_nearest_to_serve_floor(&(status_t->nearest_to_serve_floor), status->elevators[lift].current_floor);
	}
	// Give priority to passengers
	
}

// Set a target destination for the lift. It won't necessary get there, but it's its target
void set_destination(status_t *status, int lift) {
	char inertia = status->elevators[lift].inertia;
	int current_floor = status->elevators[lift].current_floor;
	evalutate_inertia_change(status, lift);
	//TODO
}
/*
// Move elevator of one floor
void move_elevators(status_t status, int i) {
	char inertia = status.elevators[i].inertia;
	int current_floor = status.elevators[i].current_floor;
	// If it is empty, check if someone called
	if() //TODO
// Potrebbe servire controllare se tutti vogliono invertire la marcia
}
*/



void add_customer(status_t *status, int from, int to) {
	enqueue(&status->floors_queues[from], to);
}

void time_step(status_t *status) {
	// Check who's arrived
	check_calls(status);
	// For each elevator
	for(int i=0; i<NUM_ELEVATORS; i++) {
		// Decide which elevator goes where
		set_destination(status, i);
		move_elevator(i);
	}
	//move_elevators(status);
	//people_get_out(status);
	// get on
	// if nobody's there, go where called
	//update_queuing_floors(status);
}

