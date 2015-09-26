#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "commands.h"
#include "utility.h"

void create_commands_tree(Command **commands, const char *file) {
	char buffer[COMMAND_NAME_LEN];
	int cost, damage;
	FILE* fp = fopen(file,"r");

	//Command* root = *commands;

	while (fgets(buffer,COMMAND_NAME_LEN,fp)) {
		StringArray* sr = tokenizer(buffer,",\n");
		insert_into_commands_tree(commands, sr->strings);
	}
	fclose(fp);
}

void insert_into_commands_tree(Command** node, char** data) {
	if (*node == NULL) {
		*node = create_command(data);
	}	
	else if (strcmp(data[0], (*node)->name) < 0) {
		insert_into_commands_tree(&(*node)->left,data);
	}
	else if (strcmp(data[0], (*node)->name) > 0) {
		insert_into_commands_tree(&(*node)->right,data);
	}
}

Command* create_command(char **data) {
	Command *new = malloc(sizeof(Command));
	new->right = NULL;
	new->left = NULL;
	strcpy(new->name,data[0]);
	new->expected_param_count = atoi(data[1]);
	return new;
}

Command* get_command(Command *node, const char *command) {

	if (node == NULL) {

		return NULL;
	}
	
	if (strcmp(node->name,command) == 0) {
		return node;
	}
	else if (strcmp(command, node->name) < 0) {
		return get_command(node->left,command);
	}
	else {

		return get_command(node->right,command);
	}
}

void destroy_commands_tree(Command* node) {
	if (node == NULL) {
		return;
	}
	destroy_commands_tree(node->left);
	destroy_commands_tree(node->right);
	free(node);
}

void display_commands(Command *node) {
	printf("\npickup <item>");
	printf("\nhelp ");
	printf("\nquit ");
	printf("\nload <file>\n\n");
}

int number_of_commands(Command *node) {
	if (node == NULL) {
		return 0;
	}
	else {
		return 1 + number_of_commands(node->left) + number_of_commands(node->right);
	}
}
