#include<stdio.h>
#include<stdlib.h>


int main () {
	char *s = malloc(20 * sizeof(char));
	char s2[20];

	int i;
	for (i = 0; i < 20; ++i) {
		s[i]  = s2[i];
		*(s + i) = *(s2 + i);
	}

	int **xxx;
	int x = 4, y = 7, z = 2, *ip1, *ip2;

	ip1 = &x;
	ip2 = &z;

	xxx = &ip2;
	printf("%d\n", **xxx);
	xxx = &ip1;
	printf("%d\n", **xxx);
	printf("%p\n", ip1);
	*ip1 = *ip2;
	printf("%d\n", x);

	return 0;
}
