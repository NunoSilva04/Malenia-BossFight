#include <stdio.h>

int main(){
    printf("Hello world\n");

    #if DEBUG
        printf("Hello from debug\n");
    #endif

    return 0;
}