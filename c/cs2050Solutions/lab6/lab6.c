#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct MatrixNode_ {
	int value;
	struct MatrixNode_ *next_row;
	struct MatrixNode_ *next_col;
}MatrixNode;

void create_linked_list_matrix(MatrixNode**,const int rows, const int cols);
MatrixNode* create_node();
void display_matrix(MatrixNode*);
void destroy_linked_list_matrix(MatrixNode **); 
void add_two_matrices_together(MatrixNode* head_a, MatrixNode* head_b, MatrixNode* result, const int rows, const int cols); 
int find_total_value_of_the_matrix(MatrixNode* head);
void write_result_to_file(MatrixNode* results);

int main (int argc, char *argv[]) {
	if (argc < 3) {
		exit(1);
	}
	srand(time(NULL));
	const int rows = atoi(argv[1]);
	const int cols = atoi(argv[2]);
	MatrixNode* head_a = NULL;
	MatrixNode* head_b = NULL;
	MatrixNode* result = NULL;
	
	printf("Matrix A\n");
	create_linked_list_matrix(&head_a,rows,cols);
	display_matrix(head_a);
	printf("Total value of Matrix A is %d\n",find_total_value_of_the_matrix(head_a));
	printf("Matrix B\n");
	create_linked_list_matrix(&head_b,rows,cols);
	display_matrix(head_b);
	printf("Total value of Matrix B is %d\n",find_total_value_of_the_matrix(head_b));

	create_linked_list_matrix(&result,rows,cols);

	add_two_matrices_together(head_a,head_b,result,rows, cols);
	write_result_to_file(result);

	destroy_linked_list_matrix(&head_a); 
	destroy_linked_list_matrix(&head_b);
	destroy_linked_list_matrix(&result);

}

void create_linked_list_matrix(MatrixNode** head, const int rows, const int cols) {
	int row = 1, col = 1;
	*head = create_node();
	MatrixNode* current_row = *head;
	MatrixNode* current_col;
	//Creates the rows
	for ( ; row < rows; ++row) {
		current_row->next_row = create_node();
		current_row = current_row->next_row;
	}
	//creates the columns
	current_row = *head;
	for (;current_row != NULL; current_row = current_row->next_row) {
		current_col  = current_row;
		for (col = 1; col < cols; ++col) {
			current_col->next_col = create_node();
			current_col = current_col->next_col;
		}
	}
}

void display_matrix(MatrixNode* head) {

	MatrixNode* current_row = head;
	MatrixNode* current_col;

	for (;current_row != NULL; current_row = current_row->next_row) {
		printf("%-3d ", current_row->value);
		for (current_col  = current_row->next_col; current_col != NULL; current_col = current_col->next_col){
			printf(" %-3d ", current_col->value);
		}
		printf("\n");
	}
}

MatrixNode * create_node() {
	MatrixNode* new = malloc(sizeof(MatrixNode));
	new->value = rand() % 100 + 1;
	new->next_row = NULL;
	new->next_col = NULL;
	return new;
}

void destroy_linked_list_matrix(MatrixNode **head) {
	MatrixNode* current_row = *head;
	MatrixNode* current_col;
	MatrixNode* destroy;

	while(current_row != NULL) {
		current_col  = current_row->next_col;
		while(current_col != NULL){
			destroy = current_col;
			current_col = current_col->next_col;
			free(destroy);
		}
		destroy = current_row;
		current_row = current_row->next_row;
		free(destroy);
	}
}

int find_total_value_of_the_matrix(MatrixNode* head) {
	MatrixNode* current_row = head;
	MatrixNode* current_col;
	int total = 0;
	for (;current_row != NULL; current_row = current_row->next_row) {
		total += current_row->value;
		for (current_col  = current_row->next_col; current_col != NULL; current_col = current_col->next_col){
			total += current_col->value;
		}
		
	}
	return total;
}

void add_two_matrices_together(MatrixNode* head_a, MatrixNode* head_b, MatrixNode* result, const int rows, const int cols) {
	MatrixNode* current_row_a = head_a;
	MatrixNode* current_col_a;

	MatrixNode* current_row_b = head_b;
	MatrixNode* current_col_b;

	MatrixNode* current_row_c = result;
	MatrixNode* current_col_c;

	while(current_row_a != NULL) {
		current_row_c->value = current_row_a->value + current_row_b->value;

		current_col_a  = current_row_a->next_col;
		current_col_b  = current_row_b->next_col;
		current_col_c  = current_row_c->next_col;

		while ( current_col_a != NULL ){
			current_col_c->value = current_col_a->value + current_col_b->value;

			current_col_a = current_col_a->next_col;
			current_col_b = current_col_b->next_col;
			current_col_c = current_col_c->next_col;
		}

		current_row_a = current_row_a->next_row;
		current_row_b = current_row_b->next_row;
		current_row_c = current_row_c->next_row;
	}
}

void write_result_to_file(MatrixNode* head) {

	MatrixNode* current_row = head;
	MatrixNode* current_col;
	FILE* fp = fopen("result.txt", "w");
	for (;current_row != NULL; current_row = current_row->next_row) {
		fprintf(fp,"%-3d ", current_row->value);
		for (current_col  = current_row->next_col; current_col != NULL; current_col = current_col->next_col){
			fprintf(fp," %-3d ", current_col->value);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);

}

