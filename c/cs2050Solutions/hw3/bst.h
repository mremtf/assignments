#ifndef _BST_H_
#define _BST_H_

#define MAX_NAME_LEN 25
#define START_DEPTH 0


typedef struct TreeNode_ {
	char name[MAX_NAME_LEN];
	struct TreeNode_ *left;
	struct TreeNode_ *right;
}TreeNode;

typedef struct StackNode_ {
	TreeNode *t;
	struct StackNode_ *next;
}StackNode;


TreeNode* read_from_file(const char* file);
TreeNode* insert(TreeNode* node, const char *name);
TreeNode* create_node(const char *name);
int size(TreeNode* node);
void display_tree(TreeNode* node, int depth);
int search_name(TreeNode* node, const char *target);
int max_depth(TreeNode* node);
const char* first_name(TreeNode* node);
void mirror_tree(TreeNode* node);
TreeNode* trim_tree(TreeNode* node, const char* start_name, const char* end_name);
void destroy_tree(TreeNode* node);
int number_of_leaves(TreeNode* node);
int is_tree_balanced(TreeNode* node);
char** flatten_tree(TreeNode* node, int *len_strings);

void push_node(StackNode** top, TreeNode* t);
TreeNode* pop_node(StackNode** top);

#endif
