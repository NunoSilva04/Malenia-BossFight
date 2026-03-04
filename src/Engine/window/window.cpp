#include "window.h"
#include <stdio.h>
#include "input.h"

Window::Window(void){
    window = nullptr;
}

Window::~Window(void){

}

bool Window::create_window(const char *window_name){
    window = SDL_CreateWindow(window_name, 800, 600, SDL_WINDOW_VULKAN);
    if(window == NULL){
        printf("Couldn't create window\n");
        return false;
    }

    return true;
}

bool Window::should_render_window(void){
    static bool render = true;

    Core::n_vector<Event> queue = Input::dispatch_input(Window_Id);
    for(size_t i = 0; i < queue.vector_size(); i++){
        Event e;
        queue.pop_first(&e);
        if(e == Keyboard_Escape){
            render = false;
        }
    }

    return render;
}

void Window::update_window(float frame_time, Input *input){
    input->update_input();
}  

void Window::render_window(void){

}

void Window::destroy_window(void){
    SDL_DestroyWindow(window);

    return;
}