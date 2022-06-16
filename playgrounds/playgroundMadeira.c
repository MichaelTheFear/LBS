#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*funcp)(int x);
typedef char *string;
typedef unsigned char byte;

// funcoes auxiliares
string *brakeInto(string buffer, int *size, char c); // TODO M
byte *littleThatEndian(byte *bytes, int fillFF);     // TODO M
byte *intToBytes(int x, int fillFF);                 // TODO M

int main(void)
{
    // using breakinto function
    int arr_size;
    string *array;
    array = brakeInto("madeiraclunacgabriel", &arr_size, 'c');
    printf("%d\n", arr_size);

    for (int i = 0; i < arr_size; i++)
    {
        printf("%s\n", array[i]);
    }

    return 0;
}

/*
given a string, return an array of string,
using the given character as separator
the size of the array should be returned in size
*/
string *brakeInto(string buffer, int *size, char c)
{
    string *array;
    int i = 0, j = 0;

    for (i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == c)
        {
            j++;
        }
    }

    array = (string *)malloc(sizeof(string) * j);
    *size = j;

    j = 0;
    for (i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == c)
        {
            array[j] = (string)malloc(sizeof(char) * i);
            strncpy(array[j], buffer, i);
            j++;

            buffer += i + 1;
            i = -1;
        }
    }

    return array;
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
// byte *intToBytes(int x, int fillFF)
// {
//     byte *array = malloc(sizeof(byte) * 4);
//     array[0] = (x >> 24) & 0xFF;
//     array[1] = (x >> 16) & 0xFF;
//     array[2] = (x >> 8) & 0xFF;
//     array[3] = x & 0xFF;

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
