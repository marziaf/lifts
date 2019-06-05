#include <stdlib.h>
#include <stdio.h>
#include "controller.h"
#include "parameters.h"
#include "queue.c"

#define INIT_ELEVATOR_STATUS {{0}, 's', 0, EL_CAPACITY}
#define INIT_STATUS {{}, {0}, {}} //TODO check if ok, otherwise use memset


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
int get_nearest_to_serve_floor(int *floors_to_be_served, int this_floor) {
	// Get the nearest floor to be served going up or down
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
	int nearest = -1;
	// If there's nobody upstairs, nearest is down
	if(found_at_u==-1) nearest = found_at_d;
	// If there's nobody down, nearest is up
	if(found_at_d==-1) nearest = found_at_u;
	// If there's people up and down, get nearest by comparison
	nearest = (found_at_u-this_floor < this_floor-found_at_d) ? found_at_u : found_at_d;

	// Signal intention to go to nearest floor, to avoid that multiple lifts are going there
	floors_to_be_served[nearest] = 0;
	return nearest;
}


// Check if it would be better to change inertia
void evalutate_inertia_change(status_t *status, int lift) {
	elevator_t *elev = &status->elevators[lift];
	// Current inertia
	int inertia = elev->inertia;
	// Current floor
	int floor = elev->current_floor;

	// Check if elevator reached top or bottom
	if(floor == 0) elev->inertia = 'u';
	else if(floor == FLOORS-1) elev->inertia = 'd';

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


void print_queue(list_identifier_t *list_id) {
	person_t *iterator = list_id->head->next;
	while(iterator != list_id->tail) {
		printf("{%2d} ", iterator->dest);
		iterator = iterator->next;
	}
}

void print_elevators(elevator_t *elevators, int floor) {
	for(int i=0; i<NUM_ELEVATORS; i++) {
		if(elevators[i].current_floor == floor) // If elevator i is at this floor
			printf("[%2d] ", elevators[i].num_people_inside);
		else // Else leave blank space
			printf("     ");
	}
}

void print_floors(status_t *status) {
	// Display people queuing at each floor and the lifts
	// 19    | [ 1]     [12] | { 1} {19} { 3} <- floor example
	int first_col_size = 6; //xx____
	int elevator_space = NUM_ELEVATORS*5;// [xx]_
	
	// Titles of table
	printf("%.*s|%.*s|%s\n", first_col_size, "FLOOR", elevator_space, "ELEVATORS", "QUEUE");
	// Print a line for each floor
	for(int f=FLOORS-1; f>=0; --f) {
		printf("%.*d|",first_col_size, f); // Number of floor
		print_elevators(status->elevators, f);
		printf("| ");
		print_queue(&status->floors_queues[f]);
		printf("\n");
	}
}

void print_inside_elevators(status_t *status) {
	// Print also the destinations of people inside elevators
	int padding = 8;
	printf("LIFT->");
	for(int i=0; i<NUM_ELEVATORS; ++i) {
		printf("LIFT #%2d",i);
	}
	printf("FLOOR ");
	for(int f=0; f<FLOORS; ++f) {
		for(int e=0; e<NUM_ELEVATORS; ++e) {
			int ppl_in_e_going_f = status->elevators[e].people_destinations[f];
			if(ppl_in_e_going_f) printf("%-.*d", padding, ppl_in_e_going_f);
			else printf("%.*s", padding, "    /    ");
		}
	}
}


void print_system_status(status_t *status) {
	printf("_____________________________");
	printf("QUEUES STATUS\n\n");
	print_floors(status);
	printf("ELEVATORS STATUS\n\n");
	print_inside_elevators(status);
}
