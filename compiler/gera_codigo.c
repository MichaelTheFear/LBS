
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
unsigned char *convertionWrapper(string buffer, funcp *entry, int *starterIndex);
byte *fixCallIndexes(byte *code, int *callsIndexes, int *functionIndexes,
                     int callsSize, int functionsSize);

// funcaoes de buffer
string generateBuffer(FILE *f);
int sizeOfFile(FILE *f);
void freeBuffer(string buffer);

// funcoes de geracao de codigo de maquina
byte *generateEnd(byte *code, int *currentSize, int *maxSize);
byte *generateFunction(byte *code, int *currentSize, int *maxSize);
byte *generateCall(int funcIndex);
byte *generateAssigmentOneToOne(byte *code, int *currentSize, int *maxSize, var v1, var v2);
byte *generateOperation(byte *code, int *currentSize, int *maxSize,
                        var v1, var op, var v2, int *newCaller);
byte *generateZret(byte *code, int *currentSize, int *maxSize, var cmp, var v1);
byte *generateSub(var v, int *len);
byte *generateSum(var v, int *len);
byte *generateMul(var v, int *len);
byte *generateReturn(byte *code, int *currentSize, int *maxSize, var v);
byte *generateAssigment(byte *code, int *currentSize, int *maxSize,
                        int varNum);
byte *generateCaller(byte *code, int *currentSize, int *maxSize);

// funcoes auxiliares
byte *pushMachineCode(byte *array, byte *code, int *sizeArray, int sizeCode);
string *breakInto(string buffer, int *size, char c);
byte *stackPosition(int varNum);
byte *littleThatEndian(byte *bytes, int fillFF);
byte *intToBytes(int x);
void *doubleSize(void *ptr, int *size, int condition);
byte varInMC(int varNum);
byte *varOrConstBytes(var v, int *size);
byte *varToR12(int varNum);
string removeFirstChar(string s);

void gera_codigo(FILE *f, unsigned char code[], funcp *entry)
{
    string buffer = generateBuffer(f);
    int starterIndex; // indice de comeco da ultima funcao do array de codigo
    // code = (unsigned char *)convertionWrapper(buffer, entry, &starterIndex);
    // entry = (*funcp)(int x) &code[starterIndex];
    freeBuffer(buffer);
}
/*
unsigned char *convertionWrapper(string buffer, funcp *entry, int *starterIndex)
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
    string *lines = breakInto(buffer, &nLines, '\n'); // divide o buffer em linhas
    char firstCharacter;
    var *tempVars; // variaveis temporarias
    for (i = 0; i < nLines; i++)
    {
        firstCharacter = lines[i].value[0];
        switch (firstCharacter)
        {
        case 'f':
            functionsIndexes[functionIndexesSize] = currentSize;
            functionIndexesSize++;
            code = generateFunction(code, &currentSize, &maxSize);
            break;
        case 'e':
            code = generateEnd(code, &currentSize, &maxSize);
            break;
        case 'r':
            tempVars = parseLineToVar(lines[i], &auxTempVars);
            code = generateReturn(code, &currentSize,
                                  &maxSize, tempVars[0]);
            free(tempVars);
            break;
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
            break;
        case 'z':
            tempVars = parseLineToVar(lines[i], &auxTempVars);
            code = generateZret(code, &currentSize, &maxSize, tempVars[0], tempVars[1]);
            code = generateReturn(code, &currentSize,
                                  &maxSize, tempVars[1]);
        default:
            break;
        }
    }
    code = fixCallIndexes(code, callerIndexes, functionsIndexes, callsIndexesSize, functionIndexesSize);
    // botar o indice da ultima funcao aqui indices 5-8
    *starterIndex = functionsIndexes[functionIndexesSize];
    return code;
}

*/

/*
    Funcao responsavel para gerar o codigo de maquina de
    return if zero (zret)

    codigo de maquina equivalente:

    movq -x(%rsp), %r12
    cmp $0, %r12
    jne pula

    movq YY, %rax
    leave
    ret
pula:
    . . .


    movq YY, %rax
    leave
    ret
    sao feitos fora desta funcao

*/
byte *generateZret(byte *code, int *currentSize, int *maxSize, var v1, var v2)
{
    byte *mov = varToR12(v1.value);            // mov
    byte cmpTo0[4] = {0x49, 0x83, 0xfc, 0x00}; // cmp $0, %r12
    byte jump[2];
    if (v2.isVar == 1)
    { // se v2 for variavel
        jump[0] = 0x75;
        jump[1] = 0x06;
    }
    else
    { // se v2 for constante
        jump[0] = 0x75;
        jump[1] = 0x09;
    } // jne
    code = doubleSize(code, maxSize, *currentSize + 10 >= *maxSize);
    code = pushMachineCode(code, mov, currentSize, 4);
    code = pushMachineCode(code, cmpTo0, currentSize, 4);
    code = pushMachineCode(code, jump, currentSize, 2);
    return code;
}

/*
    Funcao responsavel para gerar o codigo de maquina
    de multiplicacao (olhar comentario a cima de generateOperation)
*/

byte *generateMul(var v, int *len)
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
        byte *aux = intToBytes(v.value);
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

/*
    Funcao responsavel para gerar o codigo de maquina
    de subitracao (olhar comentario a cima de generateOperation)
*/

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
        byte *aux = intToBytes(v.value);
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
    de soma (olhar comentario a cima de generateOperation)
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
        byte *aux = intToBytes(v.value);
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

/*
    Funcao responsavel para gerar o codigo de maquina
    de call (olhar comentario a cima de generateOperation)
*/

byte *generateCall(int funcIndex)
{
    byte *codeToPush = malloc(sizeof(byte) * 8);
    byte *aux = intToBytes(funcIndex);
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

    existem 4 caso, cada um deles responsavel pelas 4 operacoes
    possiveis (soma, subtracao, multiplicacao e chamar funcao)

    nos 3 primeiros casos o problema e divido em 2
    1- mover o primeiro operando para o registro auxiliar
    2- realizar a operacao com o registro auxiliar e o segundo operando,
    colocando o resultado no registro auxiliar

    cada um por sua vez se divide em 2 (para caso seja constante ou variavel)

    caso o primeiro operando seja uma constante o codigo de assembly
    equivalente seria:

    movq $YY,%r12

    caso o primeiro operando seja uma variavel o codigo de assembly
    equivalente seria:

    movq -X(%rbp),%r12


    agora para o segundo operando e suas respectivas operacoes



    addq -X(%rbp),%r12

    ou

    addq $YY,%r12



    subq -X(%rbp),%r12

    ou

    subq $YY,%r12



    imul -X(%rbp),%r12

    ou

    imul $YY,%r12


    agora caso seja uma chamada de funcao necessitamos de primeiramente
    passar o valor ou variavel certa para o registro de argumento

    se variavel o codigo de assembly:

    movq -X(%rbp),%rdi

    se constante o codigo de assembly:

    movq $YY,%rdi


    depois chama a funcao

    call funcaoN

    depois move o retorno da funcao para o registrador auxiliar

    movq %rax,%r12

*/

byte *generateOperation(byte *code, int *currentSize, int *maxSize,
                        var v1, var op, var v2, int *newCaller)
{
    byte *mov;        // bytes da primeira parte
    byte *operation;  // bytes da segunda parte
    int lenMov;       // tamanho do mov
    int lenOperation; // tamanho do operation
    if (v1.isVar != -1)
    { // se nao for call
        if (v1.isVar == 1)
        { // se primeiro operando for variavel
            lenMov = 4;
            mov = malloc(sizeof(byte) * lenMov);
            mov = varToR12(v1.value); // mov para o registro auxiliar
        }
        else
        {                                              // se primeiro operando for constante
            byte *constInBytes = intToBytes(v1.value); // converte para bytes
            lenMov = 7;
            mov = malloc(sizeof(byte) * lenMov);
            mov[0] = 0x49;
            mov[1] = 0xc7;
            mov[2] = 0xc4; // mov para o registro auxiliar
            mov[3] = constInBytes[0];
            mov[4] = constInBytes[1];
            mov[5] = constInBytes[2];
            mov[6] = constInBytes[3];
        }
        if (op.value == 1)
        {
            operation = generateSum(v2, &lenOperation); // gerar o codigo de soma
        }
        else if (op.value == 2)
        {
            operation = generateSub(v2, &lenOperation); // gerar o codigo de subtracao
        }
        else
        {
            operation = generateMul(v2, &lenOperation); // gerar o codigo de multiplicacao
        }
    }
    else
    {
        if (v1.isVar == 1)
        { // move variavel para argumento
            lenMov = 4;
            mov = malloc(sizeof(byte) * lenMov);
            mov[0] = 0x48;
            mov[1] = 0x8b;
            mov[2] = 0x7d;
            mov[3] = varInMC(v1.value); // numero da variavel
        }
        else
        {                                              // move constante para argumento
            byte *constInBytes = intToBytes(v1.value); // converte para bytes
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
        operation = generateCall(v2.value); // gerar o codigo de chamada de funcao
        lenOperation = 8;
        *newCaller = lenMov + (*currentSize); // salva o indice do codigo de chamada para fazer a correcao ao final
    }

    code = doubleSize(code, maxSize, *currentSize + (lenOperation + lenMov) >= *maxSize); // aumenta o tamanho do codigo de maquina
    code = pushMachineCode(code, mov, currentSize, lenMov);                               // coloca o codigo de mov no codigo de maquina
    code = pushMachineCode(code, operation, currentSize, lenOperation);                   // coloca o codigo de operacao no codigo de maquina
    free(mov);                                                                            // libera o codigo de mov
    free(operation);                                                                      // libera o codigo de operacao
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
    byte lastByte = varInMC(value);                                 // byte equivalente ao valor da variavel
    byte codeToPush[4] = {0x4c, 0x89, 0x65, lastByte};              // codigo de maquina para movimentar o valor da variavel para o local certo da stack
    code = doubleSize(code, maxSize, *currentSize + 4 >= *maxSize); // verifica se o tamanho do codigo de maquina eh maior que o tamanho maximo
    code = pushMachineCode(code, codeToPush, currentSize, 4);       // adiciona o codigo de maquina ao codigo de maquina
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
    byte * aux;
    if (v2.isVar == 0) // se for constante
    {
        aux = intToBytes(v2.value);
        codeToPush[0] = 0x48;
        codeToPush[1] = 0xc7;
        codeToPush[2] = 0x45;
        codeToPush[3] = varInMC(v1.value); // pega o byte que representa a variavel 1
        codeToPush[4] = aux[0];
        codeToPush[5] = aux[1];
        codeToPush[6] = aux[2];
        codeToPush[7] = aux[3];
    }
    else
    {
        aux = varToR12(v2.value); // codigo para mover para o registro auxiliar
        codeToPush[0] = aux[0];
        codeToPush[1] = aux[1];
        codeToPush[2] = aux[2];
        codeToPush[3] = aux[3];
        codeToPush[4] = 0x4c;
        codeToPush[5] = 0x89;
        codeToPush[6] = 0x65;
        codeToPush[7] = varInMC(v1.value); // pega o byte que representa a variavel 2
    }

    code = doubleSize(code, maxSize, *currentSize + 8 >= *maxSize); // verifica se o tamanho do codigo de maquina eh maior que o tamanho maximo
    code = pushMachineCode(code, codeToPush, currentSize, 8);       // adiciona o codigo de maquina ao codigo de maquina
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
    byte *codeToPush;
    int size;
    if (v.isVar == 1)
    { // se for variavel ou argumento
        printf("\n?\n");
        size = 4;
        codeToPush = malloc(sizeof(byte) * 4);
        codeToPush[0] = 0x48;
        codeToPush[1] = 0x8b;
        codeToPush[2] = 0x45;
        codeToPush[3] = varInMC(v.value);
    }
    else
    { // se for constante
        size = 7;
        byte *aux = intToBytes(v.value);
        codeToPush = malloc(sizeof(byte) * 7);
        codeToPush[0] = 0x48;
        codeToPush[1] = 0xc7;
        codeToPush[2] = 0xc0;
        codeToPush[3] = aux[0];
        codeToPush[4] = aux[1];
        codeToPush[5] = aux[2];
        codeToPush[6] = aux[3];
    }
    code = doubleSize(code, maxSize, *currentSize + size >= *maxSize);
    code = pushMachineCode(code, codeToPush, currentSize, size);
    free(codeToPush);
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
    byte codeToPush[2];
    codeToPush[0] = 0xc9;
    codeToPush[1] = 0xc3;
    code = doubleSize(code, maxSize, *currentSize + 2 >= *maxSize);
    code = pushMachineCode(code, codeToPush, currentSize, 2);
    return code;
}

/*
    Funcao que gera o codigo de cabecalho de uma funcao (function)
    Codigo equivalente de assembly:

    pushq %rbp
    movq %rsp,%rbp
    subq $48,%rsp
    movq %rdi,-48(%rbp)

    {0x55,0x48,0x89,0xe5,0x48,0x83,0xec,0x30,0x48,0x89,0x7d,0xd0}

    no caso estamos alocando memoria para as 5 variaveis e para o
    parametro de entrada 6 * 8 = 48
    e passamos o parametro para a primeira parte da stack
    para salvarmos ele entre chamadas.

*/

byte *generateFunction(byte *code, int *currentSize, int *maxSize)
{
    byte *codeToPush = malloc(sizeof(byte) * 12);
    codeToPush[0] = 0x55;
    codeToPush[1] = 0x48;
    codeToPush[2] = 0x89;
    codeToPush[3] = 0xe5;
    codeToPush[4] = 0x48;
    codeToPush[5] = 0x83;
    codeToPush[6] = 0xec;
    codeToPush[7] = 0x30;
    codeToPush[8] = 0x48;
    codeToPush[9] = 0x89;
    codeToPush[10] = 0x7d;
    codeToPush[11] = 0xd0;
    code = doubleSize(code, maxSize, *currentSize + 12 >= *maxSize); // dobra o tamanho do codigo se necessario
    code = pushMachineCode(code, codeToPush, currentSize, 12);       // adiciona o codigo de maquina
    free(codeToPush);
    return code;
}

/*
    Funcao que gera o codigo de maquina correpondente ao assembly:
    movq -varN(%rbp),%r12
    {0x4c,0x8b,0x65,varN}
*/

byte *varToR12(int varN)
{
    byte lastByte = varInMC(varN);
    byte *res = malloc(sizeof(byte) * 4);
    res[0] = 0x4c;
    res[1] = 0x8b;
    res[2] = 0x65;
    res[3] = lastByte;
    return res;
}

/*

    Dado uma linha do arquivo decompoe-la em uma variavel,
    constante ou operacao

*/

/*
var *parseLineToVar(string line, int *size)
{
   // string *pieces = breakInto(line, size, ' '); // quebra em espacos
    var *vars = malloc(sizeof(var) * (*size));   // aloca memoria para as variaveis
    char firstLetter;                            // primeira letra da linha
    int j = 0;                                   // indice de var[]
    for (int i = 0; i < *size; i++)
    { // para cada variavel
        firstLetter = pieces[i].value[0];
        switch (firstLetter)
        {
        case 'v':
            vars[j].value = pieces[i].value[1] - '0'; // pega o valor da variavel
            vars[j].isVar = 1;                        // variavel
            break;
        case '$':
            string removeAux = removeFirstChar(pieces[i]);
            char *aux = (char *)removeAux.value;
            vars[j].isVar = 0;         // constante
            vars[j].value = atoi(aux); // pega o valor da constante
            break;
        case '+':
            vars[j].isVar = -1; // operacao
            vars[j].value = 1;  // soma
            break;
        case '-':
            vars[j].isVar = -1; // operacao
            vars[j].value = 2;  // subtracao
            break;
        case '*':
            vars[j].isVar = -1; // operacao
            vars[j].value = 3;  // multiplicacao
            break;
        case 'c':
            vars[j].isVar = -1; // operacao
            vars[j].value = 4;  // call
            break;
        default:
            if (firstLetter - '0' >= 0 && firstLetter - '0' <= 9)
            {
                vars[j].value = pieces[i].value[1] - '0'; // pega o valor de indice de funcao
                vars[j].isVar = 2;                        // indice de funcao
            }
            else
            { // caso seja = , ret, zret ou qualquer outra coisa ignora o pedaco
                j--;
            }
            break;
        }
        j++;
    }
    return vars;
}
*/
// funcao que devolve os bytes de acordo com o tipo de var
byte *varOrConstBytes(var v, int *size)
{
    byte *res;
    if (v.isVar == 1)
    {
        *size = 1;
        res = malloc(sizeof(byte) * 1);
        res[0] = varInMC(v.value); // pega o indice correspondete de pilha
    }
    else
    {

        *size = 4;
        res = intToBytes(v.value); // pega o valor constante (em 4 bytes)
    }
    return res;
}

/*
Dado um indice de variavel retorna o indice da pilhar correspodente
p0 = d0 (-48)
v0 = d8 (-40)
v1 = e0 (-32)
v2 = e8 (-24)
v3 = f0 (-16)
v4 = f8 (-8)
*/

byte varInMC(int x)
{
    byte res;
    res = 0xd8 + (8 * x);
    return res;
}

/*
Dado uma string, retorna um array de strings,
usando o caracter c como separador.
O tamanho do array é retornado em size.
*/

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
*/
byte *intToBytes(int x)
{
    byte *array = malloc(sizeof(byte) * 4);
    array[3] = (x >> 24) & 0xFF;
    array[2] = (x >> 16) & 0xFF;
    array[1] = (x >> 8) & 0xFF;
    array[0] = x & 0xFF;
    return array;
}

byte *fixCallIndexes(byte *code, int *callers, int *functions, int sizeCallers, int sizeFunctions)
{
    int i;
    int indexF;
    int call;
    int indexFunctionOnCode;
    byte *aux = malloc(sizeof(byte) * 4);
    for (i = 0; i < sizeCallers; i++)
    {
        call = callers[i];
        aux[0] = code[call];
        aux[1] = code[call + 1];
        aux[2] = code[call + 2];
        aux[3] = code[call + 3];
        // indexF = bytesToInt(aux);
        indexFunctionOnCode = functions[indexF];
        free(aux);
        aux = intToBytes(indexFunctionOnCode - call);
        code[call] = aux[0];
        code[call + 1] = aux[1];
        code[call + 2] = aux[2];
        code[call + 3] = aux[3];
        free(aux);
    }
    return code;
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
    if (condition) // se condition for 1
    {
        *size *= 2;                // dobra o tamanho do array
        ptr = realloc(ptr, *size); // realoca
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
    buffer.len = sizeOfFile(f);                // pega o tamanho do arquivo
    buffer.value = (char *)malloc(buffer.len); // aloca o tamanho do arquivo
    fread(buffer.value, 1, buffer.len, f);     // le o arquivo e coloca no buffer
    return buffer;
}
/*
    Funcao que pega um pedacoe de codigo de maquina e joga no final do buffer
*/
byte *pushMachineCode(byte *array, byte *code, int *sizeArray, int sizeCode)
{
    for (int i = *sizeArray; i < *sizeArray + sizeCode; i++) // loopa array apartir de sizeArray e loopa code apartir de 0
    {
        array[i] = code[i - *sizeArray]; // copia para o array o codigo de maquina
    }
    *sizeArray += sizeCode; // incrementa o tamanho do buffer
    return array;
}

void freeBuffer(string buffer)
{
    free(buffer.value);
}

// area de testes

void printBytes(byte *bytes, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d = %02x\n", i, bytes[i]);
    }
    printf("\n");
}

byte * testGenZret(){
  int maxSize = 30
  int size = 0;
  var v1;
  byte * code;
  code = (byte *)malloc(sizeof(byte) * (*maxSize));
  printBytes(code, *size);
  printf("Size: %d\n",*size);
  //testGenSumAssigment(code,maxSize, size);
  
  
  v1.isVar = 1;
  v1.value = 1;
  code = generateReturn(code, size, maxSize,v1);
  printf("Size 2: %d\n",* size);
  printBytes(code,* size);
  code = generateEnd(code, &size, &maxSize);
  printBytes(code, size);
  printf("Size 3: %d\n",size);
  

  
  
}

/* Area de teste de um programa simples de soma
function
v0 = $4
v1 = $2 + v0
ret v1
end
*/
byte * testGenSumFunc(int * maxSize,int * size){
    byte * code;
    code = (byte *)malloc(sizeof(byte) * (*maxSize));
    code = generateFunction(code, size, maxSize);
    printBytes(code, *size);
    printf("Size: %d\n",*size);
    return code;
}
byte * testGenSumAssigment(byte * code,int * maxSize, int * size){

    code = testGenSumFunc(maxSize,size);
    int funcNum;
    var v1,v2,op,v;
    v2.isVar = 0;
    v2.value = 4;
    v1.isVar = 1;
    v1.value = 0;
    code = generateAssigmentOneToOne(code, size, maxSize,v1,v2);
    printBytes(code, *size);
    printf("Size a: %d\n",*size);
    v2.isVar = 1;
    v2.value = 0;
    v1.value = 2;
    v1.isVar = 0;
    v.isVar = 1;
    v.value = 1;
    op.value=1;
    op.isVar=-2;
    code = generateOperation(code,size,maxSize,v1,op,v2,&funcNum);
    code = generateAssigment(code, size, maxSize,v.value);
    printf("Size a: %d\n",*size);
    return code;
}
byte * testGenSumReturn(int * maxSize, int * size){
    byte * code = testGenSumAssigment(code,maxSize, size);
    var v1;
    v1.isVar = 1;
    v1.value = 1;
    code = generateReturn(code, size, maxSize,v1);
    printf("Size 2: %d\n",* size);
    printBytes(code,* size);
    return code;
}
byte * testGenSumEnd(){
    int maxSize = 30;
    int size = 0;
    byte * code = testGenSumReturn(&maxSize,&size);
    code = generateEnd(code, &size, &maxSize);
    printBytes(code, size);
    printf("Size 3: %d\n",size);
    return code;
}

int testVarToR12()
{
    byte *mov = varToR12(2);
    printBytes(mov, 4);
    free(mov);
    mov = varToR12(-1);
    printBytes(mov, 4);
    free(mov);
    return 0;
}
int testVarOrConst()
{
    var v1;
    byte *res;
    int i;
    v1.isVar = 1;
    v1.value = 1;
    res = varOrConstBytes(v1, &i);
    printBytes(res, i);
    v1.isVar = 0;
    free(res);
    v1.isVar = 0;
    res = varOrConstBytes(v1, &i);
    printBytes(res, i);
    free(res);
    return 0;
}
int testVarInMCode()
{
    byte res;
    res = varInMC(0);
    printf("%s\n", res == 0xd8 ? "OK" : "ERRO");
    res = varInMC(1);
    printf("%s\n", res == 0xd8 + 8 ? "OK" : "ERRO");
    res = varInMC(-1);
    printf("%s\n", res == 0xd0 ? "OK" : "ERRO");
    return 0;
}
int testRemoveFirstChar()
{
    string s = {.len = 5, .value = "teste"};
    string newstr = removeFirstChar(s);
    if (strcmp(newstr.value, "este") == 0)
    {
        printf("removeFirstChar ok\n");
        return 1;
    }
    printf("removeFirstChar failed\n");
    free(newstr.value);
    return 0;
}
int testIntToBytes()
{
    int x = 0x12345678;
    byte *array = intToBytes(x);
    printBytes(array, 4);
    free(array);
    return 0;
}
int testPushMachineCode()
{
    byte *code = malloc(sizeof(byte) * 6);
    byte *newCode = malloc(sizeof(byte) * 4);
    int sizeArray = 6;
    int maxSize = 6;
    newCode[0] = 0xaa;
    newCode[1] = 0xbb;
    newCode[2] = 0xcc;
    newCode[3] = 0xdd;
    code[0] = 0xff;
    code[1] = 0xff;
    code[2] = 0xff;
    code[3] = 0xff;
    code[4] = 0xff;
    code[5] = 0xff;
    code = doubleSize(code, &maxSize, sizeArray + 4 >= maxSize);
    code = pushMachineCode(code, newCode, &sizeArray, 4);
    printBytes(code, sizeArray);
    printf("New size: %d\n", maxSize);
    free(code);
    free(newCode);
    return 0;
}

int main()
{
    testPushMachineCode();
    testIntToBytes();
    // testRemoveFirstChar();
    testVarInMCode();
    testVarOrConst();
    testVarToR12();
    byte * code = testGenSumEnd(); //faltadno 2 free ate aq
    printf("\n %02x\n",code[27]);
    
    funcp f = (funcp)code;
    printf("Res f: %d\n",f(2));
    free(code);
    return 0;
}