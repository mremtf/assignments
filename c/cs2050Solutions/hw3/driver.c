#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#include "bst.h"


int main (int argc, char *argv[]) {
	
	if (argc < 2) {
		printf("%s is missing parameters to run properly\n", argv[0]);
		return 1;
	}
	TreeNode* root = NULL;
	root = read_from_file(argv[1]);
	char buffer[MAX_NAME_LEN];
	printf("Would you like to search for a name? (yes or no): ");
	scanf("%s", buffer);
	getchar();
	
	while (strcmp(buffer,"yes") == 0) {
		printf("Name: ");
		scanf("%s", buffer);
		getchar();
		int result = search_name(root,buffer);
		if (result) {
			printf("\n%s is present in the database", buffer);
		}
		else {
			printf("\n%s is not present in the database", buffer);
		}
		
		printf("\nWould you like to search for a name? (yes or no): ");
		scanf("%s", buffer);
		getchar();
	}
	
	display_tree(root,START_DEPTH);
	printf("The number of records in the database is %d", size(root));
	printf("\nThe max depth of the database is %d\n", max_depth(root));
	printf("The number of leaf records are %d\n", number_of_leaves(root));
	printf("Is the database balanced (%s)\n", is_tree_balanced(root) ? "yes" : "no");
	printf("The tree mirrored looks like this\n\n");
	mirror_tree(root);
	display_tree(root,START_DEPTH);
	mirror_tree(root);
	char buffer2[MAX_NAME_LEN];
	printf("\n\nEnter the lower bound name and upper bound name: ");
	scanf("%s %s", buffer,buffer2);
	printf("Trimming the database from %s to %s\n\n", buffer, buffer2);
	root = trim_tree(root,buffer,buffer2);
	display_tree(root,START_DEPTH);
	int size = 0;
	char** strings = flatten_tree(root,&size);
	int i = 0;
	printf("\nFlattened databse is:\n");
	for (i = 0; i < size; ++i) {
		printf("%s\n", strings[i]);
		free(strings[i]);
	}
	free(strings);
	destroy_tree(root);
	return 0;
}
