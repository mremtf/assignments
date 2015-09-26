#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<time.h>

#include "items.h"
#include "commands.h"
#include "utilities.h"

#define INPUT_BUFF_SIZE 50

int main (int argc, char *argv[]){
	/*
	 * Creates the game data stuctures
	 * */
	Command *commands = NULL;
	Player *player = create_player();
	Item *items = NULL;
	create_commands_tree(&commands, argv[1]);
	create_items_tree(&items, argv[2]);
	
	char input_buffer[INPUT_BUFF_SIZE];
	do {
		fgets(input_buffer,INPUT_BUFF_SIZE,stdin);
		parse_commands(input_buffer,player,items);	
	}while (1);
}


