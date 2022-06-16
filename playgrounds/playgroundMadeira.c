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
    int x = 0x12345678;
    byte *bytes = intToBytes(x, 1);
    printf("%d\n", bytes[0]);
    printf("%d\n", bytes[1]);
    printf("%d\n", bytes[2]);
    printf("%d\n", bytes[3]);
    free(bytes);

    printf("littleThatEndian:\n");
    int y = 0x12345678;
    byte *bytes2 = littleThatEndian((byte *)&y, 0);
    printf("%d\n", bytes2[0]);
    printf("%d\n", bytes2[1]);
    printf("%d\n", bytes2[2]);
    printf("%d\n", bytes2[3]);
    free(bytes2);

    return 0;
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

/*
given a string, return an array of string,
using the given character as separator
the size of the array should be returned in size
*/
string *brakeInto(string buffer, int *size, char c)
{
    int i, j, k;
    string *array;
    int len = strlen(buffer);
    int count = 0;

    // esse for loop é só pra pegar o tamanho do array
    // somando mais 1 porque cada separador divide duas palavras
    // e no final sempre faltaria uma palavra se n somar esse 1
    // eu acho
    for (i = 0; i < len; i++)
    {
        if (buffer[i] == c)
            count++;
    }
    count++;

    array = (string *)malloc(sizeof(string) * count);

    for (i = 0, j = 0, k = 0; i < len; i++)
    {
        if (buffer[i] == c)
        {
            array[j] = (string)malloc(sizeof(char) * (k + 1));
            strncpy(array[j], buffer + k, k);
            array[j][k] = '\0';
            j++;
            k = i + 1;
        }
    }

    array[j] = (string)malloc(sizeof(char) * (k + 1));
    strncpy(array[j], buffer + k, k);
    array[j][k] = '\0';
    *size = count;

    return array;
}