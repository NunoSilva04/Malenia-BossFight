#include "Input.h"
#include <cstdio>
#include <cstdint>

//PHIDP_PREPARSED_DATA InputManager::preparseData = nullptr;

typedef struct{
    controllerButton buttonValue;
    const char *buttonName;
}buttons_t;

buttons_t buttons[] = {
    {controllerButton::TRIANGLE, "Triangle"},
    {controllerButton::CIRCLE, "Circle"},
    {controllerButton::X, "X"},
    {controllerButton::SQUARE, "Square"},
    {controllerButton::UP, "Up"},
    {controllerButton::RIGHT, "Right"},
    {controllerButton::DOWN, "Down"},
    {controllerButton::LEFT, "Left"},
    {controllerButton::R1, "R1"},
    {controllerButton::R3, "R3"},
    {controllerButton::L1, "L1"},
    {controllerButton::L3, "L3"},
    {controllerButton::SELECT, "Select"},
    {controllerButton::START, "Start"},
};
const int numButtons = 14;

typedef struct{
    controllerAxis axis;
    const char *axisName;
}axis_t;

axis_t axis[] = {
    {controllerAxis::R2, "R2"},
    {controllerAxis::RIGHT_X, "Right X"},
    {controllerAxis::RIGHT_Y, "Right Y"},
    {controllerAxis::L2, "L2"},
    {controllerAxis::LEFT_X, "Left X"},
    {controllerAxis::LEFT_Y, "Left Y"},
};
const int numAxis = 6;
const Sint16 movingThreshold = 3000;

Input::Input(){
    gamepad = nullptr;
    prevQueue = currQueue = std::vector<queue_t>();
}

Input::~Input(){

}

bool Input::initInput(HWND hWnd){
    uint32_t numDevices = 0;
    if(GetRawInputDeviceList(NULL, &numDevices, sizeof(RAWINPUTDEVICELIST)) == (uint32_t) - 1){
        std::printf("Failed to get device count\n");
        return false;
    }

    RAWINPUTDEVICELIST *deviceList = new RAWINPUTDEVICELIST[numDevices];
    if (GetRawInputDeviceList(deviceList, &numDevices, sizeof(RAWINPUTDEVICELIST)) == (uint32_t) - 1) {
        std::printf("Failed to get device list\n");
        delete[] deviceList;
        return false;
    }

    for(uint8_t deviceNum = 0; deviceNum < numDevices; deviceNum ++){
        uint32_t nameSize = 0;
        GetRawInputDeviceInfo(deviceList[deviceNum].hDevice, RIDI_DEVICENAME, NULL, &nameSize);
        wchar_t *deviceName = new wchar_t[nameSize + 1]();
        GetRawInputDeviceInfo(deviceList[deviceNum].hDevice, RIDI_DEVICENAME, deviceName, &nameSize);
        wprintf(L"Device Number %d: %ls\n", deviceNum, deviceName);
        delete[] deviceName;

        if(deviceNum == 0){
            //Parse Data
            PHIDP_PREPARSED_DATA preparseData = nullptr;
            uint32_t preparseDataSize = 0;
            GetRawInputDeviceInfo(deviceList[deviceNum].hDevice, RIDI_PREPARSEDDATA, nullptr, &preparseDataSize);

            preparseData = (PHIDP_PREPARSED_DATA) malloc(preparseDataSize);
            if(GetRawInputDeviceInfo(deviceList[deviceNum].hDevice, RIDI_PREPARSEDDATA, preparseData, &preparseDataSize) == -1){
                std::printf("Couldn't get parse data\n");
                free(preparseData);
                return false;
            }

            //Caps
            HIDP_CAPS caps;
            if(HidP_GetCaps(preparseData, &caps) == HIDP_STATUS_INVALID_PREPARSED_DATA){
                std::printf("Couldn't get caps\n");
                free(preparseData);
                return false;
            }
            std::printf("NumberInputButtonCaps: %u\nNumberInputValueCaps: %u\n", caps.NumberInputButtonCaps, caps.NumberInputValueCaps);

            //Button caps
            HIDP_BUTTON_CAPS buttonCaps[32];
            u_short buttonCapsLength = caps.NumberInputButtonCaps;
            if(HidP_GetButtonCaps(HidP_Input, buttonCaps, &buttonCapsLength, preparseData) == HIDP_STATUS_INVALID_PREPARSED_DATA){
                std::printf("Couldn't get button caps\n");
                free(preparseData);
                return false;
            }

            for(uint32_t i = 0; i < buttonCapsLength; i++){
                if(buttonCaps[i].UsagePage == 0x09){
                    if(buttonCaps[i].IsRange){
                        std::printf("Button range: UsageMin = %u to UsageMax = %u\n", buttonCaps[i].Range.UsageMin, buttonCaps[i].Range.UsageMax);
                    }else{
                        std::printf("Failed range\n");
                    }
                }
            }

            //Value caps
            HIDP_VALUE_CAPS valueCaps[32];
            u_short valueCapsLength = caps.NumberInputValueCaps;
            if(HidP_GetValueCaps(HidP_Input, valueCaps, &valueCapsLength, preparseData) == HIDP_STATUS_INVALID_PREPARSED_DATA){
                std::printf("Couldn't get value caps\n");
                return false;
            }

            for(uint32_t i = 0; i < valueCapsLength; i++){
                if(valueCaps[i].UsagePage == 0x01){
                    if(valueCaps[i].IsRange){
                        std::printf("Value range: UsageMin = %u to UsageMax = %u\n", valueCaps[i].Range.UsageMin, valueCaps[i].Range.UsageMax);
                    }else{
                        std::printf("Value Usage: %u\n", valueCaps[i].NotRange.Usage);
                    }
                }
            }

            //this->preparseData = preparseData;
            //free(preparseData);
        }
    }
    delete[] deviceList;

    

    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x05;
    rid.dwFlags = RIDEV_INPUTSINK;
    rid.hwndTarget = hWnd;

    if(RegisterRawInputDevices(&rid, 1, sizeof(rid)) == false){
        std::printf("Couldn't register controller\n");
        return false;
    }

    return true;
}

bool Input::initInput(){
    if(!SDL_Init(SDL_INIT_GAMEPAD | SDL_INIT_EVENTS)){
        std::printf("Couldn't initialize SDL subsytem gamepad\n");
        return false;
    }

    if(!SDL_HasGamepad()){
        MessageBoxA(nullptr, "Please add a game controller", "Error", MB_OK);
        SDL_Quit();
        return false;
    }

    int count = 0;
    SDL_JoystickID *joystickID = SDL_GetGamepads(&count);
    this->gamepad = SDL_OpenGamepad(joystickID[0]);
    if(this->gamepad == NULL){
        std::printf("Couldn't open gamepad\n");
        SDL_Quit();
        return false;
    }

    return true;
}

void Input::reset(){
    std::vector<queue_t> temp;
    for(int currEvent = 0; currEvent < currQueue.size(); currEvent++){
        if(currQueue[currEvent].type == EventType::AXIS){
            temp.push_back(currQueue[currEvent]);
        }
    }
    prevQueue.clear();
    prevQueue = temp;
    currQueue.clear();
}

void Input::update(float frame_time){
    reset();
    SDL_UpdateGamepads();
    for(int currButton = 0; currButton < numButtons; currButton++){
        if(SDL_GetGamepadButton(gamepad, static_cast<SDL_GamepadButton>(buttons[currButton].buttonValue))){
            queue_t newEvent;
            newEvent.type = EventType::BUTTON;
            newEvent.eventSubType.button = buttons[currButton].buttonValue;
            newEvent.eventTypeName = buttons[currButton].buttonName;
            newEvent.value = 0;
            newEvent.timeAxisHeld = 0;
            currQueue.push_back(newEvent);
        }
    }
    
    for(int currAxis = 0; currAxis < numAxis; currAxis++){
        Sint16 value = SDL_GetGamepadAxis(gamepad, static_cast<SDL_GamepadAxis>(axis[currAxis].axis));
        if(std::abs(value) >= movingThreshold){
            bool existingEvent = false;
            for(int i = 0; i < prevQueue.size(); i++){
                if(prevQueue[i].type == EventType::AXIS){
                    if(prevQueue[i].eventSubType.axis == axis[currAxis].axis){
                        existingEvent = true;
                        queue_t newEvent = prevQueue[i];
                        newEvent.value = value;
                        newEvent.timeAxisHeld += frame_time;
                        currQueue.push_back(newEvent);
                        break;
                    }
                }
            }

            if(!existingEvent){
                queue_t newEvent;
                newEvent.type = EventType::AXIS;
                newEvent.eventSubType.axis = axis[currAxis].axis;
                newEvent.eventTypeName = axis[currAxis].axisName;
                newEvent.value = value;
                newEvent.timeAxisHeld = 0.0f;    
                currQueue.push_back(newEvent);
            }
        }
    }

    for(int i = 0; i < currQueue.size(); i++){
        if(currQueue[i].type == EventType::BUTTON){
            std::printf("Queue %d: event: %s(subtype: %d) with value %d\n", i, currQueue[i].eventTypeName, currQueue[i].eventSubType.button, currQueue[i].value);
        }else{
            std::printf("Queue %d: event: %s(subtype: %d) with value %d and held for %f\n", i, currQueue[i].eventTypeName, currQueue[i].eventSubType.axis, currQueue[i].value, currQueue[i].timeAxisHeld);
        }
    }
}

void Input::cleanInput(){
    if(this->gamepad) SDL_CloseGamepad(this->gamepad);
    SDL_Quit();
}