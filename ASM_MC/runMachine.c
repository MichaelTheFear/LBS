#include <stdio.h>
#include <stdlib.h>

typedef int (*funcp)(int x);

int main(void)
{
  unsigned char machineCode[] = 
???
;
  funcp f = (funcp)machineCode;
  int i = f(1);
  printf("%d\n", i);
}

