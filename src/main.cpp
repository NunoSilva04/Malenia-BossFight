#include <stdio.h>
#include "SDL_SubSytem.h"
#include "window.h"
#include "input.h"

int main(void){
    SDL_SubSytem sdl;

    Window window;
    window.create_window("SDL Window");
    Input input;
    if(!input.initalize_input()){
        return 0;
    }

    input.close_input();
    window.destroy_window();
    return 0;
}