#include <stdio.h>
#include <string.h>

#include "items.h"
#include "commands.h"
#include "utility.h"

int main (int argc, char *argv[]) {
	
	if (argc < 2) {
		printf("%s is missing commands.dat\n", argv[0]);
		return 0;
	}

	Item* found_stuff = NULL;

	Command* options = NULL;
	create_commands_tree(&options,argv[1]);
	//return 1;
	int collecting = 1;

	char input_buffer[MAX_BUFF_SIZE];

	do {
		printf("Command: ");
		fgets(input_buffer,MAX_BUFF_SIZE,stdin);
		StringArray* parsed_input = tokenizer(input_buffer," \n");
		Command* c = get_command(options,parsed_input->strings[0]);

		if( c && parsed_input->size == c->expected_param_count) {
			if (strcmp(c->name,"quit") == 0) {
				collecting = 0;
			}
			else if (strcmp(c->name, "pickup") == 0){
				insert_into_items_tree(&found_stuff,parsed_input->strings);
			}
			else if (strcmp(c->name, "load") == 0) {
				destroy_items_tree(found_stuff);
				found_stuff = load_saved_items(parsed_input->strings[1]);
			}
			else {
				printf("\nGame Commands:\n");
				display_commands_tree(options);
			}
		}
		else {
			printf("Invalid command, please try again\n");
		}
		free_string_array(parsed_input);
		
	}while (collecting);

	printf("The contents of my bag are: \n");
	display_items_tree(found_stuff);
	destroy_commands_tree(options);
	destroy_items_tree(found_stuff);
	
}
