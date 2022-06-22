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
// string *breakInto(string buffer, int *size, char c); // TODO M
byte *littleThatEndian(byte *bytes, int fillFF); // TODO M
byte *intToBytes(int x, int fillFF);             // TODO M
string removeFirstChar(string s);
int split(string *txt, char delim, char ***tokens);

int main(void)
{

    string *str;
    str = malloc(sizeof(string));
    str->value = malloc(sizeof(char) * 10);
    str->value = "test func";
    char **tokens;
    int count, i;

    count = split(str, ' ', &tokens);

    for (i = 0; i < count; i++)
        printf("%s", tokens[i]);

    for (i = 0; i < count; i++)
        free(tokens[i]);
    free(tokens);

    return 0;
}

int split(string *txt, char delim, char ***tokens)
{
    int *tklen, *t, count = 1;
    char **arr, *p = txt->value;

    while (*p != '\0')
        if (*p++ == delim)
            count += 1;

    t = tklen = calloc(count, sizeof(int));

    for (p = (char *)txt->value; *p != '\0'; p++)
        *p == delim ? *t++ : (*t)++;

    *tokens = arr = malloc(count * sizeof(char *));
    t = tklen;
    p = *arr++ = calloc(*(t++) + 1, sizeof(char *));

    while (*txt->value != '\0')
    {
        if (*txt->value == delim)
        {
            p = *arr++ = calloc(*(t++) + 1, sizeof(char *));
            *txt->value++;
        }

        else
            *p++ = *txt->value++;
    }

    free(tklen);
    return count;
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
