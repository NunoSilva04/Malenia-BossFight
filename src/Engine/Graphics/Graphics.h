/**********************************************************************************
*   File: Graphics.h
* 
*   Description: 
*        Responsible for computing and rendering all of the graphics. Uses Vulkan.
* 
*   See: 
*       docs/Engine/Graphics.md 
*       for detailed documentation.
* 
*************************************************************************************/

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <SDL3/SDL.h>
#include "str_array.h"

typedef enum Graphics_Errors{
    GRAPHICS_ERROR_NONE = 0,
    
    GRAPHICS_ERROR_ENABLE_DEBUG_MODE = 1,
    GRAPHICS_ERROR_INITIALIZE_DEBUG_MESSENGER = 2,

    GRAPHICS_ERROR_INITIALIZE_INSTANCE = 10,

    GRAPHICS_ERROR_INITIALIZE_SDL_SURFACE = 20,

    GRAPHICS_ERROR_INITIALIZE_DEVICE_QUEUE_HANDLE = 30,

    GRAPHICS_ERROR_INITIALIZE_SWAPCHAIN = 40,

    GRAPHICS_ERROR_INITIALIZE_IMAGE_IMAGE_VIEWS = 50,

    GRAPHICS_ERROR_INITIALIZE_GRAPHICS_PIPELINE = 60,

    GRAPHICS_ERROR_INITIALIZE_FRAME_BUFFERS = 70,

    GRAPHICS_ERROR_INITIALIZE_SYNC_OBJECTS = 80,

    GRAPHICS_ERROR_INITIALIZE_COMMAND_POOL_BUFFER = 90,
}Graphics_Errors;

typedef struct Graphics_t Graphics;

const char *graphics_error_str(const Graphics_Errors error);
Graphics *initialize_graphics(SDL_Window *const window, str_array *extensions, Graphics_Errors *error);
void temp_render_function(Graphics *gfx);
void close_graphics(Graphics *gfx);

#endif