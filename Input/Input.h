#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <windows.h>
#include <hidsdi.h>
#include <SDL3/SDL.h>
#include <vector>

enum class EventType{
    BUTTON,
    AXIS,
};

enum class controllerButton{
    TRIANGLE = SDL_GAMEPAD_BUTTON_NORTH,
    CIRCLE = SDL_GAMEPAD_BUTTON_EAST,
    X = SDL_GAMEPAD_BUTTON_SOUTH,
    SQUARE = SDL_GAMEPAD_BUTTON_WEST,
    UP = SDL_GAMEPAD_BUTTON_DPAD_UP,
    RIGHT = SDL_GAMEPAD_BUTTON_DPAD_RIGHT, 
    DOWN = SDL_GAMEPAD_BUTTON_DPAD_DOWN, 
    LEFT = SDL_GAMEPAD_BUTTON_DPAD_LEFT,
    R1 = SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER,
    R3 = SDL_GAMEPAD_BUTTON_RIGHT_STICK,
    L1 = SDL_GAMEPAD_BUTTON_LEFT_SHOULDER,
    L3 = SDL_GAMEPAD_BUTTON_LEFT_STICK,
    SELECT = SDL_GAMEPAD_BUTTON_BACK,
    START = SDL_GAMEPAD_BUTTON_START,
};

enum class controllerAxis{
    R2 = SDL_GAMEPAD_AXIS_RIGHT_TRIGGER,
    RIGHT_X = SDL_GAMEPAD_AXIS_RIGHTX,
    RIGHT_Y = SDL_GAMEPAD_AXIS_RIGHTY,
    L2 = SDL_GAMEPAD_AXIS_LEFT_TRIGGER,
    LEFT_X = SDL_GAMEPAD_AXIS_LEFTX,
    LEFT_Y = SDL_GAMEPAD_AXIS_LEFTY,
};

class IInput{
public:
    virtual ~IInput(){}
    virtual Sint16 getAxisValue(controllerAxis axis) = 0;
};

class Input : public IInput{
public:
    Input();
    ~Input();
    bool initInput(HWND hWnd);  //To remove later 
    bool initInput(void);
    void reset(void);
    void update(float);
    void cleanInput(void);

    //static PHIDP_PREPARSED_DATA preparseData;

private:
    SDL_Gamepad *gamepad;

    struct queue_t{
        EventType type;
        union{
            controllerButton button;
            controllerAxis axis;
        }eventSubType;
        const char *eventTypeName;
        Sint16 value;
        float timeAxisHeld;
    };
    std::vector<queue_t> prevQueue, currQueue;

private:
    Sint16 getAxisValue(controllerAxis axis) override;
};

#endif