#include <stdio.h>
#include <stdlib.h>
#include "controller.h"

int main() {
	// TODO input file
	// TODO qualcosa nei file andrebbe messo ad unsigned
	char c = 'o';
	c = getchar();
	status_t status = init_status();

	int from, to;
	while(c!='q') {
		switch(c) {
			case 'a':
				printf("from: ");
				scanf("%d",&from);
				printf("to: ");
				scanf("%d",&to);
				add_customer(&status, from, to);
				break;
			case 't':
				time_step(&status);
				break;
			case 'p':
				print_system_status(&status);
				break;
		}
	}

	return 0;
}
