#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_STR_LEN 25

typedef struct FreightCars_ {
	char *name;
	int number;
	struct FreightCars_ *next_car;
}FreightCar;


void add_car(FreightCar *engine);
void update_car(FreightCar *engine);
void remove_next_car(FreightCar *engine);
void display_train(FreightCar *engine);
void free_cars (FreightCar *engine);


int main (int argc, char **argv) {
	if (argc < 2) {
		printf("Missing required arguements for %s\n", argv[0]);
		return 1;
	}
	const int num_cars = atoi(argv[1]);
	//Create head node
	FreightCar *engine = malloc(sizeof(FreightCar));
	engine->name = "Union Pacific Train";
	engine->number = 10;
	engine->next_car = NULL;

	int i = 0;
	for (; i < num_cars; ++i) {
		add_car(engine);
	}
	display_train(engine);
	update_car(engine);
	display_train(engine);
	remove_next_car(engine);
	display_train(engine);
	free_cars(engine);
	free(engine);
}
void add_car(FreightCar *engine) {
	if (!engine) {
		exit(1);
	}
	
	
	FreightCar *curr_car = engine;
	while (curr_car->next_car != NULL) {
		curr_car = curr_car->next_car;
	}

	curr_car->next_car = malloc(sizeof(FreightCar));
	curr_car->next_car->name = malloc(sizeof(char) * MAX_STR_LEN);
	curr_car->next_car->next_car = NULL;
	printf("Freight car name and number: ");
	scanf("%s %d", curr_car->next_car->name, &curr_car->next_car->number);
	getchar();
	
}

void free_cars (FreightCar *engine) {
	
	FreightCar *curr_car = engine->next_car;
	FreightCar *needs_freeing;
	
	while (curr_car) {
		needs_freeing = curr_car;
		curr_car = curr_car->next_car;
		free(needs_freeing->name);
		free(needs_freeing);
	}
}

void update_car(FreightCar *engine) {
	char target[MAX_STR_LEN] = {0};
	int change = 0;
	printf("Freight car to update number: ");
	scanf("%s", target);
	getchar();

	FreightCar *curr_car = engine;
	while (curr_car != NULL) {
		if (strcmp(curr_car->name, target) == 0) {
			printf("New number for %s is: ", curr_car->name);
			scanf("%d", &curr_car->number);
			getchar();
			change = 1;
		}
		curr_car = curr_car->next_car;
	}
	if (!change) {
		printf("Freight Car not found\n");
	}

}

void remove_next_car(FreightCar *engine) {
	
	if (engine->next_car == NULL) {
		printf("No freight cars to remove\n");
		return;
	}

	FreightCar *temp = engine->next_car;
	engine->next_car = engine->next_car->next_car;
	free(temp->name);
	free(temp);

}

void display_train(FreightCar *engine) {
	int count = 0;
	printf("%s %d has\n", engine->name,engine->number);	
	FreightCar *curr_car = engine->next_car;
	for ( ; curr_car; curr_car = curr_car->next_car ) {
		printf("%s %d\n", curr_car->name, curr_car->number);
		count = 1;
	}
	if (!count) printf("No cars attached to this train\n");
}
