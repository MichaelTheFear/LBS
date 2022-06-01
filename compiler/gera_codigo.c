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
byte *interpretLine(string line);

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

byte *bufferToMachineCode(string buffer)
{
    int size = sizeof(buffer / sizeof(char));
    byte *machineCode = (byte *)malloc(size * MAX_MACHINE_CODE_SIZE);
    string *s = brakeIntoLines(buffer);
    int i;

    return machineCode;
}

/*
LBS table:


*/