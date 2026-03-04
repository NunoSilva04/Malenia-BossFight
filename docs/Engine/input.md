# input.h

Input class that handles specific inputs from the keyboard and controller. Uses the third party library `SDL3`.

## enum Events

Enumerates all events handled:

`typedef enum Events Event` 

| Events         | Description         |
| ---------------|---------------|
| Keyboard_Escape| Triggered when Keyboard Escape key is pressed  |
| Gamepad_Circle | Triggered when Controller East Button is pressed |
|    Gamepad_X |Triggered when Controller South Button is pressed|
| Gamepad_Square | Triggered when Controller West Button is pressed|
| Gamepad_Triangle | Triggered when Controller North Button is pressed|

## enum Listeners_Id

Enumerates all the possible listeners to dispatch message inputs.

`typedef enum Listeners_Id Listeners_Id`

| Listeners_Id  | Description         |
| ---------------|---------------|
| Window_Id| Window System  |

## Member Types 
### Instance Members

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `gamepad` | `SDL_Gamepad*` |Pointer to SDL_Gamepad| `nullptr` |


### Static members

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `event_vector` | `static Core::n_vector<Event>`| Vector that holds all input events for the current frame | `empty` |

## Function Members

### Instance Functions

#### Function: `Input()`  
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Initializes Input instance and sets `gamepad` to nullptr.

#### Function: `~Input()`  
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Destroys the Input instance.

#### Function: `bool initialize_input(void)`  
- **Parameters:**  `void`  
- **Return:**  `bool`   
- **Description**: Initializes `gamepad`. If Debug is enabled and no Controller is connected, it will automatically return false. Returns true otherwise.

#### Function: `void update_input(void)`  
- **Parameters:**  `void`  
- **Return:**  `void`   
- **Description**: Empties the `event_vector` and polls for all other possible events pushing them into `event_vector`.

#### Function: `void close_input(void)`  
- **Parameters:**  `void`  
- **Return:**  `void`   
- **Description**: Releases and closes the `gamepad` instance.

### Static Functions 

#### Function: `Core::n_vector<Event> dispatch_input(Listeners_Id listener)`  
- **Parameters:**  `Listeners_Id listener`  
- **Return:**  `Core::n_vector<Event>`   
- **Description**: Creates a new event queue and fills that event queue with the messages needed for the listener. If there are no messages it simply returns a empty vector.

## Dependencies
- [SDL3](https://github.com/libsdl-org/SDL)
- [n_vector](../core/n_vector.md)