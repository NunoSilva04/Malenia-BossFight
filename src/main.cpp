#include <stdio.h>
#include "SDL_SubSystem.h"
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

    while(input.get_event()){

    }

    input.test_function();
    input.close_input();
    window.destroy_window();
    return 0;
}