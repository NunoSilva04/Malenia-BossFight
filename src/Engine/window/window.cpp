#include "window.h"
#include <stdio.h>

Window::Window(void){
    window = nullptr;
}

Window::~Window(void){

}

bool Window::create_window(const char *window_name){
    if(!SDL_Init(SDL_INIT_VIDEO)){
        printf("Couldn't inititalize SDL video");
        return false;
    }

    window = SDL_CreateWindow(window_name, 800, 600, SDL_WINDOW_VULKAN);
    if(window == NULL){
        printf("Couldn't create window\n");
        return false;
    }

    return true;
}

Window::Event Window::get_event(void){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type){
        case SDL_EVENT_KEY_DOWN:
            if(event.key.key == SDLK_ESCAPE){
                return Event::ESCAPE;
            }
        break;
    }
    
    return Event::NOTHING;
}

void Window::destroy_window(void){
    SDL_DestroyWindow(window);
    SDL_Quit();

    return;
}