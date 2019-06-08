#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "controller.h"

#define ARRIVES "PERSON_ARRIVES"
#define TIME "TIME_STEP"
#define PRINT "PRINT_STATUS"

#define VERBOSE "-v"

int main(int argc, char **argv) {
	// TODO qualcosa nei file andrebbe messo ad unsigned
	
	// _______________________________________________________________________
	// SETUP
	// _______________________________________________________________________

	// Get file to read from
	if(argc == 1) {
		printf("Error.\nUsage: %s command_file [%s]\n", argv[0], VERBOSE);
		return 1;
	}
	FILE *command_file = fopen(argv[1], "r");

	// Check if verbose option is activated
	char verbose = 0;
	if( argc > 2 && strcmp(argv[2], VERBOSE) == 0 ) {
		verbose = 1;
		printf("Verbose mode: press enter to step one command forward\n");
		getchar();
	}

	// Variables where to save commands
	char command[32];  // Where to save command
	int from, to;  // Arriving customer parameters

	// Get context
	status_t status = init_status();
	
	// _______________________________________________________________________
	// START
	// _______________________________________________________________________

	while( fscanf(command_file, "%s", command) == 1 ) {
		// Arrivals
		if( strcmp(command, ARRIVES) == 0 ) {
			// Get customer departure and arrival floor
			fscanf(command_file, "%d %d", &from, &to);
			add_customer(&status, from, to);
			
			if(verbose) {
				printf("A customer arrived at floor %d to go to %d\n", from, to);
			}
		}
		// Time step
		else if ( strcmp(command, TIME) == 0 ) {
			time_step(&status);

			if(verbose) {
				printf("Time++\n");
			}
		}
		// Print
		else if ( strcmp(command, PRINT) == 0 ) {
			print_system_status(&status);
		}

		if(verbose) getchar();  // Wait for user input to go on
	}

	fclose(command_file);

	//TODO free
	return 0;
}
