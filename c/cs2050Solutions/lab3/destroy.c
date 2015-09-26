#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void foo( char * ptr)
{
   ptr = malloc(255); // allocate some memory
   strcpy( ptr, "Hello World");
}

int main()
{
   char *ptr = 0;
   // call function with a pointer to pointer
   foo( ptr );
   printf("%s\n", ptr);
   // free up the memory
  free(ptr);

   return 0;
}

