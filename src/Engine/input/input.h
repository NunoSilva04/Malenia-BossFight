#ifndef __INPUT_H__
#define __INPUT_H__

#include <SDL3/SDL.h>

typedef enum Events{
    Keyboard_Escape,
    Gamepad_Circle,
}Events;

class Input{
public:
    Input();
    ~Input();
    bool initalize_input(void);

    void close_input(void);

private:
    SDL_Gamepad *gamepad;
};

#endif