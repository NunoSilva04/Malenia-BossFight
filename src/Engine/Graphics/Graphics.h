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
    typedef struct Physical_Device_Information_t{
        VkPhysicalDevice device;
        uint32_t queue_index;
        uint32_t queue_count;
        uint32_t queue_count_used;
        Core::n_vector<const char *> extensions_names;
        uint32_t extension_count;
        VkPhysicalDeviceFeatures features;
    }Physical_Device_Info;

    typedef struct Swapchain_Info_t{
        // Device Surface Capabilities
        uint32_t min_image_count;
        uint32_t max_image_count;
        VkExtent2D curr_image_extent;
        uint32_t max_image_array_layers;
        uint32_t defined_image_array_layers;
        VkImageUsageFlags image_usage_flags;
        VkSurfaceTransformFlagBitsKHR surface_transform;
        VkCompositeAlphaFlagBitsKHR alpha_flags;
        // Device Surface Formats
        VkFormat format;
        VkColorSpaceKHR color_space;
        // Device Surface Present
        VkPresentModeKHR present_mode;
        // Additional Information
        uint32_t queue_family_count_used;
        uint32_t queue_family_index;
        VkBool32 is_clipped;      
    }Swapchain_Info;

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
    bool get_best_device(void);
    bool choose_queue_family(void);
    bool get_device_extensions(void);
    bool is_device_supported(void);
    bool add_device_features(uint32_t num_features, ...);
    bool initialize_device_get_queue_handle(void);
    
    //Swapchain and Image Views
    friend bool get_device_format_color_space(VkSurfaceFormatKHR *surface_formats, uint32_t num_formats, VkFormat desired_format, Graphics::Swapchain_Info *swapchain_info);
    bool get_swapchain_info(void);
    bool create_swapchain(void);
    bool create_images_and_image_views(void);

    // Command Pools and Bufffers
    bool create_command_pool(void);
    bool create_command_buffer(void);


    // Close Graphics 
    void destroy_command_pool(void);
    void destroy_images_and_image_views(void);
    void destroy_swapchain(void);
    void destroy_device(void);
    void destroy_surface(void);
    void close_debug_messenger(void);

private:
    // Instance
    VkInstance vk_instance;
    
    // Debug
    VkDebugUtilsMessengerEXT messenger;
    
    // Surface
    VkSurfaceKHR vk_surface;
    
    // Logical Device
    Physical_Device_Info physical_device_info;
    VkDevice logical_device;
    VkQueue logical_device_queue;
    
    // Swapchain and Image Views
    Swapchain_Info swapchain_info;
    VkSwapchainKHR swapchain;
    Core::n_vector<VkImage> images;
    Core::n_vector<VkImageView> image_views;
};

#endif