# input.h

Input class that handles specific inputs from the keyboard and controller.  
Uses the third party library [SDL3](https://github.com/libsdl-org/SDL).

## Member Types

### `typedef enum Events Event`

Enumerates all events handled: 

| Events         | Description         |
| ---------------|---------------|
| Keyboard_Escape| Triggered when Keyboard Escape key is pressed  |
| Keyboard_F11| Triggered when Keyboard F11 key is pressed  |
| Gamepad_Circle | Triggered when Controller East Button is pressed |
|    Gamepad_X |Triggered when Controller South Button is pressed|
| Gamepad_Square | Triggered when Controller West Button is pressed|
| Gamepad_Triangle | Triggered when Controller North Button is pressed|

### `typedef enum Listeners_Id Listeners_Id`

Enumerates all the possible listeners to dispatch message inputs.

| Listeners_Id  | Description         |
| ---------------|---------------|
| Window_Id| Window System  |

## Member Variables
### Instance variables

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `gamepad` | `SDL_Gamepad*` |Pointer to SDL_Gamepad| `nullptr` |


### Static variables

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `event_vector` | `static Core::n_vector<Event>`| Vector that holds all input events for the current frame | `empty` |

## Member Functions

### Instance Functions

#### Function: `Input()` 
- **Access:** `Public`  
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Initializes Input instance and sets `gamepad` to nullptr.

#### Function: `~Input()`
- **Access:** `Public`  
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Destroys the Input instance.

#### Function: `bool initialize_input(void)`  
- **Access:** `Public`
- **Parameters:**  `void`  
- **Return:**  `bool`   
- **Description**: Initializes `gamepad`. If Debug is enabled and no controller is connected it will continue running, returns false and exits the program otherwise. 

#### Function: `void update_input(void)`
- **Access:** `Public`  
- **Parameters:**  `void`  
- **Return:**  `void`   
- **Description**: Empties the `event_vector` and polls for all other possible events pushing them into `event_vector`.

#### Function: `void close_input(void)`
- **Access:** `Public`  
- **Parameters:**  `void`  
- **Return:**  `void`   
- **Description**: Releases and closes the `gamepad` instance.

### Static Functions 

#### Function: `Core::n_vector<Event> dispatch_input(Listeners_Id listener)`  
- **Access:** `Public`
- **Parameters:**  `[in]Listeners_Id listener` - The type of listener.
- **Return:**  `Core::n_vector<Event>`   
- **Description**: Creates a new event queue and fills that event queue with the messages needed for the listener. If there are no messages for the listener or if `event_vector` is empty it simply returns a empty vector.

## Dependencies
- [SDL3](https://github.com/libsdl-org/SDL)
- [n_vector](../core/n_vector.md)
- [debug_helper](../core/debug_helper.md)