# SDL_SubSystem.h

SDL_SubSystem class that initializes and quits `SDL`.  
Uses the third party library [SDL3](https://github.com/libsdl-org/SDL).  

## Member Types 
None

## Member Variables
None

## Function Members
### Instance Functions

#### Function: `SDL_SubSystem()` 
- **Access:** `Public` 
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Creates a SDL_SubSystem instance. Initializes SDL with the necessary flags.

#### Function: `~SDL_SubSystem()`
- **Access:** `Public`  
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Destroys the SDL_SubSystem instance and quits SDL.

### Static Functions
None.

## Dependencies

- [SDL3](https://github.com/libsdl-org/SDL)
