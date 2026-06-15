/**********************************************************************************
*   File: Graphics_Helper.h
* 
*   Description: 
*        Helper functions mostly used for debugging and printing vulkan specifications.
*        Uses Vulkan.
* 
*   See: 
*       docs/Engine/Graphics_Helper.md 
*       for detailed documentation.
* 
*************************************************************************************/

#ifndef __GRAPHICS_HELPER_H__
#define __GRAPHICS_HELPER_H__

#include <vulkan/vulkan.h>
#include "core.h"
#include "str_array.h"

namespace vk_debug{
    bool enable_debug_mode(str_array *layers, str_array *extensions);
    bool initialize_debug_messenger(const VkInstance instance);
    void destroy_debug_messenger(const VkInstance instance);
};

namespace vk_helper{
    typedef enum Available_Features{
        Geometry_Shader,
    }Available_Features;

    typedef enum Available_Alpha_Flags{
        ALPHA_FLAG_OPAQUE,
    }Available_Alpha_Flags;

    typedef enum Available_Usage_Flags{
        USAGE_FLAG_COLOR_ATTACHMENT,
    }Available_Usage_Flags;

    const char *object_type_to_str(const VkObjectType type);
    const char *severity_to_str(VkDebugUtilsMessageSeverityFlagBitsEXT severity);
    const char *msg_type_to_str(const VkDebugUtilsMessageTypeFlagsEXT message_types);
    const char *vulkan_boolean_to_str(VkBool32 value);
    void version_to_str(uint32_t version, char *out_string, size_t string_size);
    const char *device_type_to_str(VkPhysicalDeviceType type);
    void print_physical_device_properties(const VkPhysicalDeviceProperties properties);
    void print_physical_device_memory_properties(const VkPhysicalDeviceMemoryProperties memory_properties);
    void queue_flags_to_str(const VkQueueFlags flags, char *out_string, size_t string_size, int * const num_flags);
    void print_queue_family_properties(uint32_t family_number, const VkQueueFamilyProperties queue_family_properties);
    void enumerate_features(const VkPhysicalDeviceFeatures features);
    bool is_feature_enabled(const VkPhysicalDeviceFeatures features, Available_Features feature_to_enable);
    void enable_feature(VkPhysicalDeviceFeatures *features, Available_Features feature_to_enable);
    void transform_flags_to_str(const VkSurfaceTransformFlagsKHR flags, char *out_string, size_t string_size);
    const char *get_current_transform_flag(const VkSurfaceTransformFlagBitsKHR flag);
    void alpha_flags_to_str(const VkCompositeAlphaFlagsKHR flags, char *out_string, size_t string_size);
    void usage_flags_to_str(const VkImageUsageFlags flags, char *out_string, size_t string_size);
    void enumerate_device_surface_capabilities(const VkSurfaceCapabilitiesKHR surface_capabilities);
    bool choose_alpha_flag(VkCompositeAlphaFlagBitsKHR *alpha_flags, Available_Alpha_Flags flag_to_add);
    bool choose_usage_flag(VkImageUsageFlags *usage_flags, Available_Usage_Flags chosen_flag);
    void enumerate_device_surface_formats(VkSurfaceFormatKHR *surface_formats, uint32_t num_formats);
    const char *present_mode_to_str(const VkPresentModeKHR flag);
    void enumerate_device_present_modes(const VkPresentModeKHR *present_modes, const uint32_t num_present_modes);
};

#endif