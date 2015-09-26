#include <stdio.h>
#include <string.h>

#include "commands.h"
#include "utility.h"

int main (int argc, char *argv[]) {
	
	if (argc < 2) {
		printf("%s is missing commands.dat\n", argv[0]);
		return 0;
	}


	Command* options = NULL;
	create_commands_tree(&options,argv[1]);
	int checking = 1;

	char input_buffer[MAX_BUFF_SIZE];

	do {
		printf("Command: ");
		fgets(input_buffer,MAX_BUFF_SIZE,stdin);
		StringArray* parsed_input = tokenizer(input_buffer," \n");
		Command* c = get_command(options,parsed_input->strings[0]);

		if( c && parsed_input->size == c->expected_param_count) {
			if (strcmp(c->name, "quit") == 0){
					checking = 0;
			}
			printf("Valid command used\n");
		}
		else {
			printf("Invalid command, please try again\n");
		}
		free_string_array(parsed_input);
		
	}while (checking);
	printf("\nNumber of commands: %d\n", number_of_commands(options));
	destroy_commands_tree(options);
	
}
