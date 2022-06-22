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

    // loop through the buffer and break it into words
    for (i = 0, j = 0; i < buffer.len; i++)
    {
        if (buffer.value[i] == c)
        {
            new_str[j].len = len;
            new_str[j].value = (char *)malloc(len * sizeof(char));

            for (k = 0; k < len; k++)
            {
                new_str[j].value[k] = buffer.value[k + i - len];
            }
            len = 0;
            j++;
        }

        else
        {
            len++;
        }

        if (i == buffer.len - 1)
        {
            new_str[j].len = len;
            new_str[j].value = (char *)malloc(len * sizeof(char));

            for (k = 0; k < len; k++)
            {
                new_str[j].value[k] = buffer.value[k + i - len];
            }

            len = 0;
            j++;
        }
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

/*
Dado bytes, retorna um array de 4 bytes, em little Endian
(basicamente o inverso do array).
Se fillFF for 1, o resto do array é preenchido com 0xFF.
Caso o contrario com 0x00.
*/
// byte *littleThatEndian(byte *bytes, int fillFF)
// {
//     byte *array = malloc(sizeof(byte) * 4);
//     array[0] = bytes[3];
//     array[1] = bytes[2];
//     array[2] = bytes[1];
//     array[3] = bytes[0];
//     if (fillFF)
//     {
//         array[0] = 0xFF;
//         array[1] = 0xFF;
//         array[2] = 0xFF;
//         array[3] = 0xFF;
//     }

//     else
//     {
//         array[0] = 0x00;
//         array[1] = 0x00;
//         array[2] = 0x00;
//         array[3] = 0x00;
//     }

//     return array;
// }

/*
Dados um inteiro, retorna um array de 4 bytes, em little Endian
se fill FF for 1 o resto do array é preenchido com 0xFF.
Caso contrario com 0x00.
*/
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
