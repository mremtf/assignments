#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "comamnds.h"
#include "items.h"
#include "utitilies.h"

#define COMMAND_NAME_LEN 25

typedef struct Command_ {
	char name[COMMAND_NAME_LEN];
	Commands_* left;
	Commands_* right;
}Command;

void create_commands_tree(Command **commands, const int file);
Command* get_command(Command *node, const char *command);
void activate_command(const char **commands);
void free_commands_tree(Command **commands);
int parse_commands(const char* commands,Player* player, Item *items); 

#endif
