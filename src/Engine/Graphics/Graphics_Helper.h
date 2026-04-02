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

namespace vk_debug{
    bool get_validation_layers(Core::n_vector<const char *> &layers);
    void add_debug_extension(Core::n_vector<const char *> &extensions);
    bool initialize_debug_messenger(const VkInstance vk_instance);
    void create_object_name_fn(const VkDevice logical_device);
    void set_object_name(const VkDevice logical_device, VkObjectType type, uint64_t handle, const char *name);
    void destroy_debug_messenger(const VkInstance vk_instance);
};

namespace vk_helper{
    // Features enabled of the device
    enum CoreFeatures{
        Geometry_Shader,
        Tesselation_Shader,
    };

    // Usage Flags wanted
    enum Usage_Flags{
        COLOR_ATTACHMENT,
        TRANSFER_DESTINATION,
    };

    // Alpha flags available
    enum Alpha_Flags{
        OPAQUE, 
        PRE_MULTIPLED,
        POST_MULTIPLED,
        INHERITED,
    };

    const char *object_type_to_str(const VkObjectType type);
    const char *severity_to_str(VkDebugUtilsMessageSeverityFlagBitsEXT severity);
    const char *msg_type_to_str(const VkDebugUtilsMessageTypeFlagsEXT message_types);
    const char *vulkan_boolean_to_str(VkBool32 value);
    void version_to_str(uint32_t version, char *out_string, size_t string_size);
    const char *device_type_to_str(VkPhysicalDeviceType type);
    void queue_flags_to_str(const VkQueueFlags flags, char *out_string, size_t string_size, int * const num_flags);
    void transform_flags_to_str(const VkSurfaceTransformFlagsKHR flags, char *out_string, size_t string_size);
    const char *get_current_transform_flag(const VkSurfaceTransformFlagBitsKHR flag);
    void alpha_flags_to_str(const VkCompositeAlphaFlagsKHR flags, char *out_string, size_t string_size);
    void usage_flags_to_str(const VkImageUsageFlags flags, char *out_string, size_t string_size);
    const char *present_mode_to_str(const VkPresentModeKHR flag);
    void enumerate_available_features(const VkPhysicalDeviceFeatures features);
    bool device_has_feature(const VkPhysicalDeviceFeatures available_features, const CoreFeatures current_feature);
    void device_enable_feature(VkPhysicalDeviceFeatures *features, const CoreFeatures feature_to_enable);
    void enumerate_device_capabilities(const VkSurfaceCapabilitiesKHR capabilities);
    VkImageUsageFlags get_swapchain_usage_flags(const VkImageUsageFlags flags, const uint32_t num_flags, ...);
    VkCompositeAlphaFlagBitsKHR get_swapchain_alpha_flag(Alpha_Flags desired_flag);
    void enumerate_device_formats(VkSurfaceFormatKHR *surface_formats, uint32_t num_formats);
    bool get_device_format_color_space(VkSurfaceFormatKHR *surface_formats, uint32_t num_formats, VkFormat desired_format, VkFormat &format, VkColorSpaceKHR &color_space);
    void enumerate_device_present_modes(const VkPresentModeKHR *present_modes, const uint32_t num_present_modes);
    VkPresentModeKHR get_present_mode(const VkPresentModeKHR *present_modes, const uint32_t num_present_modes, VkPresentModeKHR desired_preset);
    bool create_shader_module(const VkDevice logical_device, VkShaderModule *shader_module, const char *file_name);
    void destroy_shader_module(const VkDevice logical_device, VkShaderModule shader_module);
    VkPipelineShaderStageCreateInfo create_shader_stage_info(VkShaderStageFlagBits stage_flag, VkShaderModule module);
    VkPipelineDynamicStateCreateInfo create_dynamic_state_info(uint32_t num_states);
    VkPipelineVertexInputStateCreateInfo create_vertex_state_info(void);
    VkPipelineInputAssemblyStateCreateInfo create_assembly_state_info(void);
    VkPipelineViewportStateCreateInfo create_viewport_state_info(const VkExtent2D extent);
    VkPipelineRasterizationStateCreateInfo create_rasterizer_state_info(void);
    VkPipelineColorBlendStateCreateInfo create_color_blend_state_info(bool enabled);
    VkPipelineMultisampleStateCreateInfo create_multisample_state_info(void);
};

#endif