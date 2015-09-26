#ifndef _ITEMS_H_
#define _ITEMS_H_

#define MAX_ITEM_NAME_LEN 25

typedef struct Item_ {
	char name[MAX_ITEM_NAME_LEN];
	int occurances;
	struct Item_ *left;
	struct Item_ *right;
}Item;

void create_items_tree(Item **node, const int file);
void insert_into_items_tree(Item** node, char** data);
Item* create_item(char **strings);
Item* get_item (Item* node ,const char* target);
void destroy_items_tree(Item* node);

#endif
