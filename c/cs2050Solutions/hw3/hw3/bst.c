#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bst.h"

TreeNode* read_from_file(const char* file) {
	FILE* fp = fopen(file, "r");
	char buffer[MAX_NAME_LEN];
	if (fscanf(fp,"%s", buffer) != 1){
		fclose(fp);
		exit(1);
	}
	TreeNode *t = NULL;
	t = insert(t,buffer);
	while( fscanf(fp,"%s", buffer) == 1 ) {
		insert(t,buffer);
	}
	fclose(fp);
	return t;
}
TreeNode* insert(TreeNode* node, const char *name) {
	if (node == NULL) {
		return create_node(name);
	}
	if (strcmp(name, node->name) > 0) {
		node->left = insert(node->left,name);
	}
	else {
		node->right = insert(node->right,name);
	}
	return node;
}
TreeNode* create_node(const char *name) {
	TreeNode* new = malloc(sizeof(TreeNode));
	strcpy(new->name,name);
	new->left = NULL;
	new->right = NULL;
	return new;
}
int size(TreeNode* node) {
	if (node == NULL) {
		return 0;
	}
	else {
		return (size(node->left) + 1 + size(node->right));
	}
}

int search_name(TreeNode* node, const char *target) {
	if (node == NULL) {
		return 0;
	}
	else if (strcmp(node->name, target) == 0) {
		return 1;
	}
	else if (strcmp(target, node->name) < 0) {
		return search_name(node->left,target);
	}
	else {
		return search_name(node->right,target);
	}
}
int max_depth(TreeNode* node) {
	if (node == NULL) {
		return 0;
	}
	else {
		int ld = max_depth(node->left);
		int rd = max_depth(node->right);

		if (ld > rd)
			return (ld + 1);
		else
			return (rd + 1);
	}
}

void mirror_tree(TreeNode* node) {
	if (node == NULL) {
		return;
	}
	else {
		TreeNode* temp;

		mirror_tree(node->left);
		mirror_tree(node->right);

		temp = node->left;
		node->left = node->right;
		node->right = temp;
	}
}
TreeNode* trim_tree(TreeNode* node, const char* start_name, const char* end_name) {
	
	if (node == NULL) {
		return NULL;
	}
	node->left = trim_tree(node->left, start_name,end_name);
	node->right = trim_tree(node->right, start_name, end_name);

	if (strcmp(node->name, start_name) < 0) {
		TreeNode* rChild = node->right;
		free(node);
		return rChild;
	}

	if (strcmp(node->name, end_name) > 0) {
		TreeNode* lChild = node->left;
		free(node);
		return lChild;
	}
	return node;
}
void destroy_tree(TreeNode* node) {
	if (node == NULL) {
		return;
	}
	destroy_tree(node->left);
	destroy_tree(node->right);
	free(node);
}
int number_of_leaves(TreeNode* node) {
	if (node == NULL)
		return 0;
	if (node->left == NULL && node->right == NULL) {
		return 1;
	}
	else {
		return number_of_leaves(node->left) + number_of_leaves(node->right);
	}
}

int is_tree_balanced(TreeNode* node) {
	int lh;
	int rh;

	if (node == NULL) {
		return 1;
	}

	lh = max_depth(node->left);
	rh = max_depth(node->right);

	if (abs(lh - rh) <= 1 && is_tree_balanced(node->left) && is_tree_balanced(node->right)) 
		return 1;
	
	return 0;

}
char** flatten_tree(TreeNode* node, int *len_strings) {

	int tree_size = size(node);
	*len_strings = tree_size;
	char** str = malloc(sizeof(char*) * tree_size);
	int i = 0;
	for (; i < tree_size; ++i) {
		str[i] = malloc(sizeof(char) * MAX_NAME_LEN);
	}
	TreeNode* current = node;
	StackNode* s = NULL;
	int done = 0;
	
	i = 0;
	while (!done) {
		if (current != NULL) {
			push_node(&s,current);
			current = current->left;
		}
		else {
			if (s) {
				current = pop_node(&s);
				strcpy(str[i],current->name);
				current = current->right;
				++i;
			}
			else
				done = 1;
		}
	}
	return str;
}

void push_node(StackNode** top, TreeNode* t) {
	StackNode* new = malloc(sizeof(StackNode));
	new->t = t;
	new->next = *top;
	*top = new;
}

TreeNode* pop_node(StackNode** top_ref) {
	TreeNode* res;
	StackNode *top;
	
	top = *top_ref;
	res = top->t;
	*top_ref = top->next;
	free(top);
	return res;
}

void padding (char ch, int n){

    	int i;
    	for (i = 0; i < n; i++)
        	printf("%c%c%c%c", ch, ch ,ch, ch);

}

void display_tree (TreeNode *root, int depth) {
	TreeNode *current = root;
	if (root == NULL) {
        	padding (' ', depth);
        	printf("-\n");
    	}
    else {
		display_tree(current->left, depth+1);
       	padding(' ', depth);
       	printf ( "%s\n", root->name);
       	display_tree(current->right, depth+1);
    }
}
