#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char byte;

void freeMatrix(char **c)
{
    int i;
    for (i = 0; c[i] != NULL; i++)
    {
        free(c[i]);
    }

    free(c);
}

int split(char *txt, char delim, char ***tokens)
{
    int *tklen, *t, count = 1;
    char **arr, *p = (char *)txt;

    while (*p != '\0')
        if (*p++ == delim)
            count += 1;
    t = tklen = calloc(count, sizeof(int));
    for (p = (char *)txt; *p != '\0'; p++)
        *p == delim ? *t++ : (*t)++;
    *tokens = arr = malloc(count * sizeof(char *));
    t = tklen;
    p = *arr++ = calloc(*(t++) + 1, sizeof(char *));
    while (*txt != '\0')
    {
        if (*txt == delim)
        {
            p = *arr++ = calloc(*(t++) + 1, sizeof(char *));
            txt++;
        }
        else
            *p++ = *txt++;
    }
    free(tklen);
    return count;
}

int main(int argc, char **argv)
{
    char **tokens;
    int count, i;
    char *str = "v0 = v2 + $1 \nv1 = v3 + $3\n";

    count = split(str, ' ', &tokens);
    for (i = 0; i < count; i++)
        printf("%s\n", tokens[i]);

    /* freeing tokens */
    for (i = 0; i < count; i++)
        free(tokens[i]);
    free(tokens);
    return 0;
}

//