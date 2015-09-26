#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utility.h"

#define MAX_SPLIT_SIZE 50

/*StringArray* tokenizer (char *string, const char* delimiters){

	StringArray* sr = malloc(sizeof(StringArray));
	sr->strings = malloc(sizeof(char*) * MAX_SPLIT_SIZE);
	int i = 0;
	for (i = 0; i < MAX_SPLIT_SIZE; ++i) {
		sr->strings[i] = malloc(sizeof(char) * MAX_BUFF_SIZE );
	}

	i = 0;
	char *pch;
	pch = strtok(string,delimiters);
	strcpy(sr->strings[i], pch);
	++i;
	while (pch != NULL) {
		pch = strtok(NULL,delimiters);
		if (pch == NULL) {
			break;
		}
		strcpy(sr->strings[i],pch);
		++i;
	}
	sr->size = i;// return the sizr back to the caller
	return sr;
}*/

StringArray* tokenizer(char *string, const char* delimiters){
	StringArray* sr = malloc(sizeof(StringArray));
	sr->strings = malloc(MAX_SPLIT_SIZE * sizeof(char *));
	
	char* hold;
	
	(sr->strings)[0] = malloc(MAX_BUFF_SIZE * sizeof(char));
	hold = strtok(string, delimiters);
	strcpy((sr->strings)[0], hold);
	int i;
	for (i = 1; i < MAX_SPLIT_SIZE; i++){	
		hold = strtok(NULL, delimiters);
		if (hold == NULL){
			sr->size = i;
			break;
		}	
		(sr->strings)[i] = malloc(MAX_BUFF_SIZE * sizeof(char));
		strcpy((sr->strings)[i], hold);

	}
	return sr;
}
//LABCODE: LAST LAB
// make clean
//cd ..
//tar -zcvf lab8.tgz lab8/
//cs_submit CS2050_LAB-B LAB8 lab8.tgz
//
void free_string_array(StringArray *sr) {
	int i;
	for (i = 0; i < sr->size; ++i) {
		free(sr->strings[i]);
	}
	free(sr->strings);
	free(sr);
}
