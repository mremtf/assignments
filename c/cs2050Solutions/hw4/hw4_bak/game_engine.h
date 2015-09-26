#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_

#include "utilities.h"
#include "items.h"

// Picks up an item
int pickup(const char* target, Item* item, Player *player);
// leaves the game when this encountered
int quit ();
// Saves all the game data to .sav files
int save (Player *player);
// Loads all the game data from .sav files
int load (Player *player);
// display the game menu
void help();

#endif
