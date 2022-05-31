#include <stdio.h>
#include <stdlib.h>

int return_constant(int a)
{
    return a;
}

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

int main(void)
{

    return 0;
}
