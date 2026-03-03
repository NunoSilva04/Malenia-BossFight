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
        printf("Couldn't get gamepad\n");
        #if DEBUG
        return false;
        #endif
    }

    return true;
}

bool Input::get_event(void){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type){
        case SDL_EVENT_KEY_DOWN:
            if(event.key.key == SDLK_ESCAPE){
                return false;
            }else if(event.key.key == SDLK_A){
                event_vector.push_back(Keyboard_A);
            }else if(event.key.key == SDLK_W){
                event_vector.push_back(Keyboard_W);
            }else if(event.key.key == SDLK_S){
                event_vector.push_back(Keyboard_S);
            }else if(event.key.key == SDLK_D){
                event_vector.push_back(Keyboard_D);
            }
        break;
    }

    return true;
}

void Input::test_function(void){
    event_vector.print();

    Events e;
    event_vector.pop_first(&e);
    std::cout << "Popped: " << e << '\n';
    event_vector.print();

    event_vector.pop_first(&e);
    std::cout << "Popped: " << e << '\n';
    event_vector.print();
    
    event_vector.empty_vector();
    event_vector.print();

    return;
}

void Input::close_input(void){
    SDL_CloseGamepad(gamepad);
}