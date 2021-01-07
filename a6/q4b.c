#include <stdio.h>
#include <stdlib.h>

int x[8] = {1, 2, 3, -1, -2, 0, 184, 340057058};
int y[8] = {0,0,0,0,0,0,0,0};


int f(int j) {
    int t = 0;
    while (j != 0)
    {
        if ((j & 0x80000000) != 0)
        {
            t++;
        }
        j = j << 1;
    }
    return t;
}

int main() {
    int i = 8;
     while (i != 0) {
         i--;
         y[i] = f(x[i]);
     }

     for(int i = 0; i < 8; i++) 
		printf("%d\n", x[i]);

    for(int i = 0; i < 8; i++) 
		printf("%d\n", y[i]);

    return 0;
}