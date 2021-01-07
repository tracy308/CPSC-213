#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct string {
    int size;
    char* chars;
} string;

string str1 = {30, "Welcome! Please enter a name:\n"};
string str2 = {11, "Good luck, "};
string str3 = {43, "The secret phrase is \"squeamish ossigrade\"\n"};

int main() {
    char *buf[128];
    print(str1);
    int inputSize = read(0, buf, 256);
    print(str2);
    write(1, buf, inputSize);
}

void print(struct string str) {
    write(1, str.chars, str.size);
}

void proof() {
    print(str3);
}