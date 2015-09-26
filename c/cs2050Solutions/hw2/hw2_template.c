/*
* Name:
* Section:
* Assignment:
* Date:
* */
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>
#include<string.h>

#define MAX_STR_LEN 25

typedef struct Data_ {

	char *name;
	struct Data_ *nextData;	

}Data;

// KEEP THIS STRUCTURE
typedef struct Timer_ {
	double start;
	double end;
}Timer;

// USED FOR BUBBLE SORT
Data* bubble_sort(Data *list);
//USED FOR MERGE SORT
int length(Data* list);
void move_node(Data** destRef, Data** sourceRef);
void split_in_half(Data *source, Data **frontRef, Data** backRef);
Data* merge(Data *a, Data* b);
void merge_sort(Data **list);
// USED FOR creating out list
void push(Data **head, char *name);
//USED FOR freeing our list
void destroy(Data *list);
//Allows us to open a file and create our linked list
Data* read_from_file(const char* file, const int size);
// shows to the user the linked list
void display(Data *list);
// USED FOR TIMING OUR CODE
void start_timer(Timer* t);
double get_time_diff(Timer* t);
void end_timer(Timer *t);


int main (int argc, char **argv) {
	/*
	 * Error checking the number of passed in parameters
	 * */
	if (argc < 3) {
		printf("Not enough parameters bub!\n");
		exit(0);
	}
	/*
	 * Opening file for writng the timing output
	 * */
	FILE* fp = fopen("timing.out", "w");
	
	
	Data *head = NULL;
	// Pulling the number of names needed to read into my program
	const int size = atoi(argv[2]);
	
	/*
	 * Setting up timer structure
	 * */
	Timer *t = malloc(sizeof(Timer));
	t->start = 0;
	t->end = 0;

	/*
	 * Timing read from file
	 * */
	start_timer(t);
	head = read_from_file(argv[1], size);
	end_timer(t);
	fprintf(fp,"\nReading from the %s %d took %lf msecs\n\n", argv[1], size,get_time_diff(t));
	
	/*
	 * Timing the time taken to sort with bubble sort
	 * */
	fprintf(fp,"Bubble Sort\n");
	start_timer(t);
	head = bubble_sort(head);
	end_timer(t);
	fprintf(fp,"Sorting the linked list with bubble sort\ntook %lf msecs for %d nodes\n\n", get_time_diff(t), size);
	display(head);
	/*
	 * Destroying current head and reassigning back to NULL
	 * */
	destroy(head);
	head = NULL;
	head = read_from_file(argv[1], size);
	
	/*
	 * Timing the time taken to sort with merge sort
	 * */
	fprintf(fp,"\nMerge Sort\n");
	start_timer(t);	
	merge_sort(&head);
	end_timer(t);
	fprintf(fp,"Sorting the linked list with merge sort\ntook %lf msecs for %d nodes\n\n", get_time_diff(t), size);
	display(head);

	/*
	 * Clean up of allocations and open files
	 * */
	destroy(head);
	free(t);
	fclose(fp);
	// destroy list
	return 0;
}

/*
 * Timing functions (DO NOT DELETE THEM!)
 **/
void start_timer(Timer *t) {
  struct timeval tp;
  int rtn;
  rtn=gettimeofday(&tp, NULL);
  t->start =  (double)tp.tv_sec+(1.e-6)*tp.tv_usec;
}

double get_time_diff(Timer *t) {

	return t->end - t->start;
}

void end_timer(Timer *t) {
  struct timeval tp;
  int rtn;
  rtn=gettimeofday(&tp, NULL);
  t->end =  (double)tp.tv_sec+(1.e-6)*tp.tv_usec;
}

void destroy(Data* list) {

}

Data* read_from_file(const char *file, const int size){
	
}

void push(Data **head, char *name) {

}

Data* bubble_sort(Data *list) {

}

Data* swap(Data *left, Data *right) {

}

void display(Data *list) {


}

/*
 * Used to find the length of an linked list useful inside split_in_half function
 * */
int length(Data* list) {
	int count = 0;
	Data* current = list;
	while(current) {
		current = current->nextData;
		++count;
	}
	return count;
}

void split_in_half(Data *source, Data **frontRef, Data** backRef) {

}

/*
 * Take the node from the front of the source, and move it to the front 
 * of the dest. It is an error to call this with the sourceRef list is empty.
 *
 * before calling move_node():
 * source == {1,2,3}
 * dest == {1,2,3}
 *
 * After calling move_node():
 * source == {2,3}
 * dest == {1,1,2,3}
 * */
void move_node(Data** destRef, Data** sourceRef) {
	Data* newData = *sourceRef;
	*sourceRef = newData->nextData;
	newData->nextData = *destRef;
	*destRef = newData;
}

Data* merge(Data *a, Data* b) {

}

void merge_sort(Data **list) {

}

