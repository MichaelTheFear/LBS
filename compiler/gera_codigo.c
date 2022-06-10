/*Eduardo Dantas Luna 2111484 3WA*/
/*Gabriel de Oliveira Rosa Mariano Madeira 2111471 3WA*/

#include <stdlib.h>
#include <stdio.h>
#include <sting.h>
#define MAX_MACHINE_CODE_SIZE 100

typedef int (*funcp)(int x);
typedef string char *;
typedef byte unsigned char;

void gera_codigo(FILE *f, unsigned char code[], funcp *entry);
string generateBuffer(FILE *f);
int sizeofFile(FILE *f);
byte *bufferToMachineCode(string buffer);
void runMachineCode(byte *lbs, int size);
byte *generateFunctionHeader(int size);
byte *generateFunctionFooter();
string *brakeIntoLines(string buffer);
byte *interpretLine(string line, int *size);

int sizeOfFile(FILE *F)
{
    int size;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

string generateBuffer(FILE *f)
{
    int size = sizeofFile(f);
    char *buffer = (char *)malloc(size);
    fread(buffer, 1, size, f);
    return buffer;
}

byte *pushMachineCode(byte *array, byte *code, int *sizeArray, int sizeCode)
{
    for (int i = *sizeArray; i < *sizeArray + sizeCode; i++)
    {
        array[i] = code[i - *sizeArray];
    }
    *sizeArray += sizeCode;
    return array;
}

byte *bufferToMachineCode(string buffer)
{
    int maxSize = sizeof(buffer / sizeof(char));
    byte *machineCode = (byte *)malloc(maxSize * MAX_MACHINE_CODE_SIZE);
    string *s = brakeIntoLines(buffer);
    int trueSize = 0;
    int i;
    int lastFunction = 0;
    for (i = 0; i < strlen(s); i++)
    {
        int codeSize;
        byte *code = interpretLine(s[i], &codeSize, &lastFunction);
        machineCode = pushMachineCode(machineCode, code, trueSize, codeSize);
        // free(code);
    }
    return machineCode;
}

string *brakeWords(string *s)
{
    string *words = (string *)malloc(sizeof(string) * strlen(s));
    int i = 0;
    int j = 0;
    while (i < strlen(s))
    {
        if (s[i] == ' ')
        {
            words[j] = (string)malloc(sizeof(char) * i);
            strncpy(words[j], s, i);
            j++;
            i++;
        }
        else
        {
            i++;
        }
    }
    return words;
}

byte *interpretLine(string line, int *size, int *lastFunction)
{
    string *words = brakeWords(line);
    char *firstChar = line;
    switch (firstChar[0])
    {
    case 'f':
        // function
        byte *res = {};
        brake;
    case 'v':
        // assignment
        brake;
    case 'r':
        // return
        brake;
    case 'z':
        // return zero
        brake;
    case 'e':
        // end
        brake;
    case 'c':
        // call
        brake;
    }
}

/*
LBS table:


*/