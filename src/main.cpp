#include <stdio.h>
#include <SDL3/SDL.h>

int main(){    
    printf("Hello world from SDL\n");

    if(!SDL_Init(SDL_INIT_VIDEO)){
        printf("Couldn't init SDL3\n");
    }

    SDL_Window* window = SDL_CreateWindow("SDL Test", 800, 600, 0);
    if (!window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        SDL_Delay(16); // ~60 FPS idle loop
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}