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
    bool initialize_graphics(SDL_Window *window, Core::n_vector<const char *> extensions);
    void close_graphics(void);

private:
    //Debug utilities needed
    Core::n_vector<const char *> get_validation_layers(void);
    void add_debug_extension(Core::n_vector<const char *> &extensions);
    
    //Instance
    bool initialize_instance(Core::n_vector<const char *> extensions, Core::n_vector<const char *> layers);

    // Debug initializer
    bool initialize_debug_messenger(void);
    
    //Surface
    bool initialize_surface(SDL_Window *window);
    
    //Device
    VkPhysicalDevice get_best_device(void);
    void choose_queue_family(const VkPhysicalDevice device, uint32_t *chosen_index, uint32_t *chosen_count);
    bool get_device_extensions(const VkPhysicalDevice device, Core::n_vector<const char *> *extensions, uint32_t *num_extensions);
    bool validate_device_surface(const VkPhysicalDevice device, const uint32_t family_index);
    bool add_device_features(VkPhysicalDeviceFeatures *features, const VkPhysicalDevice device, uint32_t num_features, ...);
    bool initialize_device(void);
    
    /*Close Graphics*/
    void destroy_device(void);
    void destroy_surface(void);
    void close_debug_messenger(void);
    
    VkInstance vk_instance;
    VkDebugUtilsMessengerEXT messenger;
    VkSurfaceKHR vk_surface;
    VkDevice logical_device;
};

#endif