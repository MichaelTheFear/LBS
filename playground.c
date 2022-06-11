#include <stdio.h>

int function1(int x)
{
    return x + 1;
}

int function2(int y){
    return y + 2;
}

int main(void){
    int i;
    i = function1(2);
    printf("%d\n", i);
}