#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "controller.h"

int main() {
	// TODO input file
	// TODO qualcosa nei file andrebbe messo ad unsigned
	status_t status = init_status();
	
	char c = 'o';
	c = getchar();

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
				printf("queue at 19\n");
				print_queue(&status.floors_queues[19]);
				test(&status);
				//print_system_status(&status);
				break;
		}
		c = getchar();
	}

	return 0;
}
