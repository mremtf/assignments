#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_STRING_LEN 25

int find_number_of_suffixs(char **string, int size); 
int find_number_of_prefixs(char **strings, int size);
int binary_search(char **strings, char* target, int start_idx, int end_idx);
int histogram(char **strings, int *hist, int size);
int sort(char **string, int size); 
void destroy(char ***strings, int size); 
void allocate(char ***strings, int size);
int insert_data (char **strings,const char *filename, int size); 
int write_data (char **strings, const char *filename, int size);
int file_line_count(const char *filename);


int main (int argc, char *argv[]) {

	if (argc < 4) {
		printf("Missing arguements\n");
		return -1;
	}
	int size = 0;
	if (strcmp(argv[1],"MAX_SIZE") == 0) {
		size = file_line_count(argv[2]);
	}
	else {
		size = atoi(argv[1]);
	}
	char **data;
	int i = 0;
	
	allocate(&data,size);
	insert_data(data,argv[2],size);
	sort_data(data,size);
	
	char yes;
	do {
		char target[MAX_STRING_LEN];
		printf("Searching for: ");
		scanf("%s", target);
		getchar();
		int found = binary_search(data,target,0,size - 1);
		if (found >= 0) {
				printf("%s is in this dataset\n", target);	
		}
		else {
			printf("Not in the dataset!\n");
		}
		printf("Continue? (y/n): ");	
		scanf("%c", &yes);
		getchar();
	}while(yes != 'n');
	write_data(data,argv[3],size);
	destroy(&data,size);
}


int file_line_count(const char *filename) {
	FILE *fp = fopen(filename,"r");
	if (!fp) {
		return -1;
	}
	int ch, number_of_lines = 0;
	do {
		ch = fgetc(fp);
		if (ch == '\n') {
			++number_of_lines;
		}
	} while(ch != EOF);

	fclose(fp);

	return number_of_lines;
}

void allocate(char ***strings, int size) {
	int i = 0;
	int j = 0;
	*strings = malloc(sizeof(char*) * size);
	if (*strings == NULL) {
		printf("Failed to allocate memory\n");
		exit(1);
	}
	for ( ;i < size; ++i) {
		(*strings)[i] = malloc(sizeof(char) * MAX_STRING_LEN);
		if ((*strings)[i] == NULL) {
			printf("Failed to allocate memory\n");
			exit(1);
		}
	}
}

void destroy(char ***strings, int size) {
	int i = 0;
	for ( ;i < size; ++i) {
		free((*strings)[i]);
	}
	free(*strings);
}
// insertion sort
int sort_data(char **strings, int size) {
	char hold[MAX_STRING_LEN];	
	int i = 0;
	int j = 0;
	for ( i = 1; i < size; ++i) {
		strcpy(hold,strings[i]);
		for ( j = i; j > 0 && strcmp(hold, strings[j - 1]) < 0 ; --j) {
				strcpy(strings[j],strings[j - 1]);
		}
		strcpy(strings[j],hold);
	}

	for (i = 0; i < size; ++i) {
		printf("%s\n", strings[i]);
	}
}

int histogram(char **strings, int *hist, int size) {	
	int string_idx = 0;
	int hist_idx = 0;
	for ( ; string_idx < size; ++string_idx) {
		hist[strlen(strings[string_idx])]++;
	}
}

int insert_data (char **strings,const char *filename, int size) {
	int i = 0;
	FILE *fp = fopen(filename, "r");
	for (; i < size; ++i) {
		fscanf(fp,"%s", strings[i]);
	}

	fclose(fp);
}

int write_data (char **strings, const char *filename, int size) {
	FILE * fp = fopen (filename, "w");
	int *hist = calloc(MAX_STRING_LEN,sizeof(int));
	histogram(strings,hist,size);
	int prefix_num = find_number_of_prefixs(strings,size);
	int suffix_num = find_number_of_suffixs(strings,size);
	int i = 0;
	int j = 0;
	for ( ; i < MAX_STRING_LEN; ++i) {
		if (!hist[i]) {
			continue;
		}
		fprintf(fp,"Number of names with %d characters: ", i);
		for (j = 0; j < hist[i]; ++j) {
			fprintf(fp,"*");
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"Number of names with prefix of ma are %d", prefix_num);	
	fprintf(fp,"\nNumber of names with suffix of an are %d\n", suffix_num);
	free(hist);
	fclose(fp);
}

// Recursies BS
int binary_search(char **strings, char* target, int start_idx, int end_idx) {
	if (start_idx > end_idx) return -1;

	int mid_idx = (end_idx + start_idx)/2;
	
	if (strcmp(strings[mid_idx],target) == 0) {
		return mid_idx;
	}
	else if (strcmp(strings[mid_idx],target) < 0) {
		start_idx = mid_idx + 1;
		binary_search(strings,target,start_idx + 1,end_idx);
	}
	else
		end_idx = mid_idx - 1;
		binary_search(strings,target,start_idx,end_idx);

}

int find_number_of_prefixs(char **strings, int size) {
	int i = 0;
	int count = 0;
	for (; i < size; ++i) {
		if (strncmp(strings[i], "ma", strlen("ma")) == 0) {
			++count;
		}
	}
	return count;
}

int find_number_of_suffixs(char **strings, int size) {
	int i = 0;
	int count = 0;
	for (; i < size; ++i) {
		int lensuffix = strlen("an");
		int lenstr = strlen(strings[i]);
		if (lensuffix > lenstr) {
			continue;
		}
		if (strncmp(strings[i] + lenstr - lensuffix, "an",lensuffix) == 0) {
			count++;
		}
	}
	return count;
}
