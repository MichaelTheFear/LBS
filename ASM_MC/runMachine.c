#include <stdio.h>
#include <stdlib.h>

typedef int (*funcp)(int x);

int main(void)
{
    unsigned char machineCode[] = {0x55,
     0x48, 0x89, 0xe5, 0x41, 0xb9, 0x0a,
      0x00, 0x00, 0x00, 0x44, 0x89, 0xc8, 0x5d, 0xc3};
    funcp f = (funcp)machineCode;
    int i = f(4);
    printf("%d\n", i);
}