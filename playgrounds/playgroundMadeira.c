#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*funcp)(int x);
typedef struct __string
{
    int len;     // tamanho da string
    char *value; // a string em si
} string;
typedef unsigned char byte;

// funcoes auxiliares
string *breakInto(string buffer, int *size, char c); // TODO M
byte *littleThatEndian(byte *bytes, int fillFF);     // TODO M
byte *intToBytes(int x, int fillFF);                 // TODO M
string removeFirstChar(string s);

int main(void)
{
    // test the breakInto function using a string with two words separated by the char c
    string buffer = {.len = 10, .value = "gabrielcmadeira"};
    int size;
    string *words = breakInto(buffer, &size, 'c');
    printf("%s\n", words[0].value);
    printf("%s\n", words[1].value);
    printf("%d\n", size);

    return 0;
}

string *breakInto(string buffer, int *size, char c)
{
    string *new_str; // array of strings
    int i, j, k;
    int len = 0;
    int count = 0;

    // loop through the buffer and count the number of times c appears
    for (i = 0; i < buffer.len; i++)
    {
        if (buffer.value[i] == c)
        {
            count++;
        }
    }

    new_str = (string *)malloc(count * sizeof(string));

    for (i = 0, j = 0, k = 0; i < buffer.len; i++)
    {
        if (buffer.value[i] == c)
        {
            new_str[j].len = k;
            new_str[j].value = (char *)malloc(k * sizeof(char));
            memcpy(new_str[j].value, &buffer.value[i - k], k);
            k = 0;
            j++;
        }
        else
        {
            k++;
        }

        if (i == buffer.len - 1)
        {
            new_str[j].len = k;
            new_str[j].value = (char *)malloc(k * sizeof(char));
            memcpy(new_str[j].value, &buffer.value[i - k + 1], k);
        }

        len += k;
    }

    *size = count;

    return new_str;
}
string removeFirstChar(string s)
{
    string newstr;
    newstr.len = s.len - 1;
    newstr.value = (char *)malloc(sizeof(char) * newstr.len);

    for (int i = 0; i < s.len; i++)
    {
        newstr.value[i] = s.value[i + 1];
    }

    newstr.value[newstr.len] = '\0';

    return newstr;
}

byte *intToBytes(int x, int fillFF)
{
    byte *array = malloc(sizeof(byte) * 4);
    array[0] = (x >> 24) & 0xFF;
    array[1] = (x >> 16) & 0xFF;
    array[2] = (x >> 8) & 0xFF;
    array[3] = x & 0xFF;

    if (fillFF)
    {
        for (int i = 0; i < 4; i++)
        {
            if (array[i] == 0x00)
                array[i] = 0xFF;
        }
    }

    return array;
}
