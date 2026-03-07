#include "input.h"
#include <cstdio>

Core::n_vector<Event> Input::event_vector;

Input::Input(){
    gamepad = nullptr;
}

Input::~Input(){

}

bool Input::initialize_input(void){
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

void Input::update_input(void){
    event_vector.empty_vector();

    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            //Keyboard Events
            case SDL_EVENT_KEY_DOWN:
                if(event.key.key == SDLK_ESCAPE){
                    event_vector.push_back(Keyboard_Escape);
                }else if(event.key.key == SDLK_F11){
                    event_vector.push_back(Keyboard_F11);
                } 
            break;
            //Controller Events
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                if(event.key.key == SDL_GAMEPAD_BUTTON_EAST){
                    event_vector.push_back(Gamepad_Circle);
                }
                else if(event.key.key == SDL_GAMEPAD_BUTTON_SOUTH){
                    event_vector.push_back(Gamepad_X);
                }
                else if(event.key.key == SDL_GAMEPAD_BUTTON_WEST){
                    event_vector.push_back(Gamepad_Square);
                }
                else if(event.key.key == SDL_GAMEPAD_BUTTON_NORTH){
                    event_vector.push_back(Gamepad_Triangle);
                }
            break;
        }
    }

    return;
}

Core::n_vector<Event> Input::dispatch_input(Listeners_Id listener){
    Core::n_vector<Event> queue;
    queue.empty_vector();
    switch(listener){
        case Window_Id:
            for(size_t i = 0; i < event_vector.vector_size(); i++){
                if(event_vector.vector_data_id(i) == Keyboard_Escape){
                    queue.push_back(event_vector.vector_data_id(i));
                }
            }
        break;
    }

    return queue;
}

void Input::close_input(void){
    SDL_CloseGamepad(gamepad);
}