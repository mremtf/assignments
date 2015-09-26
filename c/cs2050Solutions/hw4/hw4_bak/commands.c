#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "commands.h"
#include "game_engine.h"

void create_commands_tree(Command **commands, const int file) {
	char buffer[COMMAND_NAME_LEN];
	int cost, damage;
	FILE* fp = fopen(file,"r");

	Commands* root = *commands;

	while (fgets(buffer,COMMAND_NAME_LEN,fp)) {
		StringArray* sr = tokenizer(buffer,",");
		insert_into_commandss_tree(&root, sr.strings);
	}
}

void insert_into_commands_tree(Command** node, char** data) {
	if (*node == NULL)
		*node = create_weapon(data);
	else {
		if (strcmp(data[1],(*node)->name) <= 0){
			insert_into_commands_tree(&(*node->left),data);
		}
		else {
			insert_into_commands_tree(&(node->right),data);
		}
	}
}

Command* get_command(Command *node, ,const char *command) {

	if (node == NULL) {
		return NULL;
	}
	else if (strcmp(node->value,command) == 0) {
		char* c = malloc(sizeof(char) * command);
		strcpy(c,command);
		return c;
	}
	else if (strcmp(command, node->value) < 0) {
		return search(node->left,target);
	}
	else {
		return search(node->right,command);
	}
}

int parse_commands(const char* commands,Player* player, Item *items, Player *player) {
	StringArray* str = tokenizer(commands," ");
	GameCommand* gc = get_command(node,str[0]);
	if (gc && str.size == gc.size) {
		activate_command(str.name, items, player);
		return 1;
	}
	else {
		return 0;
	}
}

void activate_command(const char **commands,Item *items, Player* player) {
	if (strcmp(commands[0],"pickup") == 0){
		if ( pickup(commands[1], items, player) ) {
			printf("%s successfuly picked up %s from this sack\n", commands[1]);
		}
	}
	else if (strcmp(commands[0],"quit") == 0){
		quit();
	}
	else if (strcmp(commands[0],"save") == 0){
		save(commands[1],items, player);
	}
	else if (strcmp(commands[0],"load") == 0){
		load(commands[1], items, player);
	}
	else 
		help();
		
}

void destroy_commands_tree(Weapon* node) {
		if (node == NULL) {
		return;
	}
	destroy_items_tree(node->left);
	destroy_items_tree(node->right);
	free(node);
}
