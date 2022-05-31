#include <stdio.h>

int testCall(){
    int a=2;
    a+=2;
    return a;
}


int main(void){
    int b = testCall();
    b+=b;
    
    return 0;
}