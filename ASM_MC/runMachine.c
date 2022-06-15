#include <stdio.h>
#include <stdlib.h>

typedef int (*funcp)(int x);

int main(void)
{
  unsigned char machineCode[] = {0x55,0x48,0x89,0xe5,0x48,0x89,0xfe,0x48,0x6b,0xf6,0x02,0x48,0x89,0xf0,0x5d,0xc3};
  funcp f = (funcp)machineCode;
  int i = f(1);
  printf("%d\n", i);
}

/*
function
v0 = p0 + $2
v0 = v0 * $3
ret v0

*/