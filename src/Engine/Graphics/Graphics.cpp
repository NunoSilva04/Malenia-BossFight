#include "Graphics.h"
#include <stdlib.h>
#include <stdarg.h>
#include <vulkan/vulkan.h>
#include <SDL3/SDL_vulkan.h>
#include "Graphics_Helper.h"
#include "file_system.h"

#define MAX_FRAME_IN_FLIGHT 2

typedef struct{
    VkDevice logical_device;
    VkQueue logical_device_queue;
    VkPhysicalDevice physical_device;
    uint32_t queue_index;
    uint32_t num_queues;
    uint32_t num_queues_in_use;
    str_array *extensions;
    VkPhysicalDeviceFeatures features;
}Device_Info_t;

typedef struct{
    VkSwapchainKHR swapchain;
    // Device Surface Capabilites
    uint32_t min_image_count;
    uint32_t max_image_count;
    uint32_t num_images_used;
    VkExtent2D min_image_extent;
    VkExtent2D max_image_extent;
    VkExtent2D curr_image_extend;
    uint32_t max_image_array_layers;
    uint32_t num_image_array_layers_used;
    VkSurfaceTransformFlagsKHR supported_transforms;
    VkSurfaceTransformFlagBitsKHR chosen_transform;
    VkCompositeAlphaFlagsKHR supported_alpha_flags;
    VkCompositeAlphaFlagBitsKHR chosen_alpha_flag;
    VkImageUsageFlags supported_usage_flags;
    VkImageUsageFlags chosen_usage_flags;
    // Device Surface Formats
    VkFormat format;
    VkColorSpaceKHR color_space;
    // Device Surface Present Mode
    VkPresentModeKHR present_mode;
}Swapchain_Info_t;

typedef struct{
    uint32_t num_images;
    VkImage *images;
    VkImageView *image_view;
    VkFramebuffer *frame_buffers;
}Image_Info; 

enum{
    SHADER_TYPE_VERTEX,
    SHADER_TYPE_FRAGMENT,
};

typedef struct{
    VkShaderModule shader_module;
    VkPipelineShaderStageCreateInfo shader_stage_create_info;
    int shader_type;
}Shader_Data;

typedef struct{
    uint32_t num_shaders;
    Shader_Data *data;
}Shader_Info;

typedef struct{
    VkPipelineLayout pipeline_layout;
    VkRenderPass render_pass;
    VkPipeline pipeline;
}Graphics_Pipeline_Info;

typedef struct{
    uint32_t num_sync_objects;
    VkSemaphore *image_available_semaphores;
    VkSemaphore *render_finished_semaphores;
    VkFence *fences;
}Sync_Objects_Info;

typedef struct{
    uint32_t num_command_pools;
    VkCommandPool *command_pools;
    uint32_t num_command_buffers;
    VkCommandBuffer *command_buffers;
}Commands_Info;

typedef struct Graphics_t{
    VkInstance instance;
    VkSurfaceKHR surface;
    Device_Info_t device_info;
    Swapchain_Info_t swapchain_info;
    Image_Info image_info;
    Shader_Info shader_info;
    Graphics_Pipeline_Info graphics_pipeline_info;
    Sync_Objects_Info sync_objects_info;
    Commands_Info commands_info;
}Graphics;

static bool initialize_instance(Graphics *gfx, str_array *layers, str_array *extensions);
static bool initialize_surface(Graphics *gfx, SDL_Window *window);
static bool get_best_device(const VkInstance instance, Device_Info_t *device_info);
static bool choose_queue_family(Device_Info_t *device_info, VkQueueFlags queue_family_flag, uint32_t num_queues_desired);
static bool get_device_extensions(Device_Info_t *device_info);
static bool is_device_supported(const Device_Info_t device_info, VkSurfaceKHR surface);
static bool add_device_features(Device_Info_t *device_info, uint32_t num_features, ...);
static bool initialize_device_get_queue_handle(Graphics *gfx);
static bool get_swapchain_surface_capabilities(Graphics *gfx);
static bool get_swapchain_device_formats(Graphics *gfx);
static bool get_swapchain_present_mode(Graphics *gfx);
static bool initialize_swapchain(Graphics *gfx);
static bool initialize_image_views(Graphics *gfx);
static bool create_shader_module(const VkDevice logical_device, Shader_Data *shader_data, const char *file_path);
static bool create_shader_stage_info(Shader_Data *shader_data, int shader_type);
static VkPipelineVertexInputStateCreateInfo create_vertex_input_state_info(void);
static VkPipelineInputAssemblyStateCreateInfo create_assembly_state_create_info(VkPrimitiveTopology topology);
static VkPipelineViewportStateCreateInfo create_viewport_state_create_info(const VkExtent2D curr_image_extent);
static VkPipelineRasterizationStateCreateInfo create_rasterizer_state_info(bool enable_clamp, bool enable_rasterizer_discard, VkPolygonMode polygon_mode, VkCullModeFlags cull_flags, VkFrontFace face_orientation, float line_width);
static VkPipelineMultisampleStateCreateInfo create_multisample_state_create_info(uint32_t num_samples, bool enable_shading, float min_sample_shading_fraction);
static VkPipelineColorBlendStateCreateInfo create_color_blend_state_create_info(bool enable_color_blending);
static VkPipelineDynamicStateCreateInfo create_dynamic_state_create_info(bool tesselation_enabled, bool depth_stencil_enabled);
static bool create_graphics_pipeline_layout(const VkDevice logical_device, VkPipelineLayout *pipeline_layout);
static bool create_render_pass(const VkDevice logical_device, const Swapchain_Info_t swapchain_info, VkRenderPass *render_pass);
static bool create_graphics_pipeline(Graphics *gfx);
static bool create_frame_buffers(Graphics *gfx);
static bool create_sync_objects(Graphics *gfx);
static bool create_command_pool_buffer(Graphics *gfx, uint32_t num_command_pools, uint32_t num_command_buffers);

static void record_command_buffer(Graphics *gfx, uint32_t curr_image_index, uint32_t curr_frame);

static void destroy_command_pool_buffer(Graphics *gfx);
static void destroy_sync_objects(Graphics *gfx);
static void destroy_frame_buffers(Graphics *gfx);
static void destroy_graphics_pipeline(Graphics *gfx);
static void destroy_image_view(Graphics *gfx);
static void destroy_swapchain(Graphics *gfx);
static void destroy_device(Graphics *gfx);
static void destroy_surface(Graphics *gfx);
static void destroy_instance(Graphics *gfx);

const char *graphics_error_str(const Graphics_Errors error){
    const char *message = "<unnamed>";

    switch(error){
        case GRAPHICS_ERROR_NONE:
            message = "None";
        break;

        case GRAPHICS_ERROR_ENABLE_DEBUG_MODE:
            message = "Enable Debug Mode";
        break;

        case GRAPHICS_ERROR_INITIALIZE_INSTANCE:
            message = "Initialize Instance";
        break;

        case GRAPHICS_ERROR_INITIALIZE_DEBUG_MESSENGER:
            message = "Initialize Debug Messenger";
        break;

        case GRAPHICS_ERROR_INITIALIZE_SDL_SURFACE:
            message = "Initialize SDL Surface";
        break;

        case GRAPHICS_ERROR_INITIALIZE_DEVICE_QUEUE_HANDLE:
            message = "Initialize Device and get Queue Handle";
        break;

        case GRAPHICS_ERROR_INITIALIZE_SWAPCHAIN:
            message = "Initialize Swapchain";
        break;

        case GRAPHICS_ERROR_INITIALIZE_IMAGE_IMAGE_VIEWS:
            message = "Initialize image and image views";
        break;

        case GRAPHICS_ERROR_INITIALIZE_GRAPHICS_PIPELINE:
            message = "Create Graphics Pipeline";
        break;

        case GRAPHICS_ERROR_INITIALIZE_FRAME_BUFFERS:
            message = "Create Frame Buffers";
        break;

        case GRAPHICS_ERROR_INITIALIZE_SYNC_OBJECTS:
            message = "Initialzie Sync Objects";
        break;

        case GRAPHICS_ERROR_INITIALIZE_COMMAND_POOL_BUFFER:
            message = "Initialize Command Pool and Buffers";
        break;
    }

    return message;
}

Graphics *initialize_graphics(SDL_Window *const window, str_array *extensions, Graphics_Errors *error){
    Graphics *gfx = (Graphics *) calloc(1, sizeof(Graphics));
    gfx->device_info.extensions = allocate_str_array(0);
    
    str_array *layers = allocate_str_array(0);
    if(!vk_debug::enable_debug_mode(layers, extensions)){
        *error = GRAPHICS_ERROR_ENABLE_DEBUG_MODE;
        return NULL;
    }
    if(!initialize_instance(gfx, layers, extensions)){
        *error = GRAPHICS_ERROR_INITIALIZE_INSTANCE;
        return NULL;
    }
    free_str_array(layers);
    if(!vk_debug::initialize_debug_messenger(gfx->instance)){
        *error = GRAPHICS_ERROR_INITIALIZE_DEBUG_MESSENGER;
        return NULL;
    }
    if(!initialize_surface(gfx, window)){
        *error = GRAPHICS_ERROR_INITIALIZE_SDL_SURFACE;
        return NULL;
    }
    if(!initialize_device_get_queue_handle(gfx)){
        *error = GRAPHICS_ERROR_INITIALIZE_DEVICE_QUEUE_HANDLE;
        return NULL;
    }
    if(!initialize_swapchain(gfx)){
        *error = GRAPHICS_ERROR_INITIALIZE_SWAPCHAIN;
        return NULL;
    }
    if(!initialize_image_views(gfx)){
        *error = GRAPHICS_ERROR_INITIALIZE_IMAGE_IMAGE_VIEWS;
        return NULL;
    }
    if(!create_graphics_pipeline(gfx)){
        *error = GRAPHICS_ERROR_INITIALIZE_GRAPHICS_PIPELINE;
        return NULL;
    }
    if(!create_frame_buffers(gfx)){
        *error = GRAPHICS_ERROR_INITIALIZE_FRAME_BUFFERS;
        return NULL;
    }
    if(!create_sync_objects(gfx)){
        *error = GRAPHICS_ERROR_INITIALIZE_SYNC_OBJECTS;
        return NULL;
    }
    if(!create_command_pool_buffer(gfx, 1, MAX_FRAME_IN_FLIGHT)){
        *error = GRAPHICS_ERROR_INITIALIZE_COMMAND_POOL_BUFFER;
        return NULL;
    }

    return gfx;
}

static bool initialize_instance(Graphics *gfx, str_array *layers, str_array *extensions){
    print_str_array(layers);
    print_str_array(extensions);
    VkApplicationInfo application_info {};
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pNext = NULL;
    application_info.pApplicationName = "BossFight";
    application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    application_info.pEngineName = "No engine";
    application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    application_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pNext = NULL;
#if defined(__APPLE__)
    create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#else
    create_info.flags = 0;
#endif
    create_info.pApplicationInfo = &application_info;
#ifdef DEBUG
    create_info.enabledLayerCount = layers->num_strings;
    create_info.ppEnabledLayerNames = layers->strings;
#else
    create_info.enabledLayerCount = 0;        
    create_info.ppEnabledLayerNames = NULL;
#endif
    create_info.enabledExtensionCount = extensions->num_strings;
    create_info.ppEnabledExtensionNames = extensions->strings;

    VkResult result = vkCreateInstance(&create_info, nullptr, &gfx->instance); 
    if(result != VK_SUCCESS){
        Core::debug::log(Core::debug::Fatal, "Couldn't create instance. Error value = %d\n", result);
        return false;
    }

    return true;
}

static bool initialize_surface(Graphics *gfx, SDL_Window *window){
    if(!SDL_Vulkan_CreateSurface(window, gfx->instance, NULL, &gfx->surface))
        return false;

    return true;
}

static bool get_best_device(const VkInstance instance, Device_Info_t *device_info){
    uint32_t num_physical_devices = 0;
    vkEnumeratePhysicalDevices(instance, &num_physical_devices, NULL);
    if(num_physical_devices == 0)
        return false;

    VkPhysicalDevice *physical_devices = (VkPhysicalDevice *) malloc(num_physical_devices * sizeof(VkPhysicalDevice));
    if(vkEnumeratePhysicalDevices(instance, &num_physical_devices, physical_devices) != VK_SUCCESS){
        free(physical_devices);
        return false;
    }

    VkPhysicalDevice temp_device;
    VkPhysicalDeviceProperties physical_device_properties;
    VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
    uint64_t max_vram = 0;
    bool has_dgpu = false;
    for(uint32_t i = 0; i < num_physical_devices; i++){
        vkGetPhysicalDeviceProperties(physical_devices[i], &physical_device_properties);
        vk_helper::print_physical_device_properties(physical_device_properties);
        vkGetPhysicalDeviceMemoryProperties(physical_devices[i], &physical_device_memory_properties);
        vk_helper::print_physical_device_memory_properties(physical_device_memory_properties);

        if(physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
            has_dgpu = true;
            if(physical_device_memory_properties.memoryHeaps->size > max_vram){
                temp_device = physical_devices[i];
                max_vram = physical_device_memory_properties.memoryHeaps->size;
            }
        }else{
            if(has_dgpu) 
                continue;
            else{
                temp_device = physical_devices[i];
            }
        }
    }

    device_info->physical_device = temp_device;
    free(physical_devices);
    return true;
}

static bool choose_queue_family(Device_Info_t *device_info, VkQueueFlags queue_family_flag, uint32_t num_queues_desired){
    uint32_t num_queue_families = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device_info->physical_device, &num_queue_families, NULL);
    if(num_queue_families == 0)
        return false;

    VkQueueFamilyProperties *queue_family_properties = (VkQueueFamilyProperties *) malloc(num_queue_families * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device_info->physical_device, &num_queue_families, queue_family_properties);

    bool has_flag = false;
    for(uint32_t i = 0; i < num_queue_families; i++){
        vk_helper::print_queue_family_properties(i, queue_family_properties[i]);
        if((queue_family_properties[i].queueFlags & queue_family_flag) == true){
            device_info->queue_index = i;
            device_info->num_queues = queue_family_properties[i].queueCount;
            if(num_queues_desired > queue_family_properties[i].queueCount)
                device_info->num_queues_in_use = queue_family_properties[i].queueCount;
            else    
                device_info->num_queues_in_use = num_queues_desired;

            has_flag = true;
            break;
        }
    }

    free(queue_family_properties);
    return has_flag;
}

static bool get_device_extensions(Device_Info_t *device_info){
    uint32_t num_device_extensions = 0;
    vkEnumerateDeviceExtensionProperties(device_info->physical_device, NULL, &num_device_extensions, NULL);
    if(num_device_extensions == 0)
        return false;

    VkExtensionProperties *device_extensions = (VkExtensionProperties *) malloc(num_device_extensions * sizeof(VkExtensionProperties));
    if(vkEnumerateDeviceExtensionProperties(device_info->physical_device, NULL, &num_device_extensions, device_extensions) != VK_SUCCESS){
        free(device_extensions);
        return false;
    }

    bool required_extension = false;
    for(uint32_t i = 0; i < num_device_extensions; i++){
        if(strcmp(device_extensions[i].extensionName, "VK_KHR_swapchain") == 0){
            str_array_push_back(device_info->extensions, device_extensions[i].extensionName);
            required_extension = true;
        }
#ifdef __APPLE__
        if(strcmp(device_extensions[i].extensionName, "VK_KHR_portability_subset") == 0)
            str_array_push_back(device_info->extensions, device_extensions[i].extensionName);
#endif
    }

    free(device_extensions);
    return required_extension;
}

static bool is_device_supported(const Device_Info_t device_info, VkSurfaceKHR surface){
    VkBool32 is_supported;
    if(vkGetPhysicalDeviceSurfaceSupportKHR(device_info.physical_device, device_info.queue_index, surface, &is_supported) != VK_SUCCESS) 
        return false;
    if(is_supported == VK_FALSE)
        return false;

    return true;
}

static bool add_device_features(Device_Info_t *device_info, uint32_t num_features, ...){
    if(num_features == 0){
        device_info->features = {};
        return true;
    }
    
    vkGetPhysicalDeviceFeatures(device_info->physical_device, &device_info->features);
    vk_helper::enumerate_features(device_info->features);

    va_list args;
    va_start(args, num_features);
    for(uint32_t i = 0; i < num_features; i++){
        vk_helper::Available_Features feature_to_enable = va_arg(args, vk_helper::Available_Features);
        if(vk_helper::is_feature_enabled(device_info->features, feature_to_enable) == false)
            vk_helper::enable_feature(&device_info->features, feature_to_enable);
    }

    va_end(args);
    return true;
}

static bool initialize_device_get_queue_handle(Graphics *gfx){
    if(!get_best_device(gfx->instance, &gfx->device_info))
        return false;
    if(!choose_queue_family(&gfx->device_info, VK_QUEUE_GRAPHICS_BIT, 1))
        return false;
    if(!get_device_extensions(&gfx->device_info))
        return false;
    if(!is_device_supported(gfx->device_info, gfx->surface))
        return false;
    if(!add_device_features(&gfx->device_info, 0))
        return false;

    float *priority = (float *) calloc(gfx->device_info.num_queues_in_use, sizeof(float));
    float priority_value = 1.0f;
    for(uint32_t i = 0; i < gfx->device_info.num_queues_in_use; i++){
        priority[i] = priority_value;
        priority_value -= 0.1f;
        if(priority_value <= 0.0f)
            priority_value = 0.0f;
    }
    
    VkDeviceQueueCreateInfo queue_create_info {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.pNext = NULL;
    queue_create_info.flags = 0;
    queue_create_info.queueFamilyIndex = gfx->device_info.queue_index;
    queue_create_info.queueCount = gfx->device_info.num_queues_in_use;
    queue_create_info.pQueuePriorities = priority; 

    VkDeviceCreateInfo device_create_info {};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pNext = NULL;
    device_create_info.flags = 0;
    device_create_info.queueCreateInfoCount = 1;
    device_create_info.pQueueCreateInfos = &queue_create_info;
    device_create_info.enabledLayerCount = 0;
    device_create_info.ppEnabledLayerNames = NULL;
    device_create_info.enabledExtensionCount = gfx->device_info.extensions->num_strings;
    device_create_info.ppEnabledExtensionNames = gfx->device_info.extensions->strings;
    device_create_info.pEnabledFeatures = &gfx->device_info.features;

    if(vkCreateDevice(gfx->device_info.physical_device, &device_create_info, NULL, &gfx->device_info.logical_device) != VK_SUCCESS) 
        return false;

    vkGetDeviceQueue(gfx->device_info.logical_device, gfx->device_info.queue_index, 0, &gfx->device_info.logical_device_queue);

    free(priority);
    return true;
}

static bool get_swapchain_surface_capabilities(Graphics *gfx){
    VkSurfaceCapabilitiesKHR surface_capabililites {};
    if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gfx->device_info.physical_device, gfx->surface, &surface_capabililites) != VK_SUCCESS)
        return false;
    
    vk_helper::enumerate_device_surface_capabilities(surface_capabililites);
    gfx->swapchain_info.min_image_count = surface_capabililites.minImageCount;
    gfx->swapchain_info.max_image_count = surface_capabililites.maxImageCount;
    gfx->swapchain_info.num_images_used = surface_capabililites.minImageCount + 1;
    gfx->swapchain_info.min_image_extent = surface_capabililites.minImageExtent;
    gfx->swapchain_info.max_image_extent = surface_capabililites.maxImageExtent;
    gfx->swapchain_info.curr_image_extend = surface_capabililites.currentExtent;
    gfx->swapchain_info.max_image_array_layers = surface_capabililites.maxImageArrayLayers;
    gfx->swapchain_info.num_image_array_layers_used = 1;
    gfx->swapchain_info.supported_transforms = surface_capabililites.supportedTransforms;
    gfx->swapchain_info.chosen_transform = surface_capabililites.currentTransform;
    gfx->swapchain_info.supported_alpha_flags = surface_capabililites.supportedCompositeAlpha;
    if(!vk_helper::choose_alpha_flag(&gfx->swapchain_info.chosen_alpha_flag, vk_helper::ALPHA_FLAG_OPAQUE))
        return false;
    gfx->swapchain_info.supported_usage_flags = surface_capabililites.supportedUsageFlags;
    if(!vk_helper::choose_usage_flag(&gfx->swapchain_info.chosen_usage_flags, vk_helper::USAGE_FLAG_COLOR_ATTACHMENT))
        return false;

    return true;
}

static bool get_swapchain_device_formats(Graphics *gfx){
    uint32_t num_formats = 0;
    
    vkGetPhysicalDeviceSurfaceFormatsKHR(gfx->device_info.physical_device, gfx->surface, &num_formats, NULL);
    if(num_formats == 0)
        return false;

    VkSurfaceFormatKHR *surface_formats = (VkSurfaceFormatKHR *) malloc(num_formats * sizeof(VkSurfaceFormatKHR));
    if(vkGetPhysicalDeviceSurfaceFormatsKHR(gfx->device_info.physical_device, gfx->surface, &num_formats, surface_formats) != VK_SUCCESS){
        free(surface_formats);
        return false;
    }
    vk_helper::enumerate_device_surface_formats(surface_formats, num_formats);

    bool found_format = false;
    constexpr uint32_t num_possible_formats = 2;
    VkFormat possible_formats[num_possible_formats];
    possible_formats[0] = VK_FORMAT_B8G8R8A8_SRGB;
    possible_formats[1] = VK_FORMAT_B8G8R8A8_UNORM;

    /**
    * @brief: This Loop has 2 possible formats to choose: B8G8R8A8_SRGB or B8G8R8A8_UNORM. 
    *         If none of these formats are available, it will fallback 
    */
    for(uint32_t i = 0; i < num_possible_formats && found_format != true; i++){
        for(uint32_t j = 0; j < num_formats; j++){
            if(possible_formats[i] == surface_formats[j].format){
                gfx->swapchain_info.format = surface_formats[j].format;
                gfx->swapchain_info.color_space = surface_formats[j].colorSpace;
                found_format = true;
                break;
            }
        }
    }

    free(surface_formats);
    return found_format;
}

static bool get_swapchain_present_mode(Graphics *gfx){
    uint32_t num_present_modes = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(gfx->device_info.physical_device, gfx->surface, &num_present_modes, NULL);
    if(num_present_modes == 0)
        return false;

    VkPresentModeKHR *available_present_modes = (VkPresentModeKHR *) malloc(num_present_modes * sizeof(VkPresentModeKHR));
    if(vkGetPhysicalDeviceSurfacePresentModesKHR(gfx->device_info.physical_device, gfx->surface, &num_present_modes, available_present_modes) != VK_SUCCESS){
        free(available_present_modes);
        return false;
    }
    vk_helper::enumerate_device_present_modes(available_present_modes, num_present_modes);

    bool found_present_mode = false;
    constexpr uint32_t num_possible_present_modes = 3;
    VkPresentModeKHR possible_present_modes[num_possible_present_modes];
    possible_present_modes[0] = VK_PRESENT_MODE_FIFO_KHR;
    possible_present_modes[1] = VK_PRESENT_MODE_MAILBOX_KHR;
    possible_present_modes[2] = VK_PRESENT_MODE_IMMEDIATE_KHR;

    /**
    * @brief: This Loop has 3 possible present modes to choose: FIFO, MAILBOX or IMMEDIATE. 
    *         If none of these present modes are available, it will fallback 
    */
    for(uint32_t i = 0; i < num_possible_present_modes && found_present_mode != true; i++){
        for(uint32_t j = 0; j < num_present_modes; j++){
            if(possible_present_modes[i] == available_present_modes[j]){
                gfx->swapchain_info.present_mode = available_present_modes[j];
                found_present_mode = true;
                break;
            }
        }
    }

    free(available_present_modes);
    return found_present_mode;
}

static bool initialize_swapchain(Graphics *gfx){
    if(!get_swapchain_surface_capabilities(gfx))
        return false;
    if(!get_swapchain_device_formats(gfx))
        return false;
    if(!get_swapchain_present_mode(gfx))
        return false;

    VkSwapchainCreateInfoKHR swapchain_create_info {};
    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.pNext = NULL;
    swapchain_create_info.flags = 0;
    swapchain_create_info.surface = gfx->surface;
    swapchain_create_info.minImageCount = gfx->swapchain_info.num_images_used;
    swapchain_create_info.imageFormat = gfx->swapchain_info.format;
    swapchain_create_info.imageColorSpace = gfx->swapchain_info.color_space;
    swapchain_create_info.imageExtent = gfx->swapchain_info.curr_image_extend;
    swapchain_create_info.imageArrayLayers = gfx->swapchain_info.num_image_array_layers_used;
    swapchain_create_info.imageUsage = gfx->swapchain_info.chosen_usage_flags;
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_create_info.queueFamilyIndexCount = 0;
    swapchain_create_info.pQueueFamilyIndices = NULL;
    swapchain_create_info.preTransform = gfx->swapchain_info.chosen_transform;
    swapchain_create_info.compositeAlpha = gfx->swapchain_info.chosen_alpha_flag;
    swapchain_create_info.presentMode = gfx->swapchain_info.present_mode;
    swapchain_create_info.clipped = VK_TRUE;
    swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

    if(vkCreateSwapchainKHR(gfx->device_info.logical_device, &swapchain_create_info, NULL, &gfx->swapchain_info.swapchain) != VK_SUCCESS)
        return false;

    return true;
}

static bool initialize_image_views(Graphics *gfx){
    vkGetSwapchainImagesKHR(gfx->device_info.logical_device, gfx->swapchain_info.swapchain, &gfx->image_info.num_images, NULL);
    if(gfx->image_info.num_images == 0)
        return false;

    gfx->image_info.images = (VkImage *) malloc(gfx->image_info.num_images * sizeof(VkImage));
    gfx->image_info.image_view = (VkImageView *) malloc(gfx->image_info.num_images * sizeof(VkImageView));
    
    if(vkGetSwapchainImagesKHR(gfx->device_info.logical_device, gfx->swapchain_info.swapchain, &gfx->image_info.num_images, gfx->image_info.images) != VK_SUCCESS)
        return false;

    for(uint32_t i = 0; i < gfx->image_info.num_images; i++){
        VkImageViewCreateInfo image_view_create_info {};
        image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.pNext = NULL;
        image_view_create_info.flags = 0;
        image_view_create_info.image = gfx->image_info.images[i];
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format = gfx->swapchain_info.format;
        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;    // TEST IF THIS COMPONENT COULD BE THE WAY I CHANGE THE COLOR OF MY SCREEN
        image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY; 
        image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info.subresourceRange.baseMipLevel = 0;
        image_view_create_info.subresourceRange.levelCount = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount = 1;

        if(vkCreateImageView(gfx->device_info.logical_device, &image_view_create_info, NULL, &gfx->image_info.image_view[i]) != VK_SUCCESS)
            return false;
    }

    return true;
}

static bool create_shader_module(const VkDevice logical_device, Shader_Data *shader_data, const char *file_path){
    if(shader_data == NULL || file_path == NULL || logical_device == NULL)
        return false;

    uint32_t file_size = 0;
    uint32_t *file_data = Core::File::read_spv_file(file_path, &file_size);
    if(file_data == NULL)
        return false;

    VkShaderModuleCreateInfo shader_module_create_info {};
    shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info.pNext = NULL;
    shader_module_create_info.flags = 0;
    shader_module_create_info.codeSize = file_size;
    shader_module_create_info.pCode = (const uint32_t *) file_data;

    if(vkCreateShaderModule(logical_device, &shader_module_create_info, NULL, &shader_data->shader_module) != VK_SUCCESS)
        return false;

    free(file_data);
    return true;
}

static bool create_shader_stage_info(Shader_Data *shader_data, int shader_type){
    if(shader_data == NULL || shader_type < 0)
        return false;

    shader_data->shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_data->shader_stage_create_info.pNext = NULL;
    shader_data->shader_stage_create_info.flags = 0;
    shader_data->shader_type = shader_type;
    switch(shader_type){
        case SHADER_TYPE_VERTEX:
            shader_data->shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        break;

        case SHADER_TYPE_FRAGMENT:
            shader_data->shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        break;
    }
    shader_data->shader_stage_create_info.module = shader_data->shader_module;
    shader_data->shader_stage_create_info.pName = "main";
    shader_data->shader_stage_create_info.pSpecializationInfo = NULL;

    return true;
}

static VkPipelineVertexInputStateCreateInfo create_vertex_input_state_info(void){
    VkPipelineVertexInputStateCreateInfo vertex_state_create_info {};
    vertex_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_state_create_info.pNext = NULL;
    vertex_state_create_info.flags = 0;
    vertex_state_create_info.vertexBindingDescriptionCount = 0;
    vertex_state_create_info.pVertexBindingDescriptions = NULL;
    vertex_state_create_info.vertexAttributeDescriptionCount = 0;
    vertex_state_create_info.pVertexAttributeDescriptions = NULL;

    return vertex_state_create_info;
}

static VkPipelineInputAssemblyStateCreateInfo create_assembly_state_create_info(VkPrimitiveTopology topology){
    VkPipelineInputAssemblyStateCreateInfo assembly_state_create_info {};
    assembly_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    assembly_state_create_info.pNext = NULL;
    assembly_state_create_info.flags = 0;
    assembly_state_create_info.topology = topology;
    assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

    return assembly_state_create_info;
}

static VkPipelineViewportStateCreateInfo create_viewport_state_create_info(const VkExtent2D curr_image_extent){
    static VkViewport viewport {};
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = (float) curr_image_extent.width;
    viewport.height = (float) curr_image_extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    
    static VkRect2D scissor {};
    scissor.offset = {0, 0};
    scissor.extent = curr_image_extent;

    VkPipelineViewportStateCreateInfo viewport_state_create_info {};
    viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_create_info.pNext = NULL;
    viewport_state_create_info.flags = 0;
    viewport_state_create_info.viewportCount = 1;
    viewport_state_create_info.pViewports = &viewport;
    viewport_state_create_info.scissorCount = 1;
    viewport_state_create_info.pScissors = &scissor;

    return viewport_state_create_info;
}

static VkPipelineRasterizationStateCreateInfo create_rasterizer_state_info(bool enable_clamp, bool enable_rasterizer_discard, VkPolygonMode polygon_mode, VkCullModeFlags cull_flags, VkFrontFace face_orientation, float line_width){
    VkPipelineRasterizationStateCreateInfo rasterizer_state_info {};
    rasterizer_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer_state_info.pNext = NULL;
    rasterizer_state_info.flags = 0;
    if(enable_clamp)
        rasterizer_state_info.depthClampEnable = VK_TRUE;
    else
        rasterizer_state_info.depthClampEnable = VK_FALSE;
    if(enable_rasterizer_discard)
        rasterizer_state_info.rasterizerDiscardEnable = VK_TRUE;
    else
        rasterizer_state_info.rasterizerDiscardEnable = VK_FALSE;
    rasterizer_state_info.polygonMode = polygon_mode;
    rasterizer_state_info.cullMode = cull_flags;
    rasterizer_state_info.frontFace = face_orientation;
    rasterizer_state_info.depthBiasEnable = VK_FALSE;
    rasterizer_state_info.depthBiasConstantFactor = 0.0f;
    rasterizer_state_info.depthBiasClamp = 0.0f;
    rasterizer_state_info.depthBiasSlopeFactor = 0.0f;
    rasterizer_state_info.lineWidth = line_width;

    return rasterizer_state_info;
}

static VkPipelineMultisampleStateCreateInfo create_multisample_state_create_info(uint32_t num_samples, bool enable_shading, float min_sample_shading_fraction){
    VkPipelineMultisampleStateCreateInfo multisample_state_create_info {};
    multisample_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state_create_info.pNext = NULL;
    multisample_state_create_info.flags = 0;
    switch(num_samples){
        case 1:
            multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        break;

        case 2:
            multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_2_BIT;
        break;

        case 4:
            multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_4_BIT;
        break;

        case 8:
            multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_8_BIT;
        break;

        case 16:
            multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_16_BIT;
        break;

        case 32:
            multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_32_BIT;
        break;

        case 64:
            multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_64_BIT;
        break;

        default:
            Core::debug::log(Core::debug::Fatal, "Not a valid number of samples\n");
        break;
    }

    if(enable_shading){
        multisample_state_create_info.sampleShadingEnable = VK_TRUE;
        multisample_state_create_info.minSampleShading = min_sample_shading_fraction;
    }
    else{
        multisample_state_create_info.sampleShadingEnable = VK_FALSE;
        multisample_state_create_info.minSampleShading = 1.0f;
    }
    multisample_state_create_info.pSampleMask = NULL;
    multisample_state_create_info.alphaToCoverageEnable = VK_FALSE;
    multisample_state_create_info.alphaToOneEnable = VK_FALSE;

    return multisample_state_create_info;
}

static VkPipelineColorBlendStateCreateInfo create_color_blend_state_create_info(bool enable_color_blending){
    static VkPipelineColorBlendStateCreateInfo color_blend_state_info {};
    static VkPipelineColorBlendAttachmentState color_blend_attachment_state {};

    if(enable_color_blending){
        color_blend_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blend_state_info.pNext = NULL;
        color_blend_state_info.flags = 0;
        color_blend_state_info.logicOpEnable = VK_TRUE;
        color_blend_state_info.logicOp = VK_LOGIC_OP_COPY;
        color_blend_state_info.attachmentCount = 1;
        color_blend_attachment_state.blendEnable = VK_TRUE;
        color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        color_blend_state_info.pAttachments = &color_blend_attachment_state;
        color_blend_state_info.blendConstants[0] = 0.0f;
        color_blend_state_info.blendConstants[1] = 0.0f;
        color_blend_state_info.blendConstants[2] = 0.0f;
        color_blend_state_info.blendConstants[3] = 0.0f;
    }else{
        color_blend_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blend_state_info.pNext = NULL;
        color_blend_state_info.flags = 0;
        color_blend_state_info.logicOpEnable = VK_FALSE;
        color_blend_state_info.logicOp = VK_LOGIC_OP_COPY;
        color_blend_state_info.attachmentCount = 1;
        color_blend_attachment_state.blendEnable = VK_FALSE;
        color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        color_blend_state_info.pAttachments = &color_blend_attachment_state;
        color_blend_state_info.blendConstants[0] = 0.0f;
        color_blend_state_info.blendConstants[1] = 0.0f;
        color_blend_state_info.blendConstants[2] = 0.0f;
        color_blend_state_info.blendConstants[3] = 0.0f;
    }
    
    return color_blend_state_info;
}

static VkPipelineDynamicStateCreateInfo create_dynamic_state_create_info(bool tesselation_enabled, bool depth_stencil_enabled){
    VkPipelineDynamicStateCreateInfo dynamic_state_create_info {};

    if(!tesselation_enabled && ! depth_stencil_enabled){
        static VkDynamicState dynamic_states[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
        };
        
        dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_create_info.pNext = NULL;
        dynamic_state_create_info.flags = 0;
        dynamic_state_create_info.dynamicStateCount = 2;
        dynamic_state_create_info.pDynamicStates = dynamic_states;
    }else if(tesselation_enabled){
        // LOGIC TO IMPLEMENT LATER ON
    }else if(depth_stencil_enabled){
        // LOGIC TO IMPLEMENT LATER ON
    }else{
        // LOGIC TO IMPLEMENT LATER ON
    }

    return dynamic_state_create_info;
}

static bool create_graphics_pipeline_layout(const VkDevice logical_device, VkPipelineLayout *pipeline_layout){
    VkPipelineLayoutCreateInfo pipeline_layout_create_info {};
    pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_create_info.pNext = NULL;
    pipeline_layout_create_info.flags = 0;
    pipeline_layout_create_info.setLayoutCount = 0;
    pipeline_layout_create_info.pSetLayouts = NULL;
    pipeline_layout_create_info.pushConstantRangeCount = 0;
    pipeline_layout_create_info.pPushConstantRanges = NULL;

    if(vkCreatePipelineLayout(logical_device, &pipeline_layout_create_info, NULL, pipeline_layout) != VK_SUCCESS) 
        return false;

    return true;
}

static bool create_render_pass(const VkDevice logical_device, const Swapchain_Info_t swapchain_info, VkRenderPass *render_pass){
    VkAttachmentDescription attachment_description {};
    attachment_description.flags = 0;
    attachment_description.format = swapchain_info.format;
    attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment_description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_reference {};
    color_attachment_reference.attachment = 0;
    color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass_reference {};
    subpass_reference.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass_reference.colorAttachmentCount = 1;
    subpass_reference.pColorAttachments = &color_attachment_reference;

    // This dependency struct was added when i was creating my draw function
    VkSubpassDependency subpass_dependency {};
    subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpass_dependency.dstSubpass = 0;
    subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dependency.srcAccessMask = 0;
    subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo render_pass_create_info {};
    render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info.pNext = NULL;
    render_pass_create_info.flags = 0;
    render_pass_create_info.attachmentCount = 1;
    render_pass_create_info.pAttachments = &attachment_description;
    render_pass_create_info.subpassCount = 1;
    render_pass_create_info.pSubpasses = &subpass_reference;
    render_pass_create_info.dependencyCount = 1;
    render_pass_create_info.pDependencies = &subpass_dependency;

    if(vkCreateRenderPass(logical_device, &render_pass_create_info, NULL, render_pass) != VK_SUCCESS) 
        return false;

    return true;
}

static bool create_graphics_pipeline(Graphics *gfx){
    gfx->shader_info.num_shaders = 2;
    gfx->shader_info.data = (Shader_Data *) malloc(gfx->shader_info.num_shaders * sizeof(Shader_Data));

    if(!create_shader_module(gfx->device_info.logical_device, &gfx->shader_info.data[0], "Shaders/VertexShader.spv"))
        return false;
    if(!create_shader_stage_info(&gfx->shader_info.data[0], SHADER_TYPE_VERTEX))
        return false;
    if(!create_shader_module(gfx->device_info.logical_device, &gfx->shader_info.data[1], "Shaders/FragmentShader.spv"))
        return false;
    if(!create_shader_stage_info(&gfx->shader_info.data[1], SHADER_TYPE_FRAGMENT))
        return false;

    VkPipelineShaderStageCreateInfo *shader_state_create_info = (VkPipelineShaderStageCreateInfo *) malloc(gfx->shader_info.num_shaders * sizeof(VkPipelineShaderStageCreateInfo));
    for(uint32_t i = 0; i < gfx->shader_info.num_shaders; i++)
        shader_state_create_info[i] = gfx->shader_info.data[i].shader_stage_create_info;

    VkPipelineVertexInputStateCreateInfo vertex_state_create_info = create_vertex_input_state_info();
    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info = create_assembly_state_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    VkPipelineViewportStateCreateInfo viewport_state_create_info = create_viewport_state_create_info(gfx->swapchain_info.curr_image_extend);
    VkPipelineRasterizationStateCreateInfo rasterizer_state_create_info = create_rasterizer_state_info(false, false, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE, 1.0f);
    VkPipelineMultisampleStateCreateInfo multisample_state_create_info = create_multisample_state_create_info(1, false, 0.0f);
    VkPipelineColorBlendStateCreateInfo color_blend_state_info = create_color_blend_state_create_info(false);
    VkPipelineDynamicStateCreateInfo dynamic_state_create_info = create_dynamic_state_create_info(false, false);
    if(!create_graphics_pipeline_layout(gfx->device_info.logical_device, &gfx->graphics_pipeline_info.pipeline_layout))
        return false;
    if(!create_render_pass(gfx->device_info.logical_device, gfx->swapchain_info, &gfx->graphics_pipeline_info.render_pass))
        return false;

    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info {};
    graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline_create_info.pNext = NULL;
    graphics_pipeline_create_info.flags = 0;
    graphics_pipeline_create_info.stageCount = gfx->shader_info.num_shaders;
    graphics_pipeline_create_info.pStages = shader_state_create_info;
    graphics_pipeline_create_info.pVertexInputState = &vertex_state_create_info;
    graphics_pipeline_create_info.pInputAssemblyState = &input_assembly_state_create_info;
    graphics_pipeline_create_info.pTessellationState = NULL;    // THE EXTENSIONS DON'T NEED TO BE ENABLED DUE TO ME USING VULKAN 1.0.0 PLUS REQUIRING CERTAIN DEVICE EXTENSIONS ENABLED
    graphics_pipeline_create_info.pViewportState = &viewport_state_create_info;
    graphics_pipeline_create_info.pRasterizationState = &rasterizer_state_create_info;
    graphics_pipeline_create_info.pMultisampleState = &multisample_state_create_info;
    graphics_pipeline_create_info.pDepthStencilState = NULL;    // THE EXTENSIONS DON'T NEED TO BE ENABLED DUE TO ME USING VULKAN 1.0.0 PLUS REQUIRING CERTAIN DEVICE EXTENSIONS ENABLED
    graphics_pipeline_create_info.pColorBlendState = &color_blend_state_info;
    graphics_pipeline_create_info.pDynamicState = &dynamic_state_create_info;
    graphics_pipeline_create_info.layout = gfx->graphics_pipeline_info.pipeline_layout;
    graphics_pipeline_create_info.renderPass = gfx->graphics_pipeline_info.render_pass;
    graphics_pipeline_create_info.subpass = 0;
    graphics_pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
    graphics_pipeline_create_info.basePipelineIndex = -1;

    if(vkCreateGraphicsPipelines(gfx->device_info.logical_device, VK_NULL_HANDLE, 1, &graphics_pipeline_create_info, NULL, &gfx->graphics_pipeline_info.pipeline) != VK_SUCCESS)
        return false;

    free(shader_state_create_info);
    return true;
}

static bool create_frame_buffers(Graphics *gfx){
    gfx->image_info.frame_buffers = (VkFramebuffer *) malloc(gfx->image_info.num_images * sizeof(VkFramebuffer));
    
    for(uint32_t i = 0; i < gfx->image_info.num_images; i++){
        VkFramebufferCreateInfo frame_buffer_create_info {};
        frame_buffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frame_buffer_create_info.pNext = NULL;
        frame_buffer_create_info.flags = 0;
        frame_buffer_create_info.renderPass = gfx->graphics_pipeline_info.render_pass;
        frame_buffer_create_info.attachmentCount = 1;
        frame_buffer_create_info.pAttachments = &gfx->image_info.image_view[i];
        frame_buffer_create_info.width = gfx->swapchain_info.curr_image_extend.width;
        frame_buffer_create_info.height = gfx->swapchain_info.curr_image_extend.height;
        frame_buffer_create_info.layers = gfx->swapchain_info.num_image_array_layers_used;

        if(vkCreateFramebuffer(gfx->device_info.logical_device, &frame_buffer_create_info, NULL, &gfx->image_info.frame_buffers[i]) != VK_SUCCESS)
            return false;
    }

    return true;
}

static bool create_sync_objects(Graphics *gfx){
    gfx->sync_objects_info.num_sync_objects = MAX_FRAME_IN_FLIGHT;
    gfx->sync_objects_info.image_available_semaphores = (VkSemaphore *) malloc(gfx->sync_objects_info.num_sync_objects * sizeof(VkSemaphore));
    gfx->sync_objects_info.fences = (VkFence *) malloc(gfx->sync_objects_info.num_sync_objects * sizeof(VkFence));
    gfx->sync_objects_info.render_finished_semaphores = (VkSemaphore *) malloc(gfx->image_info.num_images * sizeof(VkSemaphore));

    VkSemaphoreCreateInfo semaphore_create_info {};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = NULL;
    semaphore_create_info.flags = 0;

    VkFenceCreateInfo fence_create_info {};
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.pNext = NULL;
    fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;    // MIGHT NEED TO CREATE IT IN THE SIGNALED STATE

    VkResult result;
    for(uint32_t i = 0; i < gfx->sync_objects_info.num_sync_objects; i++){
        result = vkCreateSemaphore(gfx->device_info.logical_device, &semaphore_create_info, NULL, &gfx->sync_objects_info.image_available_semaphores[i]);
        if(result != VK_SUCCESS)
            return false;
        result = vkCreateFence(gfx->device_info.logical_device, &fence_create_info, NULL, &gfx->sync_objects_info.fences[i]);
        if(result != VK_SUCCESS)
            return false;
    }

    for(uint32_t i = 0; i < gfx->image_info.num_images; i++){
        result = vkCreateSemaphore(gfx->device_info.logical_device, &semaphore_create_info, NULL, &gfx->sync_objects_info.render_finished_semaphores[i]);
        if(result != VK_SUCCESS)
            return false;
    }

    return true;
}

static bool create_command_pool_buffer(Graphics *gfx, uint32_t num_command_pools, uint32_t num_command_buffers){
    gfx->commands_info.num_command_pools = num_command_pools;
    gfx->commands_info.num_command_buffers = num_command_buffers;
    gfx->commands_info.command_pools = (VkCommandPool *) malloc(num_command_pools * sizeof(VkCommandPool));
    gfx->commands_info.command_buffers = (VkCommandBuffer *) malloc(num_command_buffers * sizeof(VkCommandBuffer));
    
    VkCommandPoolCreateInfo command_pool_create_info {};
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.pNext = NULL;
    command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_create_info.queueFamilyIndex = gfx->device_info.queue_index;

    for(uint32_t i = 0; i < num_command_pools; i++){
        if(vkCreateCommandPool(gfx->device_info.logical_device, &command_pool_create_info, NULL, &gfx->commands_info.command_pools[i]) != VK_SUCCESS)
            return false;
    }

    VkCommandBufferAllocateInfo command_buffer_allocate_info {};
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.pNext = NULL;
    command_buffer_allocate_info.commandPool = gfx->commands_info.command_pools[0];
    command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount = 1;
    
    for(uint32_t i = 0; i < num_command_buffers; i++){
        if(vkAllocateCommandBuffers(gfx->device_info.logical_device, &command_buffer_allocate_info, &gfx->commands_info.command_buffers[i]) != VK_SUCCESS)
            return false;
    }

    return true;
}

static void record_command_buffer(Graphics *gfx, uint32_t curr_image_index, uint32_t curr_frame){
    VkCommandBufferBeginInfo command_buffer_begin_info {};
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.pNext = NULL;
    command_buffer_begin_info.flags = 0;
    command_buffer_begin_info.pInheritanceInfo = NULL;

    if(vkBeginCommandBuffer(gfx->commands_info.command_buffers[curr_frame], &command_buffer_begin_info) != VK_SUCCESS)
        exit(EXIT_FAILURE);

    VkRenderPassBeginInfo render_pass_begin_info {};
    render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_begin_info.pNext = NULL;
    render_pass_begin_info.renderPass = gfx->graphics_pipeline_info.render_pass;
    render_pass_begin_info.framebuffer = gfx->image_info.frame_buffers[curr_image_index];
    render_pass_begin_info.renderArea.offset = {0, 0};
    render_pass_begin_info.renderArea.extent = gfx->swapchain_info.curr_image_extend;
    VkClearValue clear_color;
    clear_color.color.float32[0] = 0.0f;
    clear_color.color.float32[1] = 0.0f;
    clear_color.color.float32[2] = 0.0f;
    clear_color.color.float32[3] = 1.0f;
    render_pass_begin_info.clearValueCount = 1;
    render_pass_begin_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(gfx->commands_info.command_buffers[curr_frame], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(gfx->commands_info.command_buffers[curr_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, gfx->graphics_pipeline_info.pipeline);

    VkViewport viewport {};
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = (float) gfx->swapchain_info.curr_image_extend.width;
    viewport.height = (float) gfx->swapchain_info.curr_image_extend.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(gfx->commands_info.command_buffers[curr_frame], 0, 1, &viewport);

    VkRect2D scissor {};
    scissor.offset = {0, 0};
    scissor.extent = gfx->swapchain_info.curr_image_extend;
    vkCmdSetScissor(gfx->commands_info.command_buffers[curr_frame], 0, 1, &scissor);

    vkCmdDraw(gfx->commands_info.command_buffers[curr_frame], 3, 1, 0, 0);
    vkCmdEndRenderPass(gfx->commands_info.command_buffers[curr_frame]);

    if(vkEndCommandBuffer(gfx->commands_info.command_buffers[curr_frame]) != VK_SUCCESS)
        exit(EXIT_FAILURE);

    return;
}

void temp_render_function(Graphics *gfx){
    static uint32_t curr_frame = 0;
    static uint32_t curr_image_index = 0;

    vkWaitForFences(gfx->device_info.logical_device, 1, &gfx->sync_objects_info.fences[curr_frame], VK_TRUE, UINT64_MAX);
    vkResetFences(gfx->device_info.logical_device, 1, &gfx->sync_objects_info.fences[curr_frame]);

    VkResult result = vkAcquireNextImageKHR(gfx->device_info.logical_device, gfx->swapchain_info.swapchain, UINT64_MAX, gfx->sync_objects_info.image_available_semaphores[curr_frame], VK_NULL_HANDLE, &curr_image_index);
    if(result != VK_SUCCESS)
        exit(EXIT_FAILURE);

    vkResetCommandBuffer(gfx->commands_info.command_buffers[curr_frame], 0);
    record_command_buffer(gfx, curr_image_index, curr_frame);

    VkSubmitInfo submit_info {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = NULL;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &gfx->sync_objects_info.image_available_semaphores[curr_frame];
    VkPipelineStageFlags stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit_info.pWaitDstStageMask = &stage_flags;  
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &gfx->commands_info.command_buffers[curr_frame];
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &gfx->sync_objects_info.render_finished_semaphores[curr_image_index];

    if(vkQueueSubmit(gfx->device_info.logical_device_queue, 1, &submit_info, gfx->sync_objects_info.fences[curr_frame]) != VK_SUCCESS)
        exit(EXIT_FAILURE);

    VkPresentInfoKHR present_info {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.pNext = NULL;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &gfx->sync_objects_info.render_finished_semaphores[curr_image_index];
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &gfx->swapchain_info.swapchain;
    present_info.pImageIndices = &curr_image_index;
    present_info.pResults = NULL;

    vkQueuePresentKHR(gfx->device_info.logical_device_queue, &present_info);

    curr_frame = (curr_frame + 1) % MAX_FRAME_IN_FLIGHT;

    return;
}

static void destroy_command_pool_buffer(Graphics *gfx){
    for(uint32_t i = 0; i < gfx->commands_info.num_command_pools; i++){
        vkDestroyCommandPool(gfx->device_info.logical_device, gfx->commands_info.command_pools[i], NULL);
    }
    free(gfx->commands_info.command_pools);
    free(gfx->commands_info.command_buffers);

    return;
}

static void destroy_sync_objects(Graphics *gfx){
    for(uint32_t i = 0; i < gfx->sync_objects_info.num_sync_objects; i++){
        vkDestroySemaphore(gfx->device_info.logical_device, gfx->sync_objects_info.image_available_semaphores[i], NULL);
        vkDestroyFence(gfx->device_info.logical_device, gfx->sync_objects_info.fences[i], NULL);
    }
    for(uint32_t i = 0; i < gfx->image_info.num_images; i++)
        vkDestroySemaphore(gfx->device_info.logical_device, gfx->sync_objects_info.render_finished_semaphores[i], NULL);

    free(gfx->sync_objects_info.image_available_semaphores);
    free(gfx->sync_objects_info.render_finished_semaphores);
    free(gfx->sync_objects_info.fences);

    return;
}

static void destroy_frame_buffers(Graphics *gfx){
    for(uint32_t i = 0; i < gfx->image_info.num_images; i++){
        vkDestroyFramebuffer(gfx->device_info.logical_device, gfx->image_info.frame_buffers[i], NULL);
    }
    free(gfx->image_info.frame_buffers);

    return;
}

static void destroy_graphics_pipeline(Graphics *gfx){
    for(uint32_t i = 0; i < gfx->shader_info.num_shaders; i++){
        vkDestroyShaderModule(gfx->device_info.logical_device, gfx->shader_info.data[i].shader_module, NULL);
    }
    free(gfx->shader_info.data);

    vkDestroyPipelineLayout(gfx->device_info.logical_device, gfx->graphics_pipeline_info.pipeline_layout, NULL);
    vkDestroyRenderPass(gfx->device_info.logical_device, gfx->graphics_pipeline_info.render_pass, NULL);
    vkDestroyPipeline(gfx->device_info.logical_device, gfx->graphics_pipeline_info.pipeline, NULL);

    return;
}

static void destroy_image_view(Graphics *gfx){
    for(uint32_t i = 0; i < gfx->image_info.num_images; i++){
        vkDestroyImageView(gfx->device_info.logical_device, gfx->image_info.image_view[i], NULL);
    }
    free(gfx->image_info.image_view);
    free(gfx->image_info.images);
    
    return;
}

static void destroy_swapchain(Graphics *gfx){
    vkDestroySwapchainKHR(gfx->device_info.logical_device, gfx->swapchain_info.swapchain, NULL);

    return;
}

static void destroy_device(Graphics *gfx){
    vkDestroyDevice(gfx->device_info.logical_device, NULL);

    return;
}

static void destroy_surface(Graphics *gfx){
    SDL_Vulkan_DestroySurface(gfx->instance, gfx->surface, NULL);

    return;
}

static void destroy_instance(Graphics *gfx){
    vkDestroyInstance(gfx->instance, NULL);

    return;
}

void close_graphics(Graphics *gfx){
    vkDeviceWaitIdle(gfx->device_info.logical_device);
    destroy_command_pool_buffer(gfx);
    destroy_sync_objects(gfx);
    destroy_frame_buffers(gfx);
    destroy_graphics_pipeline(gfx);
    destroy_image_view(gfx);
    destroy_swapchain(gfx);
    destroy_device(gfx);
    destroy_surface(gfx);
    vk_debug::destroy_debug_messenger(gfx->instance);
    destroy_instance(gfx);
    free_str_array(gfx->device_info.extensions);
    free(gfx);

    printf("Completed graphics\n");
    return;
}