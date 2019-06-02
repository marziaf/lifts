#ifndef ELEVATOR_H
#define POL_H

#include "parameters.h"

typedef struct elevator {
	// Vector containing number of people and their destination floor
	// people_inside[x] -> number of people going to floor x
	int people_destinations[FLOORS];
	// Can assume values:
	// u -> going up
	// d -> going down
	// s -> stopped
	char inertia;
	// The floor in which elevator is
	int current_floor;
	// Information about how many people can get in (capacity - number of people inside)
	int num_people_inside;
} elevator_t;

//TODO is empty, next floor(?)

#endif //ELEVATOR_H
