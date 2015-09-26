#include<stdio.h>

int main(void)
{
   int number = 10;
   int next_number = 12;
   float new_number;

   printf("%d\n", next_number); 
   printf("%d\n", number);
   printf("%d\n", number + next_number);

   printf("Enter a new number: ");
   scanf("%d", &new_number);
  
   int a[10];
   int i;
   for (i = 0; i < 12; ++i) {
   	a[i] = i;
   }
   for (i = 0; i < 10; ++i) {
   	printf("%d\n", a[i]);
	if (i == 5) {
		printf("hey it's a four\n");
	}
   }
   printf("Number Entered was %d\n", new_number);
  
}
