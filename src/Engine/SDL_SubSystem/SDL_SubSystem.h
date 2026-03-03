#ifndef __SDL_SUBSYTEM_H__
#define __SDL_SUBSYTEM_H__

#include <SDL3/SDL.h>
#include <iostream>

class SDL_SubSytem{
public:
    SDL_SubSytem(){
        if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS)){
            throw std::runtime_error("Couldn't initalize SDL_SubSystem\n");
        }
    }

    ~SDL_SubSytem(){
        SDL_Quit();
    }
};

#endif 