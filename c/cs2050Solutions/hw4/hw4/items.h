#ifndef _ITEMS_H_
#define _ITEMS_H_

#define MAX_ITEM_NAME_LEN 25

typedef struct Item_ {
	char name[MAX_ITEM_NAME_LEN];
	int occurances;
	struct Item_ *left;
	struct Item_ *right;
}Item;

void insert_into_items_tree(Item** node, char** data);
Item* create_item(char **strings);
void destroy_items_tree(Item* node);
Item* load_saved_items(const char *file);
void save_all_items(Item* node, const char *file);
void display_items_tree(Item *node);
#endif
