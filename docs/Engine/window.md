# window.h

Window class responsible for creating and destroying the window, as well as update and rendering the game.  
The window created is displayed on the main monitor.  
Uses the third party library [SDL3](https://github.com/libsdl-org/SDL).  

## Member Types 

### `properties_t` 

Structure containing the display information.

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `id` | `uint32_t` |Window Id| `0` |
| `name` | `const char *` |Name of the window| `nullptr` |
| `n_rect` | `bounds` | Bounds of the display | `0` |
| `n_rect` | `usable_bounds` |Usable bounds of the display. Disregards things like taskbar| `0` |
| `scale` | `float` |Scale of the display| `0.0f` |
| `pixel_density` | `float` |Pixel density of the display| `0.0f` |
| `refresh_rate` | `float` |Refresh rate of the display| `0.0f` |

## Member Variables
### Instance variables

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `window` | `SDL_Window*` |Pointer to SDL_Window| `nullptr` |
| `display_properties` | `propeties_t` |Containts essential display properties| `Zero` |

### Static variables
None.


## Function Members
### Instance Functions

#### Function: `Window()`  
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Initializes Window instance along with all of it's variables.

#### Function: `~Window()`  
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Destroys the Window instance.

#### Function: `bool create_window(const char *window_name)`  
- **Parameters:**  `[in]const char *window_name`  
- **Return:**  `bool`   
- **Description**: Gets the main display information for `display_properties` and creates window. 

#### Function: `bool should_render_window(void)`
- **Parameters:** `void`
- **Return:**  `bool`   
- **Description**: Gets a vector of all of the messages related to the window. If any of the messages are of the type [Keyboard_Escape](input.md#typedef-enum-events-event) or [Quit](input.md#typedef-enum-events-event) it returns false. Returns true otherwise.  

#### Function: `void update_window(float frame_time, Input *input)`
- **Parameters:** 
    - `float frame_time`
    - `Input *input` 
- **Return:**  `void`   
- **Description**: Updates Window.  

#### Function: `void render_window(void)`
- **Parameters:** `void` 
- **Return:**  `void`   
- **Description**: Renders Window.

#### Function: `void destroy_window(void)`
- **Parameters:** `void`
- **Return:**  `void`   
- **Description**: Destroys the window and frees the `window` variable. 

### Static Functions
None.

## Dependencies

- [SDL3](https://github.com/libsdl-org/SDL)
- [input.h](input.md)