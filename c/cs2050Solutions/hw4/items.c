
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "items.h"
#include "utility.h"

void insert_into_items_tree(Item **node, char** data) {
	if (*node == NULL)
		*node = create_item(data);
	else {
		if (strcmp(data[1],(*node)->name) == 0) {
			(*node)->occurances += atoi(data[2]);	
		}
		else if (strcmp(data[1],(*node)->name) < 0){
			insert_into_items_tree(&((*node)->left),data);
		}
		else {
			insert_into_items_tree(&((*node)->right),data);
		}
	}
}

Item* create_item(char **strings){
	Item* i = malloc(sizeof(Item));
	strcpy(i->name,strings[1]);
	i->occurances = atoi(strings[2]);
	i->left = NULL;
	i->right = NULL;
	return i;
}

void destroy_items_tree(Item* node) {
	if (node == NULL) {
		return;
	}
	destroy_items_tree(node->left);
	destroy_items_tree(node->right);
	free(node);
}

Item* load_saved_items(const char *file) {	
	char buffer[MAX_BUFF_SIZE];
	FILE* fp = fopen(file,"r");

	Item* root = NULL;

	while (fgets(buffer,MAX_BUFF_SIZE,fp)) {
		StringArray* sr = tokenizer(buffer,",\n");
		insert_into_items_tree(&root, sr->strings);
	}
	display_items_tree(root);
	fclose(fp);
	return root;
}

void display_items_tree(Item *node) {
	if (node == NULL) {
		return;
	}
	display_items_tree(node->left);
	printf("%s %d\n", node->name, node->occurances);
	display_items_tree(node->right);
}
