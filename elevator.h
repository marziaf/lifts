#ifndef ELEVATOR_H
#define POL_H

#include "parameters.h"


/** Struct elevator
 * Represents an elevator and contains informations about
 * people destinations, current floor, current inertia and
 * the number of people inside.
 * */
typedef struct elevator {

	// Vector containing number of people and their destination floor
	// people_destinations[x] == number of people going to floor x
	int people_destinations[FLOORS];

	// Inertia can assume values:
	// u -> going up
	// d -> going down
	// s -> stopped
	char inertia;

	// The floor in which the elevator is
	int current_floor;

	// Number of people inside the elevator
	int num_people_inside;

} elevator_t;

#endif //ELEVATOR_H
