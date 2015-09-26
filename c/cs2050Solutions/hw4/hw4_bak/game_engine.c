#include "game_engine.h"

// Picks up an item
int pickup (const char* target, Item *items, Player* player) {	
	Item* item = get_items(items,target);
	if (item) {
		insert_into_collected_items(player.collected_items,item);	
		return 1;
	}
	else
		return 0;
}
// leaves the game when this encountered
int quit () {

}
// Saves all the game data to .sav files
int save () {

}
// Loads all the game data from .sav files
int load () {

}
// display the game menu
void help () {
	printf("\nGame commands for Dungeon Escape");
	printf("\n Pickup <target>");
	printf("\n Help ");
	printf("\n Quit ");
	printf("\n Save <file>");
	printf("\n Load <file>");
}
