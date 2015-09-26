#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>
#include<string.h>
#include<sys/time.h>

#define MAX_STR_LEN 25

typedef struct Data_ {

	char *name;
	struct Data_ *nextData;	

}Data;


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
	fprintf(fp,"\nReading from the %s %d\ntook %lf seconds\n\n", argv[1], size,get_time_diff(t));
	
	/*
	 * Timing the time taken to sort with bubble sort
	 * */
	printf("Bubble Sort\n");
	fprintf(fp,"Bubble Sort\n");
	start_timer(t);
	head = bubble_sort(head);
	end_timer(t);
	fprintf(fp,"Sorting the linked list with bubble sort\ntook %lf seconds for %d nodes\n\n", get_time_diff(t), size);
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
	printf("Merge Sort\n");
	fprintf(fp,"\nMerge Sort\n");
	start_timer(t);	
	merge_sort(&head);
	end_timer(t);
	fprintf(fp,"Sorting the linked list with merge sort\ntook %lf seconds for %d nodes\n\n", get_time_diff(t), size);
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
	Data *curr = list;
	Data *needs_freeing;
	
	while (curr) {
		needs_freeing = curr;
		curr = curr->nextData;
		free(needs_freeing->name);
		free(needs_freeing);
	}
}

Data* read_from_file(const char *file, const int size){
	
	FILE *fp = fopen(file,"r");
	if (!fp) {
		exit(1);
	}
	char name[MAX_STR_LEN];
	Data *head = NULL;
	
	int i = 0;
	for ( ;i < size; ++i) {
		fscanf(fp,"%s", name);
		push(&head,name);
	}
	fclose(fp);
	return head;
}

void push(Data **head, char *name) {
	
	Data *new_node = malloc(sizeof(Data));
	new_node->name = malloc(sizeof(char) * MAX_STR_LEN);
	strncpy(new_node->name,name, MAX_STR_LEN);
	new_node->nextData = *head;
	*head = new_node;
}

Data* bubble_sort(Data *list) {

	int num_nodes = length(list);
	int idx = 0;

	for (idx = 0; idx < num_nodes; ++idx) {
		Data* current = list;
		Data* next = current->nextData;
		Data* prev = NULL;
		while (next != NULL) {
			if (strcmp(current->name, current->nextData->name) < 0) {
				if (current == list) {
					list = next;
				}
				else {
					prev->nextData = next;
				}

				current->nextData = next->nextData;
				next->nextData = current;

				prev = next;
				next = current->nextData;
			}
			else {
				prev = current;
				current = current->nextData;
				next = current->nextData;
			}
		}
	}
	return list;
	/*Data *p = NULL;
	Data *q = NULL;
	Data *top = NULL;
	int changed = 1;

	top = malloc(sizeof(Data));
	if (top == NULL) {
		exit(1);
	}
	top->nextData = list;
	if (list != NULL && list->nextData != NULL) {
		while (changed) {
			p = top;
			q = top->nextData;
			changed = 0;
			while(p->nextData != NULL) {
				if (strcmp(p->name, p->nextData->name) < 0) {
					q->nextData = swap(p,p->nextData);
					changed = 1;
				}
				q = p;
				if ( p->nextData != NULL)
					p = p->nextData;
			}
		}
	}
	p = top->nextData;
	free(top);
	return p;*/
}

Data* swap(Data *left, Data *right) {
	left->nextData = right->nextData;
	right->nextData = left;
}

void display(Data *list) {
	if (!list) {
		printf("LIST IS EMPTY\n");
		exit(1);
	}	
	int i = 0;
	Data *current = list;
	for ( ; current ; current = current->nextData) {
		printf("%s\n", current->name);
	}

}

/*
 * Used to find the length of an array useful inside front back split function
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
	int len = length(source);

	int i = 0;

	Data* current = source;

	if (len < 2){
		*frontRef = source;
		*backRef = NULL;
	}
	else {
		int hopCount = (len - 1)/2;
		for (i = 0; i < hopCount;++i) {
			current = current->nextData;
		}
		*frontRef = source;
		*backRef = current->nextData;
		current->nextData = NULL;
	}
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
	Data *result = NULL;
	Data **lastPtrRef = &result;
	
	while (1) {
		if (a == NULL) {
			*lastPtrRef = b;
			break;
		}
		else if (b == NULL) {
			*lastPtrRef = a;
			break;
		}
		if (strcmp(a->name,b->name) > 0) {
			move_node(lastPtrRef, &a);	
		}
		else {
			move_node(lastPtrRef,&b);
		}

		lastPtrRef = &((*lastPtrRef)->nextData);
	}
	return(result);
}

void merge_sort(Data **list) {
	Data* head = *list;
	Data* a;
	Data* b;

	if ((head == NULL) || (head->nextData == NULL)){
		return;
	}
	
	split_in_half(head, &a, &b);
	
	merge_sort(&a);
	merge_sort(&b);

	*list = merge(a,b);

}

