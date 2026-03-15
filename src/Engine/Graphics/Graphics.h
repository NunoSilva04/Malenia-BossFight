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
#include <core.h>

class Graphics{
public:
    Graphics();
    ~Graphics();
    bool initialize_graphics(Core::n_vector<const char *> extensions);
    void close_graphics(void);

private:
    Core::n_vector<const char *> get_validation_layers(void);
    void add_debug_extension(Core::n_vector<const char *> extensions);
    bool initialize_instance(Core::n_vector<const char *> extensions, Core::n_vector<const char *> layers);
    bool initialize_debug_messenger(void);
    VkInstance vk_instance;
    VkDebugUtilsMessengerEXT messenger;
};

#endif