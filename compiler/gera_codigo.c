
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
int sizeOfFile(FILE *f);
byte *bufferToMachineCode(string buffer);
void runMachineCode(byte *lbs, int size);
byte *generateFunctionHeader(int size);
byte *generateFunctionFooter();
string *brakeIntoLines(string buffer,int *size);
byte *interpretLine(string line, int *size);

int sizeOfFile(FILE *F) // Funcao que retorna o tamanho de um arquivo
{
    int size;
    fseek(f, 0, SEEK_END); // vai ate o final do arquivo
    size = ftell(f); //pega o tamanho do arquivo
    fseek(f, 0, SEEK_SET); // volta ao inicio do arquivo
    return size; //retorna o tamanho do arquivo
}

string generateBuffer(FILE *f)// Funcao que retorna um buffer para determinado arquivo
{
    int size = sizeOfFile(f);// pega o tamanho do arquivo
    char *buffer = (char *)malloc(size); //aloca o tamanho do arquivo
    fread(buffer, 1, size, f); // le o arquivo e coloca no buffer
    return buffer;
}
/*
    Funcao que pega um pedacoe de codigo de maquina e joga no final do buffer
*/
byte *pushMachineCode(byte *array, byte *code, int *sizeArray, int sizeCode) 
{
    for (int i = *sizeArray; i < *sizeArray + sizeCode; i++) //loop que copia o codigo de maquina para o buffer
    {
        array[i] = code[i - *sizeArray];
    }
    *sizeArray += sizeCode; // incrementa o tamanho do buffer
    return array;
}

string * brakeIntoLines(string buffer, int *size){ // Funcao que divide o buffer em um array de linhas
    
} 

byte *bufferToMachineCode(string buffer) //Funcao que transforma um buffer em codigo de maquina
{
    int maxSize = sizeof(buffer / sizeof(char)); // pega o tamanho do buffer
    byte *machineCode = (byte *)malloc(maxSize * 5); //aloca um tamanho para o buffer
    int *functions = malloc(sizeof(int)*5); //array com os tamanhos das funcoes
    int numberOfLines = 0;
    string *s = brakeIntoLines(buffer,&numberOfLines); // pega as linhas do buffer
    int trueSize = 0; // tamanho do codigo de maquina ate agora
    int i;
    
    for (i = 0; i < strlen(s); i++)
    {
        int codeSize;
        byte *code = interpretLine(s[i], &codeSize, functions);
        if(trueSize + codeSize >= maxSize){
            maxSize *= 2;
            machineCode = (byte *)realloc(machineCode, maxSize);
        }
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
