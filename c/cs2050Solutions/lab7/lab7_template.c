#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

typedef struct TreeNode_ {
	int value;
	struct TreeNode_ *left;
	struct TreeNode_ *right;
}TreeNode;


TreeNode* insert(TreeNode* node, const int value);
TreeNode* create_node(const int value);
void destroy(TreeNode* node);
void display(TreeNode* node);

int main (int argc, char *argv[]) {
		if (argc < 3) {
			printf("%s is missing parameters\n", argv[0]);
			return 0;
		}
		const int size = atoi(argv[1]);
		const int root_val = atoi(argv[2]);
		TreeNode* root = NULL;
		int i;
		root = insert(root,root_val);
		printf("Head node is %d\n", root->value);
		for (i = 0; i < size; ++i) {
			int value = rand() % 10 + 1;
			insert(root,value);
		}
		display(root);
		destroy(root);

}