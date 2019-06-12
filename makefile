edit : main.o controller.o queue.o
	gcc -O2 -o lift main.o controller.o queue.o
main.o : main.c controller.h
	gcc -c main.c
controller.o : controller.c controller.h queue.h parameters.h
	gcc -c controller.c
queue.o : queue.c queue.h parameters.h
	gcc -c queue.c
clean :
	rm lift main.o controller.o queue.o
