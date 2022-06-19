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
string removeFirstChar(string s);

int main(void)
{
    // using littleThatEndian function
    // byte *bytes = littleThatEndian((byte *)&x, 1);
    // printf("%d\n", bytes[0]);
    // printf("%d\n", bytes[1]);
    // printf("%d\n", bytes[2]);
    // printf("%d\n", bytes[3]);

    string a = removeFirstChar("teste");
    printf("%s\n", a);

    return 0;
}

string removeFirstChar(string s)
{
    string new_str;
    new_str = (string)malloc(sizeof(char) * (strlen(s) - 1));
    int i;

    for (i = 0; i < strlen(s); i++)
    {
        new_str[i] = s[i + 1];
    }

    new_str[i] = '\0';

    return new_str;
}

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
byte *littleThatEndian(byte *bytes, int fillFF)
{
    byte *array = malloc(sizeof(byte) * 4);
    array[0] = bytes[3];
    array[1] = bytes[2];
    array[2] = bytes[1];
    array[3] = bytes[0];
    if (fillFF)
    {
        array[0] = 0xFF;
        array[1] = 0xFF;
        array[2] = 0xFF;
        array[3] = 0xFF;
    }

    else
    {
        array[0] = 0x00;
        array[1] = 0x00;
        array[2] = 0x00;
        array[3] = 0x00;
    }

    return array;
}

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
        array[0] = 0xFF;
        array[1] = 0xFF;
        array[2] = 0xFF;
        array[3] = 0xFF;
    }

    else
    {
        array[0] = 0x00;
        array[1] = 0x00;
        array[2] = 0x00;
        array[3] = 0x00;
    }

    return array;
}
