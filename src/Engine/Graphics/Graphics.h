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

#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

class Graphics{
public:
    Graphics();
    ~Graphics();
    bool initialize_graphics(char const * const * array_extensions, uint32_t num_extensions);
    void close_graphics(void);

private:
    VkInstance vk_instance;
};

#endif