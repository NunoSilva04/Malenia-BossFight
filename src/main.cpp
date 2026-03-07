#include <stdio.h>
#include "SDL_SubSystem.h"
#include "window.h"
#include "input.h"

int main(void){
    SDL_SubSytem sdl;

    Window window;
    if(!window.create_window("SDL Window")) return 0;
    Input input;
    if(!input.initialize_input()){
        return 0;
    }

    while(window.should_render_window()){
        input.update_input();
        //Game to be rendered here 
    }

    input.close_input();
    window.destroy_window();
    return 0;
}