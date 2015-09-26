#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#define INIT_SIZE 8
#define MULTIPLER 2

typedef struct Array_ {
	
	int data;

}DynamicArray;

typedef struct ArrayInfo_ {

	unsigned int max_size;
	unsigned int curr_idx;
	
}DynamicArrayInfo;


DynamicArrayInfo info;

void allocate(DynamicArray ** v);
void destroy(DynamicArray **v);
void resize(DynamicArray **v);
void populate(DynamicArray **v,unsigned int size);
int sort(DynamicArray *v, unsigned int size);
void display(DynamicArray *v, unsigned int size); 
void append(DynamicArray **v, unsigned int value);

int main (int argc, char *argv[]) {

	if (argc < 2) {
		printf("Failure to use program right\n");
		return -1;
	}
		info.curr_idx = 0;

	srand(time(NULL));

	const unsigned int number_elems = atoi(argv[1]);
	info.max_size = number_elems;

	
	DynamicArray *rand_data = NULL;

	allocate(&rand_data);
	populate(&rand_data, number_elems);	
	sort(rand_data, info.curr_idx);
	display(rand_data,info.curr_idx);
	destroy(&rand_data);

	printf("Final Stats from info [last index: %d current max size: %d]\n", info.curr_idx, info.max_size);
	//destroy(NULL);
	return 0;
}


void allocate(DynamicArray **v) {
	*v = malloc(sizeof(DynamicArray) * info.max_size);
	if (!*v) {
		printf("Can't allocate memory terminating the program\n");
		exit(1);
	}

}
void resize(DynamicArray **v) {
	
	DynamicArray *new_v;
	info.max_size = info.max_size * MULTIPLER;
	allocate(&new_v);
	memcpy(new_v, *v, sizeof(DynamicArray) * (info.curr_idx + 1));
	destroy(v);
	
	*v = new_v;
}

void append(DynamicArray **v, unsigned int value) {

	if (info.curr_idx >= info.max_size){
		resize(v);
	}
	//append at the current index
	DynamicArray* temp = *v;
	(temp + info.curr_idx)->data = value;
	info.curr_idx++;
}

void destroy(DynamicArray **v) {
	free(*v);
}

void populate(DynamicArray **v, unsigned int size) {
	unsigned int i = 0;
	int res = 0;
	for ( ; i < size; ++i) {
		res = rand() % 100 + 1;
		append(v,res);
	}
}

void display(DynamicArray *v, unsigned int size) {
	unsigned int i = 0;
	DynamicArray * temp = v;
	//printf("Printing off the Dynamic Array: \n");
	if (size > 0) {
		printf("%d\n", (temp + size - 1)->data);
		display(v,--size);
	}
	//printf("\n");
	return;
}
// Selection sort
int sort(DynamicArray *v, unsigned int size) {
	int i = 0, j = 0;
	DynamicArray temp;
	for (; i < size; ++i) {
		for (j = 0; j < size - 1; ++j) {
			if ((v + j)->data > (v +j + 1)->data) {
				temp = *(v + j);
				*(v + j) = *(v +j + 1);
				*(v +j + 1) = temp;

			}
		}
	}
	return 0;
}
