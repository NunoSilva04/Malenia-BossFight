#include <stdio.h>
#include "SDL_SubSystem.h"
#include "window.h"
#include "input.h"
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include "Rect.h"

int main(void){
    SDL_SubSytem sdl;

    Window window;
    window.create_window("SDL Window");
    Input input;
    if(!input.initialize_input()){
        return 0;
    }

    float frame_time = 0;

    while(window.should_render_window()){
        window.update_window(frame_time, &input);
        window.render_window();
    }


    n_math::rect<int, false> a;
    a.print();

    input.close_input();
    window.destroy_window();
    return 0;
}