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
// int split(string txt, const char *delim, string **tokens);
int split(string txt, char delim, char ***tokens);

int main(void)
{
    // string oldStr;
    // string *tokens;
    // int count;
    // char delim[] = " ";

    // testes da funcao split com parametros string txt, const char *delim, string **tokens
    /*

    // tokens = (string *)malloc(sizeof(string) * 10);
    // tokens[0].value = (char *)malloc(sizeof(char) * 10);
    oldStr.value = "gabriel madeira";
    oldStr.len = strlen(oldStr.value);

    count = split(oldStr, delim, &tokens);

    // printf("%d\n", count);
    // printf("%s\n", tokens[0].value);

    // for (int i = 0; i < count; i++)
    // {
    //     printf("len: %s\n", tokens[i].value);
    //     printf("value: %d\n", tokens[i].len);
    // }

    */

    // testes da funcao split com os parametros string txt, char *delim, char ***tokens

    string txt;
    char delim = ' ';
    char **tokens;
    int count, i;

    // txt = malloc(sizeof(string));
    txt.value = (char *)malloc(sizeof(char) * 20);
    strcpy(txt.value, "gabriel madeira");

    count = split(txt, delim, &tokens);

    printf("%d\n", count);

    for (i = 0; i < count; i++)
    {
        printf("len: %s\n", tokens[i]);
        printf("value: %ld\n", strlen(tokens[i]));
    }

    // freeing tokens
    for (i = 0; i < count; i++)
    {
        free(tokens[i]);
    }
    free(tokens);

    return 0;
}

int split(string txt, char delim, char ***tokens)
{
    int *tklen, *t, count = 1;
    char **arr, *p = txt.value;

    while (*p != '\0')
        if (*p++ == delim)
            count += 1;

    t = tklen = calloc(count, sizeof(int));

    for (p = (char *)txt.value; *p != '\0'; p++)
        *p == delim ? *t++ : (*t)++;

    *tokens = arr = malloc(count * sizeof(char *));
    t = tklen;
    p = *arr++ = calloc(*(t++) + 1, sizeof(char *));

    while (*txt.value != '\0')
    {
        if (*txt.value == delim)
        {
            p = *arr++ = calloc(*(t++) + 1, sizeof(char *));
            *txt.value++;
        }

        else
            *p++ = *txt.value++;
    }

    free(tklen);

    return count;
}

// int split(string txt, const char *delim, string **tokens)
// {
//     char tmpStr[50];
//     char *tmpToken;
//     string **tmpTk;
//     int i = 0, j = 0;

//     tokens = (string **)malloc(sizeof(string *) * 10);
//     tmpTk = (string **)malloc(sizeof(string *) * 10);

//     strcpy(tmpStr, txt.value);
//     tmpToken = strtok(tmpStr, delim);

//     while (tmpToken != NULL)
//     {
//         tmpTk[i] = (string *)malloc(sizeof(string));
//         tokens[i] = (string *)malloc(sizeof(string));
//         tmpTk[i]->value = tmpToken;
//         // strcpy(tokens[i]->value, tmpToken);
//         // strcpy(tmpTk[i]->value, tmpToken);
//         // tokens[i]->value = tmpToken;
//         tmpToken = strtok(NULL, delim);
//         i++;
//     }

//     char test[20] = "hello";

//     for (int j = 0; j < i; j++)
//     {
//         tokens[j]->len = strlen(tmpTk[j]->value);
//         strcpy(tokens[j]->value, test);
//     }

//     return i;
// }

// string removeFirstChar(string s)
// {
//     string newstr;
//     newstr.len = s.len - 1;
//     newstr.value = (char *)malloc(sizeof(char) * newstr.len);

//     for (int i = 0; i < s.len; i++)
//     {
//         newstr.value[i] = s.value[i + 1];
//     }

//     newstr.value[newstr.len] = '\0';

//     return newstr;
// }

// byte *intToBytes(int x, int fillFF)
// {
//     byte *array = malloc(sizeof(byte) * 4);
//     array[0] = (x >> 24) & 0xFF;
//     array[1] = (x >> 16) & 0xFF;
//     array[2] = (x >> 8) & 0xFF;
//     array[3] = x & 0xFF;

//     if (fillFF)
//     {
//         for (int i = 0; i < 4; i++)
//         {
//             if (array[i] == 0x00)
//                 array[i] = 0xFF;
//         }
//     }

//     return array;
// }
