#include <stdio.h>
#include <stdlib.h>

int return_constant(int a)
{
    return a;
}

/*
    return constant assembly and machine code
   0:	f3 0f 1e fa          	endbr64
   4:	55                   	push   %rbp
   5:	48 89 e5             	mov    %rsp,%rbp
   8:	89 7d fc             	mov    %edi,-0x4(%rbp)
   b:	8b 45 fc             	mov    -0x4(%rbp),%eax
   e:	5d                   	pop    %rbp
   f:	c3                   	ret
*/

int inc_constant(int a)
{
    return a + 1;
}

int calculate_factorial(int a)
{
    if (a == 0)
        return 1;
    else
        return a * calculate_factorial(a - 1);
}

int number_square(int a)
{
    return a * a;
}

int sum_square(int n)
{
    int sum = 0;

    for (int i = 1; i <= n; i++)
    {
        sum += number_square(i);
    }

    return sum;
}
