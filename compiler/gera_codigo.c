
/*Eduardo Dantas Luna 2111484 3WA*/
/*Gabriel de Oliveira Rosa Mariano Madeira 2111471 3WA*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int (*funcp)(int x);
typedef struct __string
{
    int len;     // tamanho da string
    char *value; // a string em si
} string;
typedef unsigned char byte;
typedef struct __var
{
    int value; /* se for uma variavel, este numero correspondera ao indice
      da variavel, caso seja uma constante este numero corresponde o seu valor,
      caso seja uma operacao seguir a tabela a seguir
       + -> 1
       - -> 2
       * -> 3
       call -> 4
    caso seja p0 value = -1
      */
    int isVar;
    /*
        0 - constante
        1 - variavel ou argumento da funcao
        2 - indice de funcao
        -1 - operacao
    */
} var;

// funcoes de interpretacao de codigo de maquina
var *parseLineToVar(string line, int *size);
void gera_codigo(FILE *f, unsigned char code[], funcp *entry);
unsigned char *convertionWrapper(string buffer, funcp *entry);
byte *fixCallIndexes(byte *code, int *callsIndexes, int *functionIndexes);

// funcaoes de buffer
string generateBuffer(FILE *f);
int sizeOfFile(FILE *f);
void freeBuffer(string buffer);

// funcoes de geracao de codigo de maquina
byte *generateEnd(byte *code, int *currentSize, int *maxSize);
byte *generateFunction(byte *code, int *currentSize, int *maxSize);
byte *generateCall(byte *code, int *currentSize, int *maxSize, int index, int *newCaller);
byte *generateAssigmentOneToOne(byte *code, int *currentSize, int *maxSize, var v);
byte *generateOperation(byte *code, int *currentSize, int *maxSize,
                        var v1, var op, var v2);
byte *generateZret(byte *code, int *currentSize, int *maxSize, var v, int distanceToLabel);
byte *generateSum(byte *code, int *currentSize, int *maxSize, var v1);
byte *generateSub(byte *code, int *currentSize, int *maxSize, var v1);
byte *generateMul(byte *code, int *currentSize, int *maxSize, var v1);
byte *generateReturn(byte *code, int *currentSize, int *maxSize, var v);
byte *generateAssigment(byte *code, int *currentSize, int *maxSize,
                        int varNum);
byte *generateCaller(byte *code, int *currentSize, int *maxSize);

// funcoes auxiliares
string *brakeInto(string buffer, int *size, char c);
byte *stackPosition(int varNum);
byte *littleThatEndian(byte *bytes, int fillFF);
byte *intToBytes(int x, int fillFF);
void *doubleSize(void *ptr, int *size, int condition);
byte varInMC(int varNum);
byte *varOrConstBytes(var v, int *size);
byte *varToR12(int varNum);
string removeFirstChar(string s);

void gera_codigo(FILE *f, unsigned char code[], funcp *entry)
{
    string buffer = generateBuffer(f);
    code = (unsigned char *)convertionWrapper(buffer, entry);
    freeBuffer(buffer);
}

unsigned char *convertionWrapper(string buffer, funcp *entry)
{
    int i;                                                 // indice atual do vetor de linhas
    int maxSize = 50;                                      // tamanho inicial maximo do vetor de linhas
    int currentSize = 0;                                   // tamanho atual do vetor de linhas
    int nLines = 0;                                        // numero de linhas do arquivo
    int functionIndexesSize = 0;                           // tamanho do vetor de funcoes
    int callsIndexesSize = 0;                              // tamanho do vetor de chamadas
    int auxTempVars;                                       // variavel auxiliar para saber o tamanho da variavel temporaria
    int *functionsIndexes = malloc(sizeof(int) * maxSize); // vetor de indices das funcoes
    int *callerIndexes = malloc(sizeof(int) * maxSize);    // vetor de indices dos calls
    int newCaller = -1;
    unsigned char *code = (unsigned char *)malloc(sizeof(unsigned char) * maxSize);
    string *lines = brakeInto(buffer, &nLines, '\n'); // divide o buffer em linhas
    char firstCharacter;
    var *tempVars;                                       // variaveis temporarias
    code = generateCaller(code, &currentSize, &maxSize); // gera o codigo que chama a funcao principal
    for (i = 0; i < nLines; i++)
    {
        firstCharacter = lines[i].value[0];
        switch (firstCharacter)
        {
        case 'f':
            functionsIndexes[functionIndexesSize] = currentSize;
            functionIndexesSize++;
            code = generateFunction(code, &currentSize, &maxSize);
            brake;
        case 'e':
            code = generateEnd(code, &currentSize, &maxSize);
            brake;
        case 'r':
            tempVars = parseLineToVar(lines[i], &auxTempVars);
            code = generateReturn(code, &currentSize,
                                  &maxSize, tempsVars[0]);
            free(tempVars);
            brake;
        case 'v':
            tempVars = parseLineToVar(lines[i], &auxTempVars);
            if (auxTempVars != 2)
            {
                code = generateOperation(code, &currentSize, &maxSize,
                                         tempVars[1], tempVars[2], tempVars[3], &newCaller);
                if (newCaller != -1)
                {
                    callerIndexes[callsIndexesSize] = newCaller;
                    callsIndexesSize++;
                    newCaller = -1;
                }
                code = generateAssigment(code, &currentSize, &maxSize, tempVars[0].value);
            }
            else
            {
                code = generateAssigmentOneToOne(code, &currentSize, &maxSize, tempVars[0], tempVars[1]);
            }
            free(tempVars);
            brake;
        case 'r':
            tempVars = parseLineToVar(lines[i], &auxTempVars);
        }
    }

    // botar o indice da ultima funcao aqui indices 5-8
}

byte *generateImul(var v, int *len)
{
    byte *codeToPush;
    if (v.isVar == 1)
    {
        *len = 5;
        codeToPush = malloc(sizeof(byte) * (*len));
        codeToPush[0] = 0x4c;
        codeToPush[1] = 0x0f;
        codeToPush[2] = 0xaf;
        codeToPush[3] = 0x65;
        codeToPush[4] = varInMC(v.value);
    }
    else
    {
        byte aux[4] = intToBytes(v.value, 0);
        *len = 7;
        codeToPush = malloc(sizeof(byte) * (*len));
        codeToPush[0] = 0x4d;
        codeToPush[1] = 0x69;
        codeToPush[2] = 0xe4;
        codeToPush[3] = aux[0];
        codeToPush[4] = aux[1];
        codeToPush[5] = aux[2];
        codeToPush[6] = aux[3];
    }
    return codeToPush;
}

byte *generateSub(var v, int *len)
{
    byte *codeToPush;
    if (v.isVar == 1)
    {
        *len = 4;
        codeToPush = malloc(sizeof(byte) * (*len));
        codeToPush[0] = 0x4c;
        codeToPush[1] = 0x2b;
        codeToPush[2] = 0x65;
        codeToPush[3] = varInMC(v.value);
    }
    else
    {
        byte aux[4] = intToBytes(v.value, 0);
        *len = 7;
        codeToPush = malloc(sizeof(byte) * (*len));
        codeToPush[0] = 0x49;
        codeToPush[1] = 0x81;
        codeToPush[2] = 0xec;
        codeToPush[3] = aux[0];
        codeToPush[4] = aux[1];
        codeToPush[5] = aux[2];
        codeToPush[6] = aux[3];
    }
    return codeToPush;
}

/*
    Funcao responsavel para gerar o codigo de maquina
    de soma
*/
byte *generateSum(var v, int *len)
{
    byte *codeToPush;
    if (v.isVar == 1)
    {
        *len = 4;
        codeToPush = malloc(sizeof(byte) * (*len));
        codeToPush[0] = 0x4c;
        codeToPush[1] = 0x03;
        codeToPush[2] = 0x65;
        codeToPush[3] = varInMC(v.value);
    }
    else
    {
        byte aux[4] = intToBytes(v.value, 0);
        *len = 7;
        codeToPush = malloc(sizeof(byte) * (*len));
        codeToPush[0] = 0x49;
        codeToPush[1] = 0x81;
        codeToPush[2] = 0xc4;
        codeToPush[3] = aux[0];
        codeToPush[4] = aux[1];
        codeToPush[5] = aux[2];
        codeToPush[6] = aux[3];
    }
    return codeToPush;
}

byte *generateCall(int funcIndex)
{
    byte codeToPush = malloc(sizeof(byte) * 8);
    byte aux[4] = intToBytes(funcIndex, 0);
    codeToPush[0] = 0xe8;
    codeToPush[1] = aux[0];
    codeToPush[2] = aux[1];
    codeToPush[3] = aux[2];
    codeToPush[4] = aux[3];
    codeToPush[5] = 0x49;
    codeToPush[6] = 0x89;
    codeToPush[7] = 0xc4;
    return codeToPush;
}

/*
    Funcao responsavel por gerar o codigo de maquina
    das operacoes aritimeticas ou de chamada de funcao
*/

byte *generateOperation(byte *code, int *currentSize, int *maxSize,
                        var v1, var op, var v2, int *newCaller)
{
    byte *mov;
    byte *operation;
    int lenMov;
    int lenOperation;
    if (v1.isVar != -1)
    {
        if (v1.isVar == 1)
        {
            lenMov = 4;
            mov = malloc(sizeof(byte) * lenMov);
            mov = varToR12(v1.value);
        }
        else
        {
            byte constInBytes[4] = intToBytes(v1.value, 0);
            lenMov = 7;
            mov = malloc(sizeof(byte) * lenMov);
            mov[0] = 0x49;
            mov[1] = 0xc7;
            mov[2] = 0xc4;
            mov[3] = constInBytes[0];
            mov[4] = constInBytes[1];
            mov[5] = constInBytes[2];
            mov[6] = constInBytes[3];
        }
        if (op.value == 1)
        {
            operation = generateSum(code, currentSize, maxSize, v2, &lenOperation);
        }
        else if (op.value == 2)
        {
            operation = generateSub(code, currentSize, maxSize, v2, &lenOperation);
        }
        else
        {
            operation = generateMul(code, currentSize, maxSize, v2, &lenOperation);
        }
    }
    else
    {
        if (v1.isVar == 1)
        {
            lenMov = 4;
            mov = malloc(sizeof(byte) * lenMov);
            mov[0] = 0x48;
            mov[1] = 0x8b;
            mov[2] = 0x7d;
            mov[3] = varInMC(v1.value);
        }
        else
        {
            byte constInBytes[4] = intToBytes(v1.value, 0);
            lenMov = 7;
            mov = malloc(sizeof(byte) * lenMov);
            mov[0] = 0x48;
            mov[1] = 0xc7;
            mov[2] = 0xc7;
            mov[3] = constInBytes[0];
            mov[4] = constInBytes[1];
            mov[5] = constInBytes[2];
            mov[6] = constInBytes[3];
        }
        operation = generateCall();
        lenOperation = 5;
        *newCaller = lenMov + (*currentSize);
    }

    code = doubleSize(code, maxSize, *currentSize + (lenOperation + lenMov) >= *maxSize);
    code = pushMachineCode(code, mov, *currentSize, lenMov);
    code = pushMachineCode(code, operation, *currentSize, lenOperation);
    free(mov);
    free(operation);
    return code;
}

/*
Funcao reponsavel por gerar codigo de maquina de
assigment com operacoes ou calls

O registro %r12 eh usado para guardar o resultado de operacoes
entao basta mover o que tiver no registro %r12 para o local
certo da stack

Codigo equivalente de assembly:

movq %r12,-X(%rbp)

*/

byte *generateAssigment(byte *code, int *currentSize, int *maxSize, int value)
{
    byte lastByte = varInMC(value);
    byte codeToPush[4] = {0x4c, 0x89, 0x65, lastByte};
    code = doubleSize(code, maxSize, *currentSize + 4 >= *maxSize);
    code = pushMachineCode(code, codeToPush, *currentSize, 4);
    return code;
}

/*
Funcao responsavel por gerar codigo de maquina de
assigment sem operacoes e sem calls
Temos dois caso:
1 - var = const
2 - var1 = var2

codigo de assembly equivalete caso 1:

movq $YY,-X(%rbp)

botamos a constante na pilha

codigo de assembly equivalente caso 2:

movq -X1(%rbp),%r12
movq %r12,-X2(%rbp)

*/
byte *generateAssigmentOneToOne(byte *code, int *currentSize, int *maxSize, var v1, var v2)
{

    byte codeToPush[8];
    byte aux[4];
    if (v.isVar == 0)
    {
        aux = intToBytes(v2.value, 0);
        codeToPush[0] = 0x48;
        codeToPush[1] = 0xc7;
        codeToPush[2] = 0x45;
        codeToPush[3] = varInMC(v1.value); // pega o byte que representa a variavel
        codeToPush[4] = aux[0];
        codeToPush[5] = aux[1];
        codeToPush[6] = aux[2];
        codeToPush[7] = aux[3];
    }
    else
    {
        aux = varToR12(v1.value);
        codeToPush[0] = aux[0];
        codeToPush[1] = aux[1];
        codeToPush[2] = aux[2];
        codeToPush[3] = aux[3];
        codeToPush[4] = 0x4c;
        codeToPush[5] = 0x89;
        codeToPush[6] = 0x65;
        codeToPush[7] = varInMC(v2.value);
    }

    code = doubleSize(code, maxSize, *currentSize + 8 >= *maxSize);
    code = pushMachineCode(code, codeToPush, *currentSize, 8);
    return code;
}

/*
Funcao que gera o codigo de retono
Aqui temos duas possibilidades:
1 - Retornar uma variavel
2 - Retornar uma constante

se retornar uma variavel, temos o assembly :

movq -X(%rbp),%rax

onde X eh o equivalente na stack a variavel

se retornar uma constante, temos o assembly :

movq $YY,%rax

onde YY eh a constante

*/

byte *generateReturn(byte *code, int *currentSize, int *maxSize, var v)
{
    int size;
    byte *lastBytes = varOrConstBytes(v, &size);
    size += 3;
    byte codeToPush[size] = {0x48, 0x8b, 0x45};
    code = doubleSize(code, maxSize, *currentSize + size >= *maxSize);
    code = pushMachineCode(code, codeToPush, *currentSize, size);
    return code;
}

/*
 Funcao que gera o codigo de termino de uma funcao (end)
 Codigo equivalente de assembly:

 leave
 ret

*/

byte *generateEnd(byte *code, int *currentSize, int *maxSize)
{
    byte *codeToPush = {0xc9, 0xc3};
    code = doubleSize(code, maxSize, *currentSize + 2 >= *maxSize);
    code = pushMachineCode(code, codeToPush, *currentSize, 2);
    return code;
}

/*
    Funcao que gera o codigo de cabecalho de uma funcao (function)
    Codigo equivalente de assembly:

    pushq %rbp
    movq %rsp,%rbp
    subq $48,%rsp
    movq %rdi,-48(%rbp)

    no caso estamos alocando memoria para as 5 variaveis e para o
    parametro de entrada 6 * 8 = 48
    e passamos o parametro para a primeira parte da stack
    para salvarmos ele entre chamadas.

*/

byte *generateFunction(byte *code, int *currentSize, int *maxSize)
{
    byte *codeToPush = {0x55, 0x48, 0x89,
                        0xe5, 0x48, 0x83,
                        0xec, 0x30, 0x48,
                        0x89, 0x7d, 0xd0};
    code = doubleSize(code, maxSize, *currentSize + 11 >= *maxSize);
    code = pushMachineCode(code, codeToPush, *currentSize, 11);
    return code;
}

/*

Gera o chamador da ultima funcao
Codigo equivalente de assembly:

pushq %rbp
movq %rsp,%rbp
call lastFunction
leave
ret

*/

byte *generateCaller(byte *code, int *currentSize, int *maxSize)
{
    byte *codeToPush = {0x55, 0x48, 0x89, 0xe5, 0xe8,
                        0x00, 0x00, 0x00, 0x00, 0xc9, 0xc3}; // codigo de maquina para chamar a funcao
    // note que o equivalente de call esta chamando somente 0, no final do wrapper
    // ele ajeita para chamar a ultima funcao
    // a funcao nao necessita de avaliar o tamanho pois e a primeira funcao a ser chamada
    code = pushMachineCode(code, codeToPush, currentSize, 11);
    *currentSize += 11;
    return code;
}

byte *varToR12(int varN)
{
    byte lastByte = varInMC(varN);
    byte res[4] = {0x4c, 0x8b, 0x65, lastByte};
    return res;
}

/*
    Dado uma linha do arquivo decompoe-la em uma variavel,
    constante ou operacao

*/
var *parseLineToVar(string line, int *size)
{
    string *pieces = brakeInto(line, size, ' '); // quebra em espacos
    var *vars = malloc(sizeof(var) * (*size));   // aloca memoria para as variaveis
    char firstLetter;                            // primeira letra da linha
    for (int i = 0; i < *size; i++)
    { // para cada variavel
        firstLetter = pieces[i].value[0];
        switch (firstLetter)
        {
        case 'v':
            vars[i].value = pieces[i].value[1] - '0'; // pega o valor da variavel
            vars[i].isVar = 1;                        // variavel
            break;
        case '$':
            char *aux = (char *)removeFirstChar(pieces[i].value);
            vars[i].isVar = 0;         // constante
            vars[i].value = atoi(aux); // pega o valor da constante
            break;
        case '+':
            vars[i].isVar = -1; // operacao
            vars[i].value = 1;  // soma
            break;
        case '-':
            vars[i].isVar = -1; // operacao
            vars[i].value = 2;  // subtracao
            break;
        case '*':
            vars[i].isVar = -1; // operacao
            vars[i].value = 3;  // multiplicacao
            break;
        case 'c':
            vars[i].isVar = -1; // operacao
            vars[i].value = 4;  // call
            break;
        case '=': // volta um pois esta informacao e desnecessaria
            i--;
            break;
        default:
            if (firstLetter - '0' >= 0 && firstLetter - '0' <= 9)
            {
                vars[i].value = atoi(pieces[i].value[1]); // pega o valor de indice de funcao
                vars[i].isVar = 2;                        // indice de funcao
            }
            else
            {
                i--; // considerando que tem algum espaco no lugar errado;
            }
            break;
            // TODO: implementar operacoes
        }
    }
    return vars;
}

// funcao que devolve os bytes de acordo com o tipo de var
byte *varOrConstBytes(var v, int *size)
{
    int size = v.isVar == 1 ? 1 : 4; // se for variavel, o tamanho e 1, se for constante, o tamanho e 4
    byte res[size];
    if (v.isVar == '0')
    {
        *size = 1;
        res[0] = varInMc(v.value); // pega o indice correspondete de pilha
    }
    else
    {
        *size = 4;
        res = intToBytes(v.value, 0); // pega o valor constante (em 4 bytes)
    }
    return res;
}

/*
Dado um indice de variavel retorna o indice da pilhar correspodente
*/

byte varInMc(int x)
{
    byte res = 0xd8 + (8 * x);
    return res;
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

/*
    Dobra o tamanho de um array (string ou byte)
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

string generateBuffer(FILE *f) // Funcao que retorna um buffer para determinado arquivo
{
    string buffer;
    buffer.len = sizeOfFile(f);           // pega o tamanho do arquivo
    buffer.value = (char *)malloc(*size); // aloca o tamanho do arquivo
    fread(buffer, 1, size, f);            // le o arquivo e coloca no buffer
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