#include "Graphics_Helper.h"
#include <cstring>

/**********************************************************************************
*
*
*               vk_debug
*
*
*************************************************************************************/

static bool enable_layers(str_array *layers){
    bool layers_found = false;
    uint32_t num_layers = 0;
    vkEnumerateInstanceLayerProperties(&num_layers, NULL);
    if(num_layers == 0)
        return false;

    VkLayerProperties *layer_properties = (VkLayerProperties *) malloc(num_layers * sizeof(VkLayerProperties));
    if(vkEnumerateInstanceLayerProperties(&num_layers, layer_properties) != VK_SUCCESS){
        free(layer_properties);
        return false;
    }

    for(uint32_t i = 0; i < num_layers; i++){
        if(strcmp(layer_properties[i].layerName, "VK_LAYER_KHRONOS_validation") == 0){
            str_array_push_back(layers, layer_properties[i].layerName);
            layers_found = true;
        }
#if defined(_WIN32) || defined(__linux__)
        if(strcmp(layer_properties[i].layerName, "VK_LAYER_LUNARG_crash_diagnostic") == 0)
            str_array_push_back(layers, layer_properties[i].layerName);
#endif
    }

    free(layer_properties);
    return layers_found;
}

static bool enable_extensions(str_array *extensions){
    uint32_t num_extensions = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &num_extensions, NULL);
    if(num_extensions == 0)
        return false;

    VkExtensionProperties *extension_properties = (VkExtensionProperties *) malloc(num_extensions * sizeof(VkExtensionProperties));
    if(vkEnumerateInstanceExtensionProperties(NULL, &num_extensions, extension_properties) != VK_SUCCESS){
        free(extension_properties);
        return false;
    }

    bool required_extension = false;
    for(uint32_t i = 0; i < num_extensions; i++){
        if(strcmp(extension_properties[i].extensionName, "VK_KHR_surface") == 0){
            if(!str_array_check_str(extensions, extension_properties[i].extensionName))
                str_array_push_back(extensions, extension_properties[i].extensionName);

            required_extension = true;
        }

#ifdef DEBUG
        if(strcmp(extension_properties[i].extensionName, "VK_EXT_debug_utils") == 0)
            str_array_push_back(extensions, extension_properties[i].extensionName);
#endif

#if defined(__APPLE__)
        if(strcmp(extension_properties[i].extensionName, "VK_KHR_portability_enumeration") == 0){
            if(!str_array_check_str(extensions, extension_properties[i].extensionName))
                str_array_push_back(extensions, extension_properties[i].extensionName);
        }
        if(strcmp(extension_properties[i].extensionName, "VK_EXT_metal_surface") == 0){
            if(!str_array_check_str(extensions, extension_properties[i].extensionName))
                str_array_push_back(extensions, extension_properties[i].extensionName);
        }
        if(strcmp(extension_properties[i].extensionName, "VK_KHR_get_physical_device_properties2") == 0){
            if(!str_array_check_str(extensions, extension_properties[i].extensionName))
                str_array_push_back(extensions, extension_properties[i].extensionName);
        }
#endif
    }

    free(extension_properties);
    return required_extension;
}

static VkDebugUtilsMessengerEXT messenger = nullptr;

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
    VkDebugUtilsMessageTypeFlagsEXT             message_types,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void*                                       pUserData)
{
    (void) pUserData;  // This is just because i am getting annoyed at the warning generated when compiled for the unused parameter
    printf("Severity: %s\n", vk_helper::severity_to_str(message_severity));
    printf("Type: %s\n", vk_helper::msg_type_to_str(message_types));
    printf("Message Id: %d\n", pCallbackData->messageIdNumber);
    printf("Message: %s\n", pCallbackData->pMessage);
    printf("Object count: %d\n\n", pCallbackData->objectCount);
    for(uint32_t i = 0; i < pCallbackData->objectCount; i++){
        VkDebugUtilsObjectNameInfoEXT object = pCallbackData->pObjects[i];
        if(object.pObjectName == nullptr) object.pObjectName = "<unnamed>";
        printf("\tObj[%d]: type = %s, handle = 0x%llu, name = %s\n\n",
           i,
           vk_helper::object_type_to_str(object.objectType),
           (unsigned long long)object.objectHandle,
           object.pObjectName);
    }

    return VK_FALSE;
}

bool vk_debug::enable_debug_mode(str_array *layers, str_array *extensions){
#ifdef DEBUG
    if(!enable_layers(layers))
        return false;
#endif
    if(!enable_extensions(extensions))
        return false;

    return true;
}

bool vk_debug::initialize_debug_messenger(const VkInstance instance){
#ifdef DEBUG
    VkDebugUtilsMessengerCreateInfoEXT messenger_info {};
    messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messenger_info.pNext = NULL;
    messenger_info.flags = 0;
    messenger_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; 
    messenger_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
#if defined(_WIN32) || defined(__linux__)
    messenger_info.messageType |= VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
#endif
    messenger_info.pfnUserCallback = debug_callback;
    messenger_info.pUserData = NULL;

    PFN_vkCreateDebugUtilsMessengerEXT create_debug_messenger = nullptr;
    create_debug_messenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if(!create_debug_messenger){
        Core::debug::log(Core::debug::Fatal, "Couldn't create debug messenger\n");
        return false;
    }
    
    if(create_debug_messenger(instance, &messenger_info, NULL, &messenger) != VK_SUCCESS) 
        return false;
    
    //test function 
    PFN_vkSubmitDebugUtilsMessageEXT submit_debug_message = nullptr;
    submit_debug_message = (PFN_vkSubmitDebugUtilsMessageEXT)vkGetInstanceProcAddr(instance, "vkSubmitDebugUtilsMessageEXT");
    
    if(submit_debug_message){
        VkDebugUtilsMessengerCallbackDataEXT data {};
        data.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT;
        data.pNext = NULL;
        data.flags = 0;
        data.pMessageIdName = NULL;
        data.pMessage = "Hello from debug messenger";
        submit_debug_message(instance, VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT, VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT, &data);
    }else{
        Core::debug::log(Core::debug::Error, "Couldn't create test debug function\n");
        return false;
    }
#endif

    return true;
}

void vk_debug::destroy_debug_messenger(const VkInstance instance){
#ifdef DEBUG
    PFN_vkDestroyDebugUtilsMessengerEXT destroy_debug_messenger = nullptr;
    destroy_debug_messenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if(!destroy_debug_messenger){
        Core::debug::log(Core::debug::Fatal, "Couldn't create destroy debug messenger\n");
        return;
    }
    destroy_debug_messenger(instance, messenger, nullptr);
#endif

    return;
}

/**********************************************************************************
*
*
*               vk_helper
*
*
*************************************************************************************/

const char *vk_helper::object_type_to_str(const VkObjectType type){
    const char *message = nullptr;
    
    switch(type){
    case VK_OBJECT_TYPE_INSTANCE:
        message = "Instance";
    break;

    case VK_OBJECT_TYPE_PHYSICAL_DEVICE:
        message = "Physical Device";
    break;

    case VK_OBJECT_TYPE_DEVICE:
        message = "Logical Device";
    break;

    case VK_OBJECT_TYPE_QUEUE:
        message = "Queue";
    break;

    case VK_OBJECT_TYPE_SEMAPHORE:
        message = "Semaphore";
    break;

    case VK_OBJECT_TYPE_COMMAND_BUFFER:
        message = "Command Buffer";
    break;

    case VK_OBJECT_TYPE_FENCE:
        message = "Fence";
    break;

    case VK_OBJECT_TYPE_DEVICE_MEMORY:
        message = "Logical Device Memory";
    break;

    case VK_OBJECT_TYPE_BUFFER:
        message = "Buffer";
    break;

    case VK_OBJECT_TYPE_IMAGE:
        message = "Image";
    break;

    case VK_OBJECT_TYPE_EVENT:
        message = "Event";
    break;

    case VK_OBJECT_TYPE_QUERY_POOL:
        message = "Query Pool";
    break;

    case VK_OBJECT_TYPE_BUFFER_VIEW:
        message = "Buffer View";
    break;

    case VK_OBJECT_TYPE_IMAGE_VIEW:
        message = "Image View";
    break;

    case VK_OBJECT_TYPE_SHADER_MODULE:
        message = "Shader Module";
    break;

    case VK_OBJECT_TYPE_PIPELINE_CACHE:
        message = "Pipeline Cache";
    break;

    case VK_OBJECT_TYPE_PIPELINE_LAYOUT:
        message = "Pipeline Layout";
    break;

    case VK_OBJECT_TYPE_RENDER_PASS:
        message = "Render Pass";
    break; 

    case VK_OBJECT_TYPE_PIPELINE:
        message = "Graphics Pipeline";
    break;

    case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:
        message = "Set Layout";  
    break;

    case VK_OBJECT_TYPE_SAMPLER:
        message = "Sampler";
    break;

    case VK_OBJECT_TYPE_DESCRIPTOR_POOL:
        message = "Descriptor Pool";
    break;

    case VK_OBJECT_TYPE_DESCRIPTOR_SET:
        message = "Descriptor Set";
    break;

    case VK_OBJECT_TYPE_FRAMEBUFFER:
        message = "Framebuffer";
    break;

    case VK_OBJECT_TYPE_COMMAND_POOL:
        message = "Command Pool"; 
    break;

    case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE:
        message = "Descriptor Update Template";
    break;

    case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION:
        message = "Sampler YCBCR Conversion"; 
    break;

    case VK_OBJECT_TYPE_PRIVATE_DATA_SLOT:
        message = "Private Data Slot";
    break;

    case VK_OBJECT_TYPE_SURFACE_KHR:
        message = "Surface KHR";
    break;

    case VK_OBJECT_TYPE_SWAPCHAIN_KHR:
        message = "Swapchain KHR";
    break;

    case VK_OBJECT_TYPE_UNKNOWN:
    default:
        message = "Unknown";
    break;
    }

    return message;
}

const char *vk_helper::severity_to_str(VkDebugUtilsMessageSeverityFlagBitsEXT severity){
    const char *message;
    switch(severity){
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            message = "Error";
        break;
        
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            message = "Warning";
        break;
        
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            message = "Verbose";
        break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        default:
            message = "Info";
        break;
    };

    return message;
}

const char *vk_helper::msg_type_to_str(const VkDebugUtilsMessageTypeFlagsEXT message_types){
    const char *message;
    switch(message_types){
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            message = "Validation";
        break;

        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            message = "Performance";
        break;

        case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
            message = "Address Binding";
        break;

        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: 
        default:
            message = "General";
        break;
    }

    return message;
}

const char *vk_helper::vulkan_boolean_to_str(VkBool32 value){
    if(value == VK_TRUE) return "True";
    else return "False";
}

void vk_helper::version_to_str(uint32_t version, char *out_string, size_t string_size){
    uint32_t major = (version >> 22) & ((1u << 7) - 1);
    uint32_t minor = (version >> 12) & ((1u << 10) - 1);
    uint32_t patch = version & ((1u << 12) - 1);
    snprintf(out_string, string_size, "%d.%d.%d", major, minor, patch);

    return;
}

const char *vk_helper::device_type_to_str(VkPhysicalDeviceType type){
    const char *message = nullptr;

    switch(type){
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            message = "Integrated GPU";
        break;

        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            message = "Discrete GPU";
        break;

        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            message = "Virtual GPU";
        break;

        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            message = "CPU";
        break;

        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        default:
            message = "Other";
        break;
    }

    return message;
}

void vk_helper::print_physical_device_properties(const VkPhysicalDeviceProperties properties){
    char version_string[30] = {};
    version_to_str(properties.apiVersion, version_string, 30 * sizeof(char));
    Core::debug::log(Core::debug::Info, "API Version: %s\n", version_string);
    Core::debug::log(Core::debug::Info, "Driver Version: %d\n", properties.driverVersion);
    Core::debug::log(Core::debug::Info, "Vendor ID: %d\n", properties.vendorID);
    Core::debug::log(Core::debug::Info, "Device ID: %d\n", properties.deviceID);
    Core::debug::log(Core::debug::Info, "Device type: %s\n", device_type_to_str(properties.deviceType));
    Core::debug::log(Core::debug::Info, "Device Name: %s\n", properties.deviceName);

    return;
}

void vk_helper::print_physical_device_memory_properties(const VkPhysicalDeviceMemoryProperties memory_properties){
    Core::debug::log(Core::debug::Info, "Memory type count: %d\n", memory_properties.memoryTypeCount);
    Core::debug::log(Core::debug::Info, "Memory heap count: %d\n", memory_properties.memoryHeapCount);
    Core::debug::log(Core::debug::Info, "Memory heap size: %llu\n\n\n", memory_properties.memoryHeaps->size / (1024 * 1024));

    return;
}

void vk_helper::queue_flags_to_str(const VkQueueFlags flags, char *out_string, size_t string_size, int * const num_flags){
    snprintf(out_string, string_size, "");
    if(flags & VK_QUEUE_GRAPHICS_BIT){
        strcat(out_string, "Graphics, ");
        (*num_flags)++;
    }
    if(flags & VK_QUEUE_COMPUTE_BIT){
        strcat(out_string, "Compute, ");
        (*num_flags)++;
    }
    if(flags & VK_QUEUE_TRANSFER_BIT){
        strcat(out_string, "Transfer, ");
        (*num_flags)++;
    }
    if(flags & VK_QUEUE_SPARSE_BINDING_BIT){
        strcat(out_string, "Sparse Binding, ");
        (*num_flags)++;
    }
    if(flags & VK_QUEUE_PROTECTED_BIT){
        strcat(out_string, "Protected, ");
        (*num_flags)++;
    }
    if(flags & VK_QUEUE_VIDEO_DECODE_BIT_KHR){
        strcat(out_string, "Video Decode, ");
        (*num_flags)++;
    }
    if(flags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR){
        strcat(out_string, "Video Encode, ");
        (*num_flags)++;
    }
    if(flags & VK_QUEUE_OPTICAL_FLOW_BIT_NV){
        strcat(out_string, "Optical Flow, ");
        (*num_flags)++;
    }  

    return;
}

void vk_helper::print_queue_family_properties(uint32_t family_number, const VkQueueFamilyProperties queue_family_properties){
    Core::debug::log(Core::debug::Info, "Family %d\n", family_number);
    char flag_string[64] = {};
    int num_flags = 0;
    queue_flags_to_str(queue_family_properties.queueFlags, flag_string, 64 * sizeof(char), &num_flags);
    Core::debug::log(Core::debug::Info, "Number of flags: %d\nFlags: %s\n", num_flags, flag_string);
    Core::debug::log(Core::debug::Info, "Queue count: %d\n\n\n", queue_family_properties.queueCount);

    return;
}

void vk_helper::enumerate_features(const VkPhysicalDeviceFeatures features){
    Core::debug::log(Core::debug::Info, "Features:\n");

    Core::debug::log(Core::debug::Info, "Robust Buffer Access: %s\n", vulkan_boolean_to_str(features.robustBufferAccess));
    Core::debug::log(Core::debug::Info, "Full Draw Index Uint32: %s\n", vulkan_boolean_to_str(features.fullDrawIndexUint32));
    Core::debug::log(Core::debug::Info, "Image Cube Array: %s\n", vulkan_boolean_to_str(features.imageCubeArray));
    Core::debug::log(Core::debug::Info, "Independent Blend: %s\n", vulkan_boolean_to_str(features.independentBlend));
    Core::debug::log(Core::debug::Info, "Geometry Shader: %s\n", vulkan_boolean_to_str(features.geometryShader));
    Core::debug::log(Core::debug::Info, "Tesselation Shader: %s\n", vulkan_boolean_to_str(features.tessellationShader));
    Core::debug::log(Core::debug::Info, "Sample Rate Shading: %s\n", vulkan_boolean_to_str(features.sampleRateShading));
    Core::debug::log(Core::debug::Info, "Dual Source Blend: %s\n", vulkan_boolean_to_str(features.dualSrcBlend));
    Core::debug::log(Core::debug::Info, "Logic Op: %s\n", vulkan_boolean_to_str(features.logicOp));
    Core::debug::log(Core::debug::Info, "Multi Draw Inderect: %s\n", vulkan_boolean_to_str(features.multiDrawIndirect));
    Core::debug::log(Core::debug::Info, "Draw Indirect First Instance: %s\n", vulkan_boolean_to_str(features.drawIndirectFirstInstance));
    Core::debug::log(Core::debug::Info, "Depth Clamp: %s\n", vulkan_boolean_to_str(features.depthClamp));
    Core::debug::log(Core::debug::Info, "Depth Bias Clamp: %s\n", vulkan_boolean_to_str(features.depthBiasClamp));
    Core::debug::log(Core::debug::Info, "Fill Mode None Solid: %s\n", vulkan_boolean_to_str(features.fillModeNonSolid));
    Core::debug::log(Core::debug::Info, "Depth Bounds: %s\n", vulkan_boolean_to_str(features.depthBounds));
    Core::debug::log(Core::debug::Info, "Wide Lines: %s\n", vulkan_boolean_to_str(features.wideLines));
    Core::debug::log(Core::debug::Info, "Large Points: %s\n", vulkan_boolean_to_str(features.largePoints));
    Core::debug::log(Core::debug::Info, "Alpha To One: %s\n", vulkan_boolean_to_str(features.alphaToOne));
    Core::debug::log(Core::debug::Info, "Multi Viewport: %s\n", vulkan_boolean_to_str(features.multiViewport));
    Core::debug::log(Core::debug::Info, "Sampler Anisotropy: %s\n", vulkan_boolean_to_str(features.samplerAnisotropy));
    Core::debug::log(Core::debug::Info, "Texture Compression ETC2: %s\n", vulkan_boolean_to_str(features.textureCompressionETC2));
    Core::debug::log(Core::debug::Info, "Texture Compression ASTC_LDR: %s\n", vulkan_boolean_to_str(features.textureCompressionASTC_LDR));
    Core::debug::log(Core::debug::Info, "Texture Compression BC: %s\n", vulkan_boolean_to_str(features.textureCompressionBC));
    Core::debug::log(Core::debug::Info, "Occlusin Query Precise: %s\n", vulkan_boolean_to_str(features.occlusionQueryPrecise));
    Core::debug::log(Core::debug::Info, "Pipeline Statistics Query: %s\n", vulkan_boolean_to_str(features.pipelineStatisticsQuery));
    Core::debug::log(Core::debug::Info, "Vertex Pipeline Stores and Atomics: %s\n", vulkan_boolean_to_str(features.vertexPipelineStoresAndAtomics));
    Core::debug::log(Core::debug::Info, "Fragment Stores and Atomics: %s\n", vulkan_boolean_to_str(features.fragmentStoresAndAtomics));
    Core::debug::log(Core::debug::Info, "Shader Tessellation and Geometry Point Size: %s\n", vulkan_boolean_to_str(features.shaderTessellationAndGeometryPointSize));
    Core::debug::log(Core::debug::Info, "Shader Image Gather Extended: %s\n", vulkan_boolean_to_str(features.shaderImageGatherExtended));
    Core::debug::log(Core::debug::Info, "Shader Storage Image Extended Format: %s\n", vulkan_boolean_to_str(features.shaderStorageImageExtendedFormats));
    Core::debug::log(Core::debug::Info, "Shader Storage Image Multisample: %s\n", vulkan_boolean_to_str(features.shaderStorageImageMultisample));
    Core::debug::log(Core::debug::Info, "Shader Storage Image Read Without Format: %s\n", vulkan_boolean_to_str(features.shaderStorageImageReadWithoutFormat));
    Core::debug::log(Core::debug::Info, "Shader Storage Image Write Without Format: %s\n", vulkan_boolean_to_str(features.shaderStorageImageWriteWithoutFormat));
    Core::debug::log(Core::debug::Info, "Shader Uniform Buffer Array Dynamic Indexing: %s\n", vulkan_boolean_to_str(features.shaderUniformBufferArrayDynamicIndexing));
    Core::debug::log(Core::debug::Info, "Shader Sampled Image Array Dynamic Indexing: %s\n", vulkan_boolean_to_str(features.shaderSampledImageArrayDynamicIndexing));
    Core::debug::log(Core::debug::Info, "Shader Storage Buffer Array Dynamic Indexing: %s\n", vulkan_boolean_to_str(features.shaderStorageBufferArrayDynamicIndexing));
    Core::debug::log(Core::debug::Info, "Shader Storage Image Array Dynamic Indexing: %s\n", vulkan_boolean_to_str(features.shaderStorageImageArrayDynamicIndexing));
    Core::debug::log(Core::debug::Info, "Shader Clip Distance: %s\n", vulkan_boolean_to_str(features.shaderClipDistance));
    Core::debug::log(Core::debug::Info, "Shader Cull Distance: %s\n", vulkan_boolean_to_str(features.shaderCullDistance));
    Core::debug::log(Core::debug::Info, "Shader Float64: %s\n", vulkan_boolean_to_str(features.shaderFloat64));
    Core::debug::log(Core::debug::Info, "Shader Int64: %s\n", vulkan_boolean_to_str(features.shaderInt64));
    Core::debug::log(Core::debug::Info, "Shader Int16: %s\n", vulkan_boolean_to_str(features.shaderInt16));
    Core::debug::log(Core::debug::Info, "Shader Resource Residency: %s\n", vulkan_boolean_to_str(features.shaderResourceResidency));
    Core::debug::log(Core::debug::Info, "Shader Resource Min LOD: %s\n", vulkan_boolean_to_str(features.shaderResourceMinLod));
    Core::debug::log(Core::debug::Info, "Sparse Binding: %s\n", vulkan_boolean_to_str(features.sparseBinding));
    Core::debug::log(Core::debug::Info, "Sparse Residency Buffer: %s\n", vulkan_boolean_to_str(features.sparseResidencyBuffer));
    Core::debug::log(Core::debug::Info, "Sparse Residency Imagade 2D: %s\n", vulkan_boolean_to_str(features.sparseResidencyImage2D));
    Core::debug::log(Core::debug::Info, "Sparse Residency Image 3D: %s\n", vulkan_boolean_to_str(features.sparseResidencyImage3D));
    Core::debug::log(Core::debug::Info, "Sparse Residency 2 Samples: %s\n", vulkan_boolean_to_str(features.sparseResidency2Samples));
    Core::debug::log(Core::debug::Info, "Sparse Residency 4 Samples: %s\n", vulkan_boolean_to_str(features.sparseResidency4Samples));
    Core::debug::log(Core::debug::Info, "Sparse Residency 8 Samples: %s\n", vulkan_boolean_to_str(features.sparseResidency8Samples));
    Core::debug::log(Core::debug::Info, "Sparse Residency 16 Samples: %s\n", vulkan_boolean_to_str(features.sparseResidency16Samples));
    Core::debug::log(Core::debug::Info, "Sparse Residency Aliased: %s\n", vulkan_boolean_to_str(features.sparseResidencyAliased));
    Core::debug::log(Core::debug::Info, "Variable Multisample Rate: %s\n", vulkan_boolean_to_str(features.variableMultisampleRate));
    Core::debug::log(Core::debug::Info, "Inherited Queries: %s\n", vulkan_boolean_to_str(features.inheritedQueries));

    return;
}

bool vk_helper::is_feature_enabled(const VkPhysicalDeviceFeatures features, Available_Features feature_to_enable){
    switch(feature_to_enable){
        case Geometry_Shader:
            if(features.geometryShader == VK_TRUE)
                return true;
        break;
    }

    return false;
}

void vk_helper::enable_feature(VkPhysicalDeviceFeatures *features, Available_Features feature_to_enable){
    switch(feature_to_enable){
        case Geometry_Shader:
            features->geometryShader = VK_TRUE;
        break;
    }

    return;
}

void vk_helper::transform_flags_to_str(const VkSurfaceTransformFlagsKHR flags, char *out_string, size_t string_size){
    memset(out_string, 0, string_size);

    if(flags & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) strcat(out_string, "Identity, ");
    if(flags & VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR) strcat(out_string, "Rotate 90, ");
    if(flags & VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR) strcat(out_string, "Rotate 180, ");
    if(flags & VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR) strcat(out_string, "Rotate 270, ");
    if(flags & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR) strcat(out_string, "Horizontal Mirror, ");
    if(flags & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR) strcat(out_string, "Horizontal mirror rotate 90, ");
    if(flags & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR) strcat(out_string, "Horizontal mirror rotate 180, ");
    if(flags & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR ) strcat(out_string, "Horizontal mirror rotate 270, ");
    if(flags & VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR) strcat(out_string, "Inherit, ");

    return;
}

const char *vk_helper::get_current_transform_flag(const VkSurfaceTransformFlagBitsKHR flag){
    if(flag & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) return "Identity, ";
    if(flag & VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR) return "Rotate 90, ";
    if(flag & VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR) return "Rotate 180, ";
    if(flag & VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR) return "Rotate 270, ";
    if(flag & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR) return "Horizontal Mirror, ";
    if(flag & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR) return "Horizontal mirror rotate 90, ";
    if(flag & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR) return "Horizontal mirror rotate 180, ";
    if(flag & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR ) return "Horizontal mirror rotate 270, ";
    if(flag & VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR) return "Inherit, ";

    return "";
}

void vk_helper::alpha_flags_to_str(const VkCompositeAlphaFlagsKHR flags, char *out_string, size_t string_size){
    memset(out_string, 0, string_size);
    
    if(flags & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) strcat(out_string, "Opaque, ");
    if(flags & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR) strcat(out_string, "Pre Multiplied, ");
    if(flags & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR) strcat(out_string, "Post Multipled, ");
    if(flags & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR) strcat(out_string, "Inherit, ");

    return;
}

void vk_helper::usage_flags_to_str(const VkImageUsageFlags flags, char *out_string, size_t string_size){
    memset(out_string, 0, string_size);

    if(flags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) strcat(out_string, "Transfer Source, ");
    if(flags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) strcat(out_string, "Transfer Destination, ");
    if(flags & VK_IMAGE_USAGE_SAMPLED_BIT) strcat(out_string, "Sampled, ");
    if(flags & VK_IMAGE_USAGE_STORAGE_BIT) strcat(out_string, "Storage, ");
    if(flags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) strcat(out_string, "Color Attachment, ");
    if(flags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) strcat(out_string, "Depth Stencil, ");
    if(flags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) strcat(out_string, "Transient Attachment, ");
    if(flags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) strcat(out_string, "Input Attachment, ");
    if(flags & VK_IMAGE_USAGE_HOST_TRANSFER_BIT) strcat(out_string, "Host transfer, ");
    if(flags & VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR) strcat(out_string, "Video Decode Destination, ");
    if(flags & VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR) strcat(out_string, "Video Decode Source, ");
    if(flags & VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR) strcat(out_string, "Video Decode DPB(Decode Picture Buffer), ");
    if(flags & VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT) strcat(out_string, "Fragment Density, ");
    if(flags & VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR) strcat(out_string, "Fragment Shading, ");
    if(flags & VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR) strcat(out_string, "Video Encode Destination, ");
    if(flags & VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR) strcat(out_string, "Video Encode Source, ");
    if(flags & VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR) strcat(out_string, "Video Encode DPB(Decode Picture Buffer), ");
    if(flags & VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT) strcat(out_string, "Attachment FeedBack Loop, ");
    if(flags & VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI) strcat(out_string, "Invocation Mask Huawei, ");
    if(flags & VK_IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM) strcat(out_string, "Sample Weight, ");
    if(flags & VK_IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM) strcat(out_string, "Sample Block Match, ");
    if(flags & VK_IMAGE_USAGE_TENSOR_ALIASING_BIT_ARM) strcat(out_string, "Tensor Aliasing ARM, ");
    if(flags & VK_IMAGE_USAGE_TILE_MEMORY_BIT_QCOM) strcat(out_string, "Tile Memory, ");
    if(flags & VK_IMAGE_USAGE_VIDEO_ENCODE_QUANTIZATION_DELTA_MAP_BIT_KHR) strcat(out_string, "Video Encode Quantization, ");
    if(flags & VK_IMAGE_USAGE_VIDEO_ENCODE_EMPHASIS_MAP_BIT_KHR) strcat(out_string, "Video Encode Emphasis, ");
    if(flags & VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV) strcat(out_string, "Fragment Shading (From NV), ");
    if(flags & VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT) strcat(out_string, "Host transfer (From EXT), ");

    return;
}

void vk_helper::enumerate_device_surface_capabilities(const VkSurfaceCapabilitiesKHR surface_capabilities){
    Core::debug::log(Core::debug::Info, "Min Image count: %d\n", surface_capabilities.minImageCount);
    Core::debug::log(Core::debug::Info, "Max Image count: %d\n", surface_capabilities.maxImageCount);
    Core::debug::log(Core::debug::Info, "Current Image Extent: (width = %d, height %d)\n", surface_capabilities.currentExtent.width, surface_capabilities.currentExtent.height);
    Core::debug::log(Core::debug::Info, "Min Image Extent: (width = %d, height %d)\n", surface_capabilities.minImageExtent.width, surface_capabilities.minImageExtent.height);
    Core::debug::log(Core::debug::Info, "Max Image Extent: (width = %d, height %d)\n", surface_capabilities.maxImageExtent.width, surface_capabilities.maxImageExtent.height);
    Core::debug::log(Core::debug::Info, "Max Image Layers: %d\n", surface_capabilities.maxImageArrayLayers);
    char string[1024] = {};
    transform_flags_to_str(surface_capabilities.supportedTransforms, string, 1024 * sizeof(char));
    Core::debug::log(Core::debug::Info, "Transform flags: %s\n", string);
    Core::debug::log(Core::debug::Info, "Current flag: %s\n", get_current_transform_flag(surface_capabilities.currentTransform));
    alpha_flags_to_str(surface_capabilities.supportedCompositeAlpha, string, 1024 * sizeof(char));
    Core::debug::log(Core::debug::Info, "Alpha flags: %s\n", string);
    usage_flags_to_str(surface_capabilities.supportedUsageFlags, string, 1024 * sizeof(char));
    Core::debug::log(Core::debug::Info, "Usage flags: %s\n", string);

    return;
}

bool vk_helper::choose_alpha_flag(VkCompositeAlphaFlagBitsKHR *alpha_flags, Available_Alpha_Flags flag_to_add){
    if(alpha_flags == NULL)
        return false;

    switch(flag_to_add){
        case ALPHA_FLAG_OPAQUE:
            *alpha_flags = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        break;
    }

    return true;
}

bool vk_helper::choose_usage_flag(VkImageUsageFlags *usage_flags, Available_Usage_Flags chosen_flag){
    if(usage_flags == NULL)
        return false;

    switch(chosen_flag){
        case USAGE_FLAG_COLOR_ATTACHMENT:
            *usage_flags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        break;
    }

    return true;
}

void vk_helper::enumerate_device_surface_formats(VkSurfaceFormatKHR *surface_formats, uint32_t num_formats){
    for(uint32_t i = 0; i < num_formats; i++){
        Core::debug::log(Core::debug::Info, "Surface[%d]: Format = %d, Color = %d\n", i, surface_formats[i].format, surface_formats[i].colorSpace);
    }

    return;
}

const char *vk_helper::present_mode_to_str(const VkPresentModeKHR flag){
    switch(flag){
        case VK_PRESENT_MODE_MAILBOX_KHR:
            return "MailBox";
        break;

        case VK_PRESENT_MODE_FIFO_KHR:
            return "FIFO";
        break;

        case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
            return "FIFO Relaxed";
        break;

        case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
            return "Shared Demand";
        break;

        case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
            return "Shared Continuos";
        break;

        case VK_PRESENT_MODE_FIFO_LATEST_READY_KHR:
            return "FIFO Latest Ready";
        break;

        case VK_PRESENT_MODE_IMMEDIATE_KHR:
        default:
            return "Immediate";
        break;
    }
}

void vk_helper::enumerate_device_present_modes(const VkPresentModeKHR *present_modes, const uint32_t num_present_modes){
    for(uint32_t i = 0; i < num_present_modes; i++){
        Core::debug::log(Core::debug::Info, "Preset[%d]: Mode : %s\n", i, vk_helper::present_mode_to_str(present_modes[i]));
    }

    return;
}