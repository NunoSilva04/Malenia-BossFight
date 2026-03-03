#ifndef __INPUT_H__
#define __INPUT_H__

#include <SDL3/SDL.h>
#include "n_vector.h"

typedef enum Events{
    Keyboard_Escape,
    Keyboard_A,
    Keyboard_W,
    Keyboard_S,
    Keyboard_D,
    Gamepad_Circle,
}Events;

class Input{
public:
    Input();
    ~Input();
    bool initalize_input(void);
    bool get_event(void);
    void test_function(void);
    void close_input(void);

private:
    SDL_Gamepad *gamepad;
    Core::n_vector<Events> event_vector;
};

#endif