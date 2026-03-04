/**********************************************************************************
*   File: input.h
* 
*   Description: 
*        Handles certain keyboard input and all controller input. Uses SDL3.
* 
*   See: 
*       docs/Engine/input.md 
*       for detailed documentation.
* 
*************************************************************************************/

#ifndef __INPUT_H__
#define __INPUT_H__

#include <SDL3/SDL.h>
#include "core.h"

typedef enum Events{
    //Keyboard Events
    Keyboard_Escape,
    //Controller Events
    Gamepad_Circle,
    Gamepad_X,
    Gamepad_Square,
    Gamepad_Triangle,
}Event;

typedef enum Listeners_Id{
    Window_Id,
}Listeners_Id;

class Input{
public:
    Input();
    ~Input();
    bool initialize_input(void);
    void update_input(void);
    static Core::n_vector<Event> dispatch_input(Listeners_Id listener);
    void close_input(void);

private:
    SDL_Gamepad *gamepad;
    static Core::n_vector<Event> event_vector;
};

#endif