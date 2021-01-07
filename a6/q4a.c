#include <stdlib.h>
#include <stdio.h>

int arr[] = {0,0,0,0,0,0,0,0,0,0};      // 0x1000
int* ptr = arr;                      // 0x100

void foo(int a, int b) {
	ptr[b] += a;
}

int main() {
	int x = 1;
	int y = 2;
	foo(3, 4);
	foo(x, y);
	
	for(int i = 0; i < 10; i++) 
		printf("%d\n", ptr[i]);
	
	return 0;
}