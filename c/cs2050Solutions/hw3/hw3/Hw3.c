/* Name:Reem alharbi
 * Section:B
 * Date:before July 16, 2014
 * Assignment: Home work 3 - BST Tree. 
 * */
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<math.h>

#define MAX_NAME_LEN 25
#define START_DEPTH 0

/*
 * Basic Tree Node Structure
 * */
typedef struct TreeNode_ {
	char name[MAX_NAME_LEN];
	struct TreeNode_ *left;
	struct TreeNode_ *right;
}TreeNode;

/*
 * Basic Stack Structure
 * */
typedef struct StackNode_ {
	TreeNode *t;
	struct StackNode_ *next;
}StackNode;

/*
 * Binary Tree Prototypes
 * */
TreeNode* read_from_file(const char* file);
TreeNode* insert(TreeNode* node, const char *name);
TreeNode* create_node(const char *name);
int size(TreeNode* node);
void display_tree(TreeNode* node, int depth);
void padding(char,int);
int search_name(TreeNode* node, const char *target);
int max_depth(TreeNode* node);
const char* first_name(TreeNode* node);
void mirror_tree(TreeNode* node);
TreeNode* trim_tree(TreeNode* node, const char* start_name, const char* end_name);
void destroy_tree(TreeNode* node);
int number_of_leaves(TreeNode* node);
int is_tree_balanced(TreeNode* node);
char** flatten_tree(TreeNode* node, int *len_strings);

/*
 * Stack Prototypes
 * */
void push_node(StackNode** top, TreeNode* t);
TreeNode* pop_node(StackNode** top);

int main (int argc, char *argv[]) {
	/*
	 * Check command line parameters
	 * */
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
	
	/*
	 * Query the databse (binary tree)
	 * */

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
	/*
	 * Display information about the database
	 * */

	printf("The number of records in the database is %d", size(root));
	printf("\nThe max depth of the database is %d\n", max_depth(root));
	printf("The number of leaf records are %d\n", number_of_leaves(root));
	printf("Is the database balanced (%s)\n", is_tree_balanced(root) ? "yes" : "no");
	printf("The tree mirrored looks like this\n\n"); 
	/*
	 * Change or flip databse
	 * */

	mirror_tree(root);
	display_tree(root,START_DEPTH);

	mirror_tree(root);
	char buffer2[MAX_NAME_LEN];

	printf("\n\nEnter the lower bound name and upper bound name: ");
	scanf("%s %s", buffer,buffer2);
	printf("Trimming the database from %s to %s\n\n", buffer, buffer2);
	//printf("\nMy trimming fucntio cannot trim correctly so i comment it out\n");
	root = trim_tree(root,buffer,buffer2);
	display_tree(root,START_DEPTH);
	
	
	int sizey = size(root);
	char** strings = flatten_tree(root,&sizey);
	int i = 0;
	printf("\nFlattened databse is:\n");

	/*
	 * Memory deallocations
	 * */
	for (i = 0; i < sizey; ++i) {
		printf("%s\n", strings[i]);
		free(strings[i]);
	}
	free(strings);
	destroy_tree(root);
	return 0;
}

/*it reads and inserts the data in the tree
 * input: the file 
 * return: the tree root.
 */
TreeNode* read_from_file(const char* file) {

	/* HINT
	 *	TreeNode *t = NULL;
	 *  t = insert(t,buffer);
	 * loop check the return type on fscanf
	 *  insert(t,buffer);
	 */

	FILE* filey = fopen(file,"r");

	if(!filey)
	{
		printf("Cannot open %s or does not exsists\n",filey);
		exit(1);
	}

	int size = 0;

	TreeNode *t = NULL;
	char name[MAX_NAME_LEN];
	fscanf(filey,"%s",name);

	t = insert(t,name);
	while(fscanf(filey,"%s",name) != EOF)
	{
		insert(t,name);
	}

	return t;

}

/*it insert the node by using recursion
 * input: the node and the name
 * return: the adress of the inserted node
 */ 
TreeNode* insert(TreeNode* node, const char *name) {

	if( node == NULL)
	{
		node = create_node(name);
	}
	else
	{
		if(strcmp(name,node->name) <= 0)
			node->left = insert(node->left,name);
		else
			node->right = insert(node->right,name);
	}

	return node;

}

/* it creates a new node in the tree
 * input: the name
 * return: the created node.
 */
TreeNode* create_node(const char *name) {
	
	TreeNode* newNode  = (TreeNode*) malloc(sizeof(TreeNode));

	if(newNode)
	{
		strcpy(newNode->name,name);
		newNode->right = NULL;
		newNode->left = NULL;
	}

	return newNode;
}

/*it sums all the nodes in the tree recusively
 * input: the root head
 * return: the sum
 */
int size(TreeNode* node) {

	if(node == NULL)
		return 0;
	else
		return( size(node->left) + 1 + size(node->right) );
	//because every right and left pointers should have a head 
	//so:
	/* 1- i calculare the head becuase if a left or right pointer would exsists then the head must
	 * exsists so == 1.
	 * then i move to each sub tree on the left and right 
	 * if and sub fail to exsists then it will return 0 buy recurstion
	 * and therefore it will be calculated in case of not balanced BST.
	 */
}

/*it searches for the target name
 * input: the root and the target
 * return: 0 if not found or 1 if found.
 */
int search_name(TreeNode* node, const char *target) {

	if(node == NULL)
		return 0;
	else
	{
		if(strcmp(node->name,target) == 0)
			return 1;

		else if(strcmp(target,node->name) < 0)
		   return search_name(node->left,target);
		
		else
			return search_name(node->right,target);
	}	
}

/*it finds the longes path in the tree
 * input: the node
 * return: the number of nodes on the longest path.
 */
int max_depth(TreeNode* node) {

	if(node == NULL)
		return 0;
	else
	{
		int theDepthR = max_depth(node->right);
		int theDepthL = max_depth(node->left);

		//then compare between each subtree on the left of the root and its right.

		if(theDepthR > theDepthL)
			return (theDepthR + 1);//one here is the root.
		else
			return (theDepthL + 1);
	}
}

/*it fips the left and right subtrees.
 * input the root
 * return: void.
 */
void mirror_tree(TreeNode* node) {

	if(node == NULL)
		return; //do nothing if u have an empty tree
	else
	{
		TreeNode* temp;

		//go the smallest and last sub tree in both the big subtrees
		mirror_tree(node->right);
		mirror_tree(node->left);

		//after finding it swap by saving in temp before assinging so u wont loose the adress
		temp = node->right;
		node->right = node->left;
		node->left = temp;
	}
}

/*it delets the nodes outside the range
 * input: the root and the min and upper bound
 * return: the new connection to the tree.
 */
TreeNode* trim_tree(TreeNode* node, const char* start_name, const char* end_name) 
{	

	if(node == NULL) //empty or i am at the bottom of the tree
		return NULL;

	//do recurdion to both sides of the root
	node->left = trim_tree(node->left,start_name,end_name);
	node->right = trim_tree(node->right,start_name,end_name);

	/*if(strcmp(node->name,start_name) > 0 && strcmp(node->name,end_name) < 0)
	{
		return node;
	}*/
	if(strcmp(node->name,start_name) < 0)
	{
			TreeNode* tempRight = node->right;
			free(node);
			return tempRight;
	}
	else if(strcmp(node->name,end_name) >  0)
	{
		TreeNode* tempLeft = node->left;
		free(node);
		return tempLeft;

	}
	return node;
}

/*it frees all the nodes in the tree
 * input: the free node
 * return: void.
 */
void destroy_tree(TreeNode* node) {
	
	if(node == NULL)
		return;

	destroy_tree(node->left);
	destroy_tree(node->right);

	free(node);	
}

/*it counts the number of nodes that have a left and right pointer
 * input: the root
 * return: the number of leaves.
 */
int number_of_leaves(TreeNode* node) {

	if(node == NULL)//empty tree
		return 0;
	
	else
	{
		if((node->left == NULL) && (node->right == NULL) )//check if it is by itself a leaf
			return 1;

		else
		{
			return ( number_of_leaves(node->right) + number_of_leaves(node->left) );
		}
	}
}

/*checks if the tree is balanced or not
 * input: the root
 * return: 1 if true , 0 if false
 */
int is_tree_balanced(TreeNode* node) {

	if(node == NULL)
		return 1;
	
	int tR = max_depth(node->right);
	int tL = max_depth(node->left);

	if( (abs(tR - tL) <=1) && is_tree_balanced(node->left) && is_tree_balanced(node->right) )
		return 1;

	//if i reached here then the condtions are all false
	return 0;
} 

char** flatten_tree(TreeNode* node, int *len_strings) {

	int len = *len_strings;
	StackNode* stack = NULL;
	TreeNode* nodey = node;
	int condition = 1;
	char **str = malloc(sizeof(char*)*len);

	int i = 0;
	
	if( str != NULL)
	{
		for( i = 0; i < len ;i++ )
		{

			str[i] = malloc(sizeof(char)*MAX_NAME_LEN);
			
			if(str[i] == NULL)
			{
				printf("\nCannot allocate memeory for the %d element in the array\n",i+1);
				exit(1);
			}

		}
	}
	else
		printf("\ncannot allcoate memory for the whole array\n");

	int j = 0;

	while(condition)
	{
		if(nodey != NULL)
		{
			push_node(&stack,nodey);
			nodey = nodey->left;
		}
		else
		{
			if(stack != NULL)
			{
				nodey = pop_node(&stack);
				strcpy(str[j],nodey->name);
				nodey = nodey->right;
		
				j++;
			}
			else
				condition = 0;
		}
	}

	return str;

	



}

/*it pushes onto the stack
 *input: the stack as refrence and the tree node you want to push
 *return: void
 */
void push_node(StackNode** top, TreeNode* t) {

	StackNode* stack = malloc(sizeof(StackNode));

	stack->t= t;
	stack->next = *top;

	*top = stack;
	
}

/*it pops the elements out of the stak
 * input: the stack as refrence
 * retur:the node you have poped.
 */
TreeNode* pop_node(StackNode** top_ref) {


	TreeNode* theReturn;	
	StackNode* top = *top_ref;


	theReturn = top->t;

	*top_ref = top->next;

	free(top);
	

	return theReturn;
}

/*
 * Used only for padding in the display tree, makes it look pretty
 * */
void padding (char ch, int n){

    	int i;
    	for (i = 0; i < n; i++)
        	printf("%c%c%c%c", ch, ch ,ch, ch);

}

/*
 * A Beautiful way to display various sorts of trees, passing this from one generation of students
 * to the next.
 * */
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
