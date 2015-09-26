#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NAME_LEN 25
#define START_IDX_PEOPLE_ARRAY 0
#define MAX_PEOPLE 10
#define NOT_FOUND -1

typedef struct Person_ {
	char name[MAX_NAME_LEN];
	int age;
}Person;

void read_from_file_people(Person *people, char *filename);
void sort_people(Person *people);
int binary_search(Person *people, char* target, int min_idx, int max_idx);
void display_file_stats(Person *people, int size);
void write_to_file_people(Person *people, char *filename);

int main (int argc, char* argv[]) {

	if (argc < 3) {
		printf("\n%s missing one of the following persons.txt sorted_persons.txt\n\n", argv[0]);
		return 0;
	}

	Person people[MAX_PEOPLE];
	char name[MAX_NAME_LEN] = {0};
	char answer;
	
	read_from_file_people(people,argv[1]);
	sort_people(people);
	
	do {
		printf("\nEnter a name to search for: ");
		scanf("%s", name);
		getchar(); // eat the new line character
		
		const int res = binary_search(people,name,START_IDX_PEOPLE_ARRAY,MAX_PEOPLE);
		if (res >= 0) {
			printf("%s was found at this index %d with this age %d\n", people[res].name, res ,people[res].age);
		}
		else {
			printf("%s is not in this %s\n", name ,argv[1]);
			
		}	
		printf("Continue searching names? (y/n): ");
		scanf("%c", &answer);
		getchar(); // eat the new line character
		memset(name,0,strlen(name) + 1);

	} while (answer == 'y' || answer == 'Y');
	
	display_file_stats(people,MAX_PEOPLE);

	write_to_file_people(people,argv[2]);
	
	return 0;
}

void read_from_file_people(Person *people, char *filename) {
	FILE *input_fp = fopen(filename,"r");
	
	int i = 0;
	for ( ; i < MAX_PEOPLE; ++i) {
		fscanf(input_fp,"%s %d", people[i].name, &people[i].age);
	}

	fclose(input_fp);
}

void sort_people(Person *people){
	int i = 0, j = 0;
	Person p;
	for ( ; i < MAX_PEOPLE; ++i) {
		for (j = 0; j < MAX_PEOPLE - 1; ++j) {
			if (strcmp(people[j].name, people[j + 1].name) > 0) {
				p = people[j + 1];
				people[j + 1] = people[j];
				people[j] = p;
			}
		}
	}
	
}

void write_to_file_people(Person *people, char *filename){
	FILE *output_fp = fopen(filename,"w");
	
	int i = 0;
	for ( ; i < MAX_PEOPLE; ++i) {
		fprintf(output_fp,"%s %d\n", people[i].name, people[i].age);
	}
	fclose(output_fp);

}


void display_file_stats(Person *people, int size) {
	int min_age = INT_MAX;
	int min_age_idx = 0;
	int longest_name_size = 0;
	int longest_name_idx = 0;

	int i = 0;
	for ( ; i < size; ++i) {
		if (min_age > people[i].age) {
			min_age = people[i].age;
			min_age_idx = i;
		}	
		if (longest_name_size < strlen(people[i].name)) {
			longest_name_size = strlen(people[i].name);
			longest_name_idx = i;
		}
	}
	
	printf("\nLongest name is %s with %d characters\n", people[longest_name_idx].name, longest_name_size);
	printf("Youngest person is %s at %d\n\n", people[min_age_idx].name, people[min_age_idx].age);

}

int binary_search(Person *people, char* target, int min_idx, int max_idx) {
	int mid_idx = 0;

	for (;max_idx >= min_idx;) {
		mid_idx = (min_idx + max_idx)/2;

		if (strcmp(people[mid_idx].name,target) == 0) {
			return mid_idx;
		}
		else if (strcmp(people[mid_idx].name,target) < 0) {
			++min_idx;
		}
		else {
			--max_idx;
		}
	}
	return NOT_FOUND;
}
