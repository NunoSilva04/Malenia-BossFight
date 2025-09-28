#include "Input.h"

Sint16 Input::getAxisValue(controllerAxis axis){
    for(int currEvent = 0; currEvent < currQueue.size(); currEvent++){
        if(currQueue[currEvent].type == EventType::AXIS){
            if(currQueue[currEvent].eventSubType.axis == axis){
                return currQueue[currEvent].value;
            }
        }
    }
    return 0;
}