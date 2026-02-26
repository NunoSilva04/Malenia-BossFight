#include <stdio.h>
#include "testFile.hpp"

int main(){
    printf("Hello world\n");
    int a;

    addHello(2, 5);

    #if DEBUG
        printf("Hello from debug\n");
    #endif

    return 0;
}