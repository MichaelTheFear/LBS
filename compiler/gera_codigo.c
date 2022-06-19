
/*Eduardo Dantas Luna 2111484 3WA*/
/*Gabriel de Oliveira Rosa Mariano Madeira 2111471 3WA*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_MACHINE_CODE_SIZE 100

typedef int (*funcp)(int x);
typedef struct __string{
  int len; //tamanho da string
  char *value; //a string em si
}string; 
typedef unsigned char byte;
typedef struct __var{
    int value;/* se for uma variavel, este numero correspondera ao indice
     da variavel, caso seja uma constante este numero corresponde o seu valor*/
    int isVar; // diz se eh uma variavel ou se eh um valor constante
}var; 

//funcoes de interpretacao de codigo de maquina
var* parseLineToVar(string *pieces,char c);
void gera_codigo(FILE *f, unsigned char code[], funcp *entry);
unsigned char * convertionWrapper(funcp *entry);
byte *fixCallIndexes(byte *code, int * callsIndexes,int* functionIndexes);


// funcaoes de buffer
string generateBuffer(FILE *f);
int sizeOfFile(FILE *f);
void freeBuffer(string buffer);

// funcoes de geracao de codigo de maquina
byte *generateEnd();                      
byte *generateFunction();                    
byte *generateCall(int distance, int upOrDown); 
byte *generateOperation(char operationType, var v1, var v2);   
byte *generateZret(var v,int distanceToLabel);         
byte *generateSum(var v1, var v2); 
byte *generateSub(var v1, var v2);
byte *generateMul(var v1,var v2);
byte *generateReturn(var v);                            
byte *generateAssigment(int valueToAssign, char operation);



// funcoes auxiliares
string *brakeInto(string buffer, int *size, char c); 
byte *stackPosition(int varNum);
byte *littleThatEndian(byte *bytes, int fillFF);     
byte *intToBytes(int x, int fillFF);                
void *doubleSize(void *ptr, int *size, int condition);

void gera_codigo(FILE *f, unsigned char code[], funcp *entry){
    int bufferSize = sizeOfFile(f);
    string buffer = generateBuffer(f);
    code = (unsigned char*) convertionWrapper(entry);
    freeBuffer(buffer);
}













/*
Dado uma string, retorna um array de strings,
usando o caracter c como separador.
O tamanho do array é retornado em size.
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
    Dobra o tamanho de um array
*/
void *doubleSize(void *ptr, int *size, int condition)
{
    if (condition)
    {
        *size *= 2;
        ptr = realloc(ptr, *size);
    }
    return ptr;
}


int sizeOfFile(FILE *f) // Funcao que retorna o tamanho de um arquivo
{
    int size;
    fseek(f, 0, SEEK_END); // vai ate o final do arquivo
    size = ftell(f);       // pega o tamanho do arquivo
    fseek(f, 0, SEEK_SET); // volta ao inicio do arquivo
    return size;           // retorna o tamanho do arquivo
}

string generateBuffer(FILE *f,int * size) // Funcao que retorna um buffer para determinado arquivo
{
    *size = sizeOfFile(f);            // pega o tamanho do arquivo
    char *buffer = (char *)malloc(*size); // aloca o tamanho do arquivo
    fread(buffer, 1, size, f);           // le o arquivo e coloca no buffer
    return buffer;
}
/*
    Funcao que pega um pedacoe de codigo de maquina e joga no final do buffer
*/
byte *pushMachineCode(byte *array, byte *code, int *sizeArray, int sizeCode)
{
    for (int i = *sizeArray; i < *sizeArray + sizeCode; i++) // loop que copia o codigo de maquina para o buffer
    {
        array[i] = code[i - *sizeArray];
    }
    *sizeArray += sizeCode; // incrementa o tamanho do buffer
    return array;
}

/*

byte *bufferToMachineCode(string buffer) //Funcao que transforma um buffer em codigo de maquina
{
    int maxSize = sizeof(buffer / sizeof(char)); // pega o tamanho do buffer
    byte *machineCode = (byte *)malloc(maxSize * 5); //aloca um tamanho para o buffer
    int *functions = malloc(sizeof(int)*5); //array com os tamanhos das funcoes
    int numberOfLines = 0; // variavel que guarda o numero de linhas do buffer
    string *s = brakeInto(buffer,&numberOfLines,'\n'); // pega as linhas do buffer
    int trueSize = 0; // tamanho do codigo de maquina ate agora
    int i;

    for (i = 0; i < numberOfLines; i++) // loop que percorre as linhas do buffer
    {
        int codeSize;
        byte *code = interpretLine(s[i], &codeSize, functions); // pega o codigo de maquina da linha
        doubleSize(machineCode,&maxSize,trueSize + codeSize >= maxSize);
        machineCode = pushMachineCode(machineCode, code, trueSize, codeSize); // coloca o codigo de maquina no buffer
        // free(code);
    }
    free(s); //libera o array de linhas
    return machineCode;
}
*/