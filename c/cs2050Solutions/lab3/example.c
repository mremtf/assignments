#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int d;
}Example;


int main () {
	Example *ex = malloc(sizeof(Example) * 10);
	Example **ex;
	int i = 0;
	for ( ;i < 10 ; ++i) {
		(ex + i)->d = i;
		printf("%d ", (ex + i)->d);
		printf("%d\n", (*(ex + i)).d);
		ex++;

	}

}
