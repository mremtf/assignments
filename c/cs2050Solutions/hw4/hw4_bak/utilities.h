#ifndef _UTITLIES_H_
#define _UTITLIES_H_
#include "items.h"

typedef struct StringArray_ {
	char **strings;
	int size;
}StringArray

typedef struct Player_ {
	char name[MAX_PLAYER_NAME_LEN];
	Item* collected_items;
}Player;

StringArray* tokenizer (const char string, int *size, const char* delimiters);
void display_collected_items(Item* node);
void insert_into_collected_items(Item* node);
void delete_collected_items(Item* node);

#endif
