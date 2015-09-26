
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "items.h"
#include "utilities.h"

void create_items_tree(Items **node, const int file) {
	char buffer[MAX_ITEM_LEN];
	FILE* fp = fopen(file,"r");

	Item* current = *node;

	while (fgets(buffer,MAX_ITEM_NAME_LEN,fp)) {
		StringArray* sr = tokenizer(buffer,",");
		insert_into_items_tree(&current, sr.strings);
	}
}


void insert_into_items_tree(Item **node, char** data) {
	if (*node == NULL)
		*node = create_item(data);
	else {
		if (strcmp(data[1],(*node)->name) <= 0){
			insert_into_items_tree(&(*node->left),data);
		}
		else {
			insert_into_items_tree(&(node->right),data);
		}
	}
}

Item* create_item(char **strings){
	Item* i = malloc(sizeof(Item));
	strcpy(i->name,strings[0]);
	i->occurances = atoi(strings[1]);
	i->left = NULL;
	i->right = NULL;
	return i;
}

//DEEP COPY 
Item* get_item_from_tree (Item* node ,const char* target) {	
	if (node == NULL) {
		return NULL; 
	}
	else if (strcmp(node->name,target) == 0) {
		return node;
	}
	else if (strcmp(target,node->name) < 0) {
		return get_item_from_tree(node->left,target);
	}
	else {
		return get_item_from_tree(node->right,target);
	}
}

void destroy_items_tree(Item* node) {
	if (node == NULL) {
		return;
	}
	destroy_items_tree(node->left);
	destroy_items_tree(node->right);
	free(node);
}

