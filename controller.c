#include <stdlib.h>
#include "controller.h"
#include "parameters.h"
#include "queue.c"

#define INIT_ELEVATOR_STATUS {{0}, 's', 0, EL_CAPACITY}
#define INIT_STATUS {{NULL}, {0}, {NULL}}


// Initialize the status of the system
status_t init_status() {
	// Initialize empty status
	status_t status = (status_t) INIT_STATUS;
	// Initialize elevators
	for(int i=0; i<NUM_ELEVATORS; i++)
		status.elevators[i] = (elevator_t) INIT_ELEVATOR_STATUS;
	// Initialize queues
	for(int i=0; i<FLOORS; i++)
		status.floors_queues[i] = init_queue();
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
	//TODO poichè floors to be serve è aggiornato ogni volta, posso mettere a 0 la cella che vorrei andare a visitare e risolvere il fatto che tutti vogliano andare nello stesso posto
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
void evalutate_inertia_change(status_t *status, int lift) { //TODO this should ensure not to go -1 or 20, check if it is doing it
	elevator_t *elev = &status->elevators[lift];
	// Current inertia
	int inertia = elev->inertia;
	// Current floor
	int floor = elev->current_floor;

	// If there are not passengers, go to serve people in queue
	int nearest_to_serve_floor = -1;
	if( elev->num_people_inside == 0) {
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
void move_elevator(status_t *status, int lift) {
	elevator_t *elevator = &status->elevators[lift];
	if(elevator->inertia=='u') elevator->current_floor++;
	else if(elevator->inertia=='d') elevator->current_floor--;
}


// People who need to get down to current floor, get down
void people_get_out(status_t *status, int lift) {
	elevator_t *elevator = &status->elevators[lift];
	// Get how many people are getting down
	int people_leaving = elevator->people_destinations[elevator->current_floor];
	// Exit the elevator
	elevator->people_destinations[elevator->current_floor] = 0;
	// Update total number of people inside elevator
	elevator->num_people_inside -= people_leaving;
}


// Get the maximum number of customers
void people_get_in(status_t *status, int lift) {
	elevator_t *elev = &status->elevators[lift];
	int remaining_capacity = EL_CAPACITY - elev->num_people_inside;
	int this_floor = elev->current_floor;
	// Get the queue in this floor
	list_identifier_t *queue = &status->floors_queues[this_floor];
	// Until there's space in elevator, and people in queue, let people get inside
	for(int i=0; i<remaining_capacity && !is_empty(queue); i++) {
		// Insert person destination in elevator people tracker
		int person_destination = dequeue(queue)->dest;
		elev->people_destinations[person_destination]++;
		elev->num_people_inside++;
	}
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
		// Move elevator of one floor (if necessary)
		move_elevator(status, i);
		// People who need to exit, get out
		people_get_out(status, i);
		// People get into the elevator
		people_get_in(status, i); //TODO
	}
}

