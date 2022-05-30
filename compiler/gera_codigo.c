#include <stdlib.h>
#include <stdio.h>

typedef int (*funcp) (int x);
void gera_codigo (FILE *f, unsigned char code[], funcp *entry);
unsigned char * lbsToMachineCode(char *lbs);
void runMachineCode(unsigned char*lbs,int size);
unsigned char * generateFunctionHeader(int size);
unsigned char * generateFunctionFooter();

