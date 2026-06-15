# window.h

Window class responsible for creating and destroying the window, as well as initializing the graphics.  
The window created is displayed on the main monitor.  
Uses the third party library [SDL3](https://github.com/libsdl-org/SDL).  

## Member Types 

### `properties_t` 

Structure containing the display information.

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `id` | `uint32_t` |Window Id| `0` |
| `name` | `const char *` |Name of the window| `nullptr` |
| `n_math::rect` | `bounds` | Bounds of the display | `0` |
| `n_math::rect` | `usable_bounds` |Usable bounds of the display. Disregards things like taskbar| `0` |
| `scale` | `float` |Scale of the display| `0.0f` |
| `pixel_density` | `float` |Pixel density of the display| `0.0f` |
| `refresh_rate` | `float` |Refresh rate of the display| `0.0f` |

## Member Variables
### Instance variables

| Members | Type | Access | Description | Default Value |
|--------|------|---------|-------------|-------------|
| `window` | `SDL_Window*` | Private |Pointer to SDL_Window| `nullptr` |
| `display_properties` | `propeties_t` | Private |Containts essential display properties| `Zero` |
| `extensions_data` | `str_array` | Pointer to `str_array` that contains the extensions strings| Private | `NULL`| 

### Static variables
None.


## Function Members
### Instance Functions

#### Function: `Window()` 
- **Access:** `Public`
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description:** Initializes Window instance along with all of it's variables.

#### Function: `~Window()`
- **Access:** `Public`  
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description:** Destroys the Window instance.

#### Function: `bool create_window(const char *window_name)`  
- **Access:** `Public`
- **Parameters:**  `[in]const char *window_name` - Name of the window.
- **Return:**  `bool`   
- **Description:** Gets the main display information for `display_properties` and creates window. Also initializes the [graphics](Graphics.md)

#### Function: `str_array *get_extensions(void) const`
- **Access:** `Public`
- **Parameters:** `void`
- **Return:**  `str_array *`   
- **Description:** Returns a structure of strings that holds the Vulkan extensions.  

#### Function: `SDL_Window *get_window_handle(void) const`
- **Access:** `Public`
- **Parameters:** `void`
- **Return:**  `SDL_Window *`   
- **Description:** Returns a pointer to the SDL window handle.  

#### Function: `void destroy_window(void)`
- **Access:** `Public`
- **Parameters:** `void`
- **Return:**  `void`   
- **Description:** Destroys the window and frees the `window` variable. 

### Static Functions
None.

## Dependencies

- [SDL3](https://github.com/libsdl-org/SDL)
- [debug_helper](../core/debug_helper.md)
- [rect](../Math_Lib/Rect.md)
- [str_array](../core/str_array.md)