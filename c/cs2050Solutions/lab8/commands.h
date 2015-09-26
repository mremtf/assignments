#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#define COMMAND_NAME_LEN 50

typedef struct Command_ {
	char name[COMMAND_NAME_LEN];
	int expected_param_count;
	struct Command_ *left;
	struct Command_ *right;
}Command;

void create_commands_tree(Command **commands, const char *file);
void insert_into_commands_tree(Command** node, char** data);
Command* get_command(Command *node, const char *command);
Command* create_command(char **data);
void destroy_commands_tree(Command* node);
void display_commands(Command *node);
int number_of_commands(Command *node); 
#endif
