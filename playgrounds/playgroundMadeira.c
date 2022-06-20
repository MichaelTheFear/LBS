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
string *brakeInto(string buffer, int *size, char c); // TODO M
byte *littleThatEndian(byte *bytes, int fillFF);     // TODO M
byte *intToBytes(int x, int fillFF);                 // TODO M
string removeFirstChar(string s);

int main(void)
{
    // byte *bytes;
    // int num = 5;

    // bytes = intToBytes(num, 1);

    // for (int i = 0; i < 4; i++)
    // {
    //     printf("%d ", bytes[i]);
    // }
    // printf("\n");

    string buffer = {5, "teste"};
    string newstr;

    newstr = removeFirstChar(buffer);
    printf("%s\n", newstr.value);

    return 0;
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

// string *brakeInto(string buffer, int *size, char c)
// {
//     string *array;
//     int i = 0, j = 0;

//     for (i = 0; i < strlen(buffer.len); i++)
//     {
//         if (buffer.value[i] == c)
//         {
//             j++;
//         }
//     }

//     array = (string *)malloc(sizeof(string) * j);
//     *size = j;

//     j = 0;
//     for (i = 0; i < strlen(buffer); i++)
//     {
//         if (buffer[i] == c)
//         {
//             array[j] = (string)malloc(sizeof(char) * i);
//             strncpy(array[j], buffer, i);
//             j++;

//             buffer += i + 1;
//             i = -1;
//         }
//     }

// return array;
// }

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
