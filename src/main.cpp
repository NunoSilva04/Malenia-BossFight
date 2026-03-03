#include <stdio.h>
#include "window.h"

int main(int argc, int *argv[]){
    Window window;
    
    window.create_window("SDL Window");

    while(window.get_event() == Window::Event::NOTHING){
        printf("on window\n");
        if(window.get_event() == Window::Event::ESCAPE){
            break;
        }
    }

    window.destroy_window();
    return 0;
}