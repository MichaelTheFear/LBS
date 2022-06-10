
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
string *brakeInto(string buffer,int *size,char c);
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

string * brakeInto(string buffer, int *size,char c){ // Funcao que divide o buffer em um array de linhas
    string * lines = malloc(sizeof(string) * 30); // aloca um array de string com 30 posicoes
    int lastLineOn = 0 ; // variavel que guarda o ultimo indice da linha
    for(int i = 0;i<strlen(buffer);i++){ // loop que percorre o buffer
        if(buffer[i] == c){ // se encontrar o marcador de fim da string
            if(sizeof(lines) / sizeof(string) >= (*size)-1){ //se o array de linhas estiver cheio
                lines = realloc(lines, (sizeof(lines) / sizeof(string))* 2); // aloca um array de string com dobro de tamanho
            }
            lines[*size] = malloc(sizeof(char) * (i-lastLineOn)); // aloca um array de char com o tamanho da linha
            strncpy(lines[*size],buffer+lastLineOn,i-lastLineOn); // copia a linha para o array de linhas
            lastLineOn = i+1; // atualiza o ultimo indice da linha
            (*size)++; // incrementa o tamanho do array de linhas
        }
    }
    return lines;
} 

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
        if(trueSize + codeSize >= maxSize){ // se o codigo de maquina for maior que o tamanho do buffer
            maxSize *= 2;
            machineCode = (byte *)realloc(machineCode, maxSize); // aloca um tamanho do buffer dobrado
        }
        machineCode = pushMachineCode(machineCode, code, trueSize, codeSize); // coloca o codigo de maquina no buffer
        // free(code);
    }
    free(s); //libera o array de linhas
    return machineCode;
}
