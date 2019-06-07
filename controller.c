#include <stdlib.h>
#include <stdio.h>
#include "controller.h"
#include "parameters.h"
#include "queue.h"

#define INIT_ELEVATOR_STATUS {{0}, 's', 0, 0}
#define INIT_STATUS { NULL , {0}, NULL} //TODO check if ok, otherwise use memset
#define SPACER printf("________________________\n");


//_____________________________________________________________________________________________
// INITIALIZATION
//_____________________________________________________________________________________________

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


//_____________________________________________________________________________________________
// ADD CUSTOMER
//_____________________________________________________________________________________________


void add_customer(status_t *status, int from, int to) {
	enqueue(&status->floors_queues[from], to);
}


//_____________________________________________________________________________________________
// MOVEMENTS
//_____________________________________________________________________________________________

// Check were people are queuing
void check_calls(status_t *status) {
	// Check in which floors there are people
	for(int i=0; i<FLOORS; i++) {
		if( !is_empty( &(status->floors_queues[i]) ) )
			status->to_serve_floors[i] = 1; // If there is someone, mark as to be visited
	}
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
	int this_floor = elev->current_floor;
	// Calculate how many people can fit
	int remaining_capacity = EL_CAPACITY - elev->num_people_inside;
	// Get the queue in this floor
	list_identifier_t *queue = &status->floors_queues[this_floor];
	// Until there's space in elevator, and people in queue, let people get inside
	int person_destination;
	for(int i=0; i<remaining_capacity && !is_empty(queue); i++) {
		// Insert person destination in elevator people tracker
		person_t *to_insert = dequeue(queue);  // Get person and it's destination
		person_destination = to_insert->dest;
		free(to_insert);
		// Update elevator params
		elev->people_destinations[person_destination]++;
		elev->num_people_inside++;
	}
}


// Get the nearest busy floor to serve
int get_nearest_to_serve_floor(int *floors_to_be_served, int this_floor, char inertia) {
	// Get the nearest floor to be served going up or down
	int found_at_d = -1;  // Nearest lower floor with people
	int found_at_u = -1;  // And upper

	// Search the nearest floor whith people queuing
	// ... looking the nearest down
	for(int i=this_floor-1; (i>=0 && found_at_d==-1); --i)
		if(floors_to_be_served[i]) found_at_d = i;
	// ... and up
	for(int i=this_floor+1; (i<FLOORS && found_at_u==-1); ++i)
		if(floors_to_be_served[i]) found_at_u = i;
	
	// If there were no busy floors, say there's nobody
	if(found_at_u==-1 && found_at_d==-1) return -1;

	int nearest;
	// If there's nobody upstairs, nearest is down
	if(found_at_u==-1) nearest = found_at_d;
	// If there's nobody down, nearest is up
	else if(found_at_d==-1) nearest = found_at_u;
	// If there's people up and down, go to nearest floor
	else if(found_at_u-this_floor != this_floor-found_at_d)
		nearest = (found_at_u-this_floor < this_floor-found_at_d) ? found_at_u : found_at_d;
	// In case of parity, keep current inertia to avoid to get stuck
	else {
		if(inertia == 'd') nearest = found_at_d;
		// Statistically, if the lift was stopped, is better to go up, as people usually go
		// from floor x to 0, and not to y>x. In this way, after collecting people were needed,
		// the lift will start going down and collecting more people who are going down too
		else nearest = found_at_u;
	}

	// Signal intention to go to nearest floor, to avoid that multiple lifts are going there
	floors_to_be_served[nearest] = 0;
	return nearest;
}


void try_serve_ppl_queueing(status_t *status, int lift) {
	elevator_t *el = &status->elevators[lift];
	// Get the nearest floor with people queueing
	int nearest_to_serve_floor;
	nearest_to_serve_floor = get_nearest_to_serve_floor(status->to_serve_floors, el->current_floor, el->inertia);

	// If nobody's queuing, stay still
	if(nearest_to_serve_floor==-1) {
		el->inertia = 's';
		return;
	}
	// Otherwise get there
	el->inertia = (nearest_to_serve_floor > el->current_floor) ? 'u' : 'd';
	return;

}


// Check if it would be better to change inertia
void evaluate_inertia_change(status_t *status, int lift) {
	elevator_t *elev = &status->elevators[lift];
	// Current inertia
	int inertia = elev->inertia;
	// Current floor
	int floor = elev->current_floor;

	// Check if elevator reached top or bottom (this inertia can change by the end of function)
	if(floor == 0) elev->inertia = 'u';
	else if(floor == FLOORS-1) elev->inertia = 'd';

	// If there are not passengers, go to serve people in queue //TODO maybe someone else can do this job faster
	if( elev->num_people_inside == 0) {
		try_serve_ppl_queueing(status, lift);
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
void move_elevator(status_t *status, int lift) { //TODO non funziona bene se ci sono più di 12 persone
	elevator_t *elevator = &status->elevators[lift];
	if(elevator->inertia=='u') elevator->current_floor++;
	else if(elevator->inertia=='d') elevator->current_floor--;
}



void time_step(status_t *status) {
	// Check who's arrived
	check_calls(status);
	// For each elevator
	for(int i=0; i<NUM_ELEVATORS; i++) {
		// People who need to exit, get out
		people_get_out(status, i);
		// People get into the elevator
		people_get_in(status, i);
		// Decide which elevator goes where
		evaluate_inertia_change(status, i);
		// Move elevator of one floor (if necessary)
		move_elevator(status, i);
	}
}


//_____________________________________________________________________________________________
// PRINT
//_____________________________________________________________________________________________

void print_queue(list_identifier_t *list_id) {
	person_t *iterator = list_id->head;
	while(iterator->next) {
		printf("{%2d} ", iterator->next->dest);
		iterator = iterator->next;
	}
}


void print_elevators(elevator_t *elevators, int floor) {
	for(int i=0; i<NUM_ELEVATORS; i++) {
		if(elevators[i].current_floor == floor) // If elevator i is at this floor
			printf("[%2c] ", elevators[i].inertia);
		else // Else leave blank space
			printf("     ");
	}
}


void print_floors(status_t *status) {
	// Display people queuing at each floor and the lifts
	// 19    | [ u]     [ s] | { 1} {19} { 3} <- floor example
	int first_col_size = 6; //xx____
	int elevator_space = NUM_ELEVATORS*5;// [xx]_
	
	// Titles of table
	printf("%.*s |%.*s |%s\n", first_col_size, "FLOOR", elevator_space, "ELEVATORS", "QUEUE");
	// Print a line for each floor
	for(int f=FLOORS-1; f>=0; --f) {
		printf("%*d|",first_col_size, f); // Number of floor
		print_elevators(status->elevators, f);
		printf("| ");
		print_queue(&status->floors_queues[f]);
		printf("\n");
	}
}


void print_inside_elevators(status_t *status) {
	// Print also the destinations of people inside elevators
	//LIFT->LIFT #0 |LIFT #1 
	//FLOOR 
	//19    1           -
	//...
	int padding = 8;
	printf("LIFT->");
	for(int i=0; i<NUM_ELEVATORS; ++i) {
		printf("LIFT #%-2d|",i);
	}
	printf("\nFLOOR \n");
	for(int f=FLOORS-1; f>=0; --f) {
		printf("%5d", f);
		for(int e=0; e<NUM_ELEVATORS; ++e) {
			int ppl_in_e_going_f = status->elevators[e].people_destinations[f];
			if(ppl_in_e_going_f) printf("%*d", padding, ppl_in_e_going_f);
			else printf("%.*s", padding, "    -    ");
		}
		printf("\n");
	}
	printf("\nTOTAL");
	for(int e=0; e<NUM_ELEVATORS; ++e) {
		printf("%*d", padding, status->elevators[e].num_people_inside);
	}
	printf("\n");
}


void print_system_status(status_t *status) {
	SPACER
	printf("QUEUES' STATUS\n\n");
	print_floors(status);
	printf("\n\n");
	SPACER
	printf("ELEVATORS' STATUS\n\n");
	print_inside_elevators(status);
	SPACER
}
