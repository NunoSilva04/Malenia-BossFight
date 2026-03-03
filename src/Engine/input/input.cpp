#include "input.h"
#include <cstdio>

Input::Input(){

}

Input::~Input(){

}

bool Input::initalize_input(void){
    if(!SDL_HasGamepad()){
        printf("No gamepad found\n");
        #if DEBUG
        return false;
        #endif
    }

    int count = 0;
    SDL_JoystickID *joystick_id = SDL_GetGamepads(&count);
    gamepad = SDL_OpenGamepad(joystick_id[0]);
    if(gamepad == NULL){
        #if DEBUG
        printf("Couldn't get gamepad\n");
        return false;
        #endif
    }

    return true;
}


void Input::close_input(void){
    SDL_CloseGamepad(gamepad);
}