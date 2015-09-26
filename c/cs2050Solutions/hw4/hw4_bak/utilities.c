#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"

Player* create_player() {
	Player* p = malloc(sizeof(Player));
	printf("Enter your name: ");
	scanf("%s", p->name);
	p->collected_items = NULL;
	return p;
}

StringArray* tokenizer (const char string, int *size, const char* delimiters) {

	StringArray* sr = malloc(sizeof(StringArray));
	sr.strings = malloc(sizeof(char*) * MAX_SIZE);
	int i = 0;
	for (i = 0; i < MAX; ++i) {
		sr.strings[i] = malloc(sizeof(char) * MAX_BUFFER_SIZE);
	}

	i = 0;
	char *pch;
	pch = strtok(string,delimiters);
	strcpy(sr.strings[i], pch);
	++i;
	while (pch != NULL) {
		pch = strtok(string,delimiters);
		strcpy(sr.strings[i],pch);
		++i;
	}
	sr.size = i;// return the sizr back to the caller
	return sr;
}

void display_collected_items(Item* node) {
	if (node == NULL) {
		return;
	}	
	display_collected_items(node->left);
	printf("%s %d\n", node->name, node->occurances);
	display_collected_items(node->right);
}

void insert_into_collected_items(Item** node, Item* data) {
	if (*node == NULL)
		*node = data;
	else {
		if (strcmp(data->name,(*node)->name) == 0) {
			data->occurances++; // it's a duplicate
		}
		else if (strcmp(data->name,(*node)->name) < 0){
			insert_into_items_tree(&(*node->left),data);
		}
		else {
			insert_into_items_tree(&(node->right),data);
		}
	}
}

void delete_collected_items(Item* node) {
	
	if (node == NULL) {
		return;
	}
	destroy_items_tree(node->left);
	destroy_items_tree(node->right);
	free(node);
	
}
