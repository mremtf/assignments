#include<stdio.h>
#include<time.h>

int main() {
	clock_t begin = clock();
	int i= 0;
	for (; i < 10000000; ++i) {
		;
	}
	clock_t end = clock();

	printf("clock %ld\n %ld %d\n", begin, end, CLK_TCK);
}
