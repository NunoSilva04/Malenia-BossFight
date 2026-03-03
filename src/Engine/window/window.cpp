#include "window.h"
#include <stdio.h>

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

void Window::destroy_window(void){
    SDL_DestroyWindow(window);

    return;
}