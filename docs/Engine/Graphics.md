# Graphics.h

Graphics struct responsible for all of the comunication with the GPU.  
Uses the third party library [Vulkan SDK](https://vulkan.lunarg.com/sdk/home).  
Uses the third party library [SDL3](https://github.com/libsdl-org/SDL). 

## Member Types
### `typedef enum Graphics_Errors Graphics_Errors`
Enum that provides additional information regarding as to where the creation of the graphics structure might have failed.

| Members | Description | Value | 
|--------|------|-------------|
| `GRAPHICS_ERROR_NONE` | No errors. Graphics were correctly intialized | `0` |
| `GRAPHICS_ERROR_ENABLE_DEBUG_MODE` | Couldn't enable debug mode | `1` |
| `GRAPHICS_ERROR_INITIALIZE_DEBUG_MESSENGER` |Couldn't initialize the Vulkan Debug Messenger| `2` |
| `GRAPHICS_ERROR_INITIALIZE_INSTANCE` | Couldn't initialize the instance | `10` |
| `GRAPHICS_ERROR_INITIALIZE_SDL_SURFACE` | Couldn't create the surface | `20` |
| `GRAPHICS_ERROR_INITIALIZE_DEVICE_QUEUE_HANDLE` | Couldn't initialize the device and get the appropriate queue | `30` |
| `GRAPHICS_ERROR_INITIALIZE_SWAPCHAIN` | Couldn't initialize the Swapchain | `40` |
| `GRAPHICS_ERROR_INITIALIZE_IMAGE_IMAGE_VIEWS` | Couldn't initialize the Images and the appropriate Image Views | `50` |
| `GRAPHICS_ERROR_INITIALIZE_GRAPHICS_PIPELINE` | Couldn't initialize the graphics pipeline | `60` |
| `GRAPHICS_ERROR_INITIALIZE_FRAME_BUFFERS` | Couldn't create the frame buffers | `70` |
| `GRAPHICS_ERROR_INITIALIZE_SYNC_OBJECTS` | Couldn't create the Vulkan syncing objects | `80` |
| `GRAPHICS_ERROR_INITIALIZE_COMMAND_POOL_BUFFER` | Couldn't initialize the command pool and the appropriate command buffers | `90` |

### `typedef struct Graphics_t Graphics`
Opaque pointer to the `Graphics_t` structure. Any GPU required action must use this pointer.


### Functions
#### Function: `const char *graphics_error_str(const Graphics_Errors error)` 
- **Parameters:**  `const Graphics_Errors error` - Graphics errors.  
- **Return:**  `const char *`   
- **Description**: Returns a valid string of the appropriate error given.

#### Function: `Graphics *initialize_graphics(SDL_Window *const window, str_array *extensions, Graphics_Errors *error)`   
- **Parameters:**  
    - `[in]SDL_Window *const window` - A valid pointer to a SDL window instance. 
    - `[in]str_array *extensions` - A valid pointer to a `str_array` of Vulkan extensions to be enabled.
    - `[in, out]Graphics_Errors *error` - A pointer to `Graphics_Errors` if needed more detailed information.
- **Return:**  `Graphics *`   
- **Description**: Creates and initializes the `Graphics` structure and returns a valid pointer to it. Returns `NULL` in case of failure. 

#### Function: `void temp_render_function(Graphics *gfx)`  
- **Parameters:** `[in]Graphics *gfx` - A valid pointer to the `Graphics` structure.
- **Return:**  `void`   
- **Description**: Temporary function to display a simple triangle.

#### Function: `void close_graphics(Graphics *gfx)` 
- **Parameters:**  `Graphics *gfx` - A valid pointer to the `Graphics` structure.  
- **Return:**  `void`   
- **Description**: Closes graphics and frees all of the resources used.

## Dependencies
- [SDL3](https://github.com/libsdl-org/SDL)