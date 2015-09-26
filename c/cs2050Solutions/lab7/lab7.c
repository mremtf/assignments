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
int min_value(TreeNode* node); 
int tree_sum(TreeNode* node);
void mirror(TreeNode* node);
int search(TreeNode* node, const int target);

int main (int argc, char *argv[]) {
		if (argc < 4) {
			printf("%s is missing parameters\nShould be %s <size> <root value> <possible numbers>\n", argv[0]);
			return 0;
		}
		srand(time(NULL));
		const int size = atoi(argv[1]);
		const int root_val = atoi(argv[2]);
		const int possible_numbers = atoi(argv[3]);
		TreeNode* root = NULL;
		int i;
		root = insert(root,root_val);
		printf("Head node is %d\n", root->value);
		for (i = 0; i < size; ++i) {
			int value = rand() % possible_numbers + 1;
			insert(root,value);
		}
		int num;
		int cont;
		do {
			printf("Enter a number to search for: ");
			scanf("%d", &num);
			if (search(root,num)) {
				printf("%d is inside the tree\n");
			}
			else {
				printf("%d is not inside the tree\n");
			}
			printf("continue?(1(yes) or 0(no))");
			scanf("%d", &cont);
		}while (cont);
		display(root);
		printf("The min value of the tree is %d\n", min_value(root));
		printf("Mirroring the tree...\n");
		mirror(root);
		display(root);
		printf("Sum of this tree is %d\n", tree_sum(root));
		destroy(root);

}

TreeNode* create_node(const int value) {
	TreeNode* new_node = malloc (sizeof(TreeNode));
	if (!new_node) {
		printf("Memory allocation failure\n");
		exit(1);
	}
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->value = value;
	return new_node;
}

TreeNode* insert(TreeNode* node, const int value) {
	
	if (node == NULL) {
		return create_node(value);
	}
	else {
		
		if (value <= node->value) {
			node->left = insert(node->left,value);
		}
		else {
			node->right = insert(node->right,value);
		}
		
		return node;
	}
}

void destroy(TreeNode* node) {
	if (node == NULL) 
		return;
    destroy(node->left);
    destroy(node->right);
	free(node);
}

void display(TreeNode* node) {
	
	if (node == NULL) {
		return;
	}
	display(node->left);
	printf("%d\n", node->value);
	display(node->right);
}

int min_value(TreeNode* node) {
	TreeNode* current = node;
	
	while (current->left != NULL) {
		current = current->left;
	}
	return(current->value);
}

int tree_sum(TreeNode* node) {
	if (node == NULL) {
		return 0;
	}
	return node->value + tree_sum(node->left) + tree_sum(node->right);
}

void mirror(TreeNode* node) {
	if (node == NULL) {
		return;
	}
	else {
		TreeNode* temp;
		mirror(node->left);
		mirror(node->right);
		
		temp= node->left;
		node->left = node->right;
		node->right = temp;
	}
}

int search(TreeNode* node, const int target) {	
	if (node == NULL) {
		return 0;
	}
	else if (node->value == target) {
		return 1;
	}
	else if (node->value < target) {
		return search(node->left,target);
	}
	else {
		return search(node->right,target);
	}
}

