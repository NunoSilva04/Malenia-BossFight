#include "Graphics.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>
#include "core.h"

//Layers i am interested in:
static const char *layer_names[] = {
    "VK_LAYER_KHRONOS_validation",
    "VK_LAYER_LUNARG_crash_diagnostic"
};

//Features enabled of the device
enum CoreFeatures{
    Geometry_Shader,
    Tesselation_Shader,
};

static const char *vulkan_boolean_to_str(VkBool32 value){
    if(value == VK_TRUE) return "True";
    else return "False";
}

static const char *severity_to_str(VkDebugUtilsMessageSeverityFlagBitsEXT severity){
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

static const char *msg_type_to_str(const VkDebugUtilsMessageTypeFlagsEXT message_types){
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

static void version_to_str(uint32_t version, char *out_string, size_t string_size){
    uint32_t major = (version >> 22) & ((1u << 7) - 1);
    uint32_t minor = (version >> 12) & ((1u << 10) - 1);
    uint32_t patch = version & ((1u << 12) - 1);
    snprintf(out_string, string_size, "%d.%d.%d", major, minor, patch);

    return;
}

static const char *device_type_to_str(VkPhysicalDeviceType type){
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

static void queue_flags_to_str(const VkQueueFlags flags, char *out_string, size_t string_size, int * const num_flags){
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

static void transform_flags_to_str(const VkSurfaceTransformFlagsKHR flags, char *out_string, size_t string_size){
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

static const char *get_current_transform_flag(const VkSurfaceTransformFlagBitsKHR flag){
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

static void alpha_flags_to_str(const VkCompositeAlphaFlagsKHR flags, char *out_string, size_t string_size){
    memset(out_string, 0, string_size);
    
    if(flags & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) strcat(out_string, "Opaque, ");
    if(flags & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR) strcat(out_string, "Pre Multiplied, ");
    if(flags & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR) strcat(out_string, "Post Multipled, ");
    if(flags & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR) strcat(out_string, "Inherit, ");

    return;
}

static void usage_flags_to_str(const VkImageUsageFlags flags, char *out_string, size_t string_size){
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

static const char *present_mode_to_str(const VkPresentModeKHR flag){
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

static void enumerate_available_features(const VkPhysicalDeviceFeatures features){
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

static bool device_has_feature(const VkPhysicalDeviceFeatures available_features, const CoreFeatures current_feature){
    switch(current_feature){
        case Geometry_Shader:
            if(available_features.geometryShader == VK_TRUE) return true;
        break;

        case Tesselation_Shader:
            if(available_features.tessellationShader == VK_TRUE) return true;
        break;

        default:
            return false;
        break;
    }

    return false;
}

static void device_enable_feature(VkPhysicalDeviceFeatures *features, const CoreFeatures feature_to_enable){
    switch(feature_to_enable){
        case Geometry_Shader:
            features->geometryShader = VK_TRUE;
        break;

        case Tesselation_Shader:
            features->tessellationShader = VK_TRUE;
        break;

        default:
            return;
        break;
    }

    return;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
    VkDebugUtilsMessageTypeFlagsEXT             message_types,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void*                                       pUserData)
{
    printf("Severity: %s\n", severity_to_str(message_severity));
    printf("Type: %s\n", msg_type_to_str(message_types));
    printf("Message Id: %d\n", pCallbackData->messageIdNumber);
    printf("Message: %s\n\n", pCallbackData->pMessage);
    
    return VK_FALSE;
}

Graphics::Graphics(){

}

Graphics::~Graphics(){

}

bool Graphics::initialize_graphics(SDL_Window *window, Core::n_vector<const char *> extensions){
    Core::n_vector<const char *> layers;
    #ifdef DEBUG
        layers = get_validation_layers();
        add_debug_extension(extensions);
    #endif

    if(!initialize_instance(extensions, layers)){
        Core::debug::log(Core::debug::Error, "Couldn't initialize instance\n");
        return false;
    } 
    #ifdef DEBUG
        if(!initialize_debug_messenger()){
            Core::debug::log(Core::debug::Error, "Couldn't initialize debug messenger\n");
            return false;
        }
    #endif
    if(!initialize_surface(window)){
        Core::debug::log(Core::debug::Fatal, "Couldn't initialize surface\n");
        return false;
    }
    if(!initialize_device()){
        Core::debug::log(Core::debug::Fatal, "Couldn't initialize device\n");
        return false;
    }

    return true;
}

Core::n_vector<const char *> Graphics::get_validation_layers(void){
    Core::n_vector<const char *> layers;

    uint32_t all_layers = 0;
    if(vkEnumerateInstanceLayerProperties(&all_layers, NULL) != VK_SUCCESS) return layers;
    VkLayerProperties *properties = new VkLayerProperties[all_layers];
    if(vkEnumerateInstanceLayerProperties(&all_layers, properties) == VK_SUCCESS){
        for(uint32_t i = 0; i < all_layers; i++){            
            if(strcmp("VK_LAYER_KHRONOS_validation", properties[i].layerName) == 0){
                layers.push_back(layer_names[0]);
                continue;
            }else if(strcmp("VK_LAYER_LUNARG_crash_diagnostic", properties[i].layerName) == 0){
                layers.push_back(layer_names[1]);
            }
        }
    }

    delete[] properties;
    return layers;
}

void Graphics::add_debug_extension(Core::n_vector<const char *> &extensions){
    extensions.push_back("VK_EXT_debug_utils");
    
    return;
}

bool Graphics::initialize_instance(Core::n_vector<const char *> extensions, Core::n_vector<const char *> layers){
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
    create_info.flags = 0;
    create_info.pApplicationInfo = &application_info;
    #ifdef DEBUG
        create_info.enabledLayerCount = (uint32_t) layers.vector_size();
        create_info.ppEnabledLayerNames = layers.vector_data();
    #else
        create_info.enabledLayerCount = 0;
        create_info.ppEnabledLayerNames = NULL;
    #endif
    create_info.enabledExtensionCount = (uint32_t) extensions.vector_size();
    create_info.ppEnabledExtensionNames = extensions.vector_data();

    if(vkCreateInstance(&create_info, nullptr, &vk_instance) != VK_SUCCESS) return false;
    
    return true;
}

bool Graphics::initialize_debug_messenger(void){
    VkDebugUtilsMessengerCreateInfoEXT messenger_info {};
    messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messenger_info.pNext = NULL;
    messenger_info.flags = 0;
    messenger_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ; 
    messenger_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
    messenger_info.pfnUserCallback = debug_callback;
    messenger_info.pUserData = NULL;

    PFN_vkCreateDebugUtilsMessengerEXT create_debug_messenger = nullptr;
    create_debug_messenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vk_instance, "vkCreateDebugUtilsMessengerEXT");
    if(!create_debug_messenger){
        Core::debug::log(Core::debug::Fatal, "Couldn't create debug messenger\n");
    }
    
    if(create_debug_messenger(vk_instance, &messenger_info, NULL, &messenger) != VK_SUCCESS) return false;

    //test function 
    PFN_vkSubmitDebugUtilsMessageEXT submit_debug_message = nullptr;
    submit_debug_message = (PFN_vkSubmitDebugUtilsMessageEXT)vkGetInstanceProcAddr(vk_instance, "vkSubmitDebugUtilsMessageEXT");
    
    if(submit_debug_message){
        VkDebugUtilsMessengerCallbackDataEXT data {};
        data.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT;
        data.pNext = NULL;
        data.flags = 0;
        data.pMessageIdName = NULL;
        data.pMessage = "Hello from debug messenger";

        submit_debug_message(vk_instance, VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT, VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT, &data);
    }

    return true;
}

bool Graphics::initialize_surface(SDL_Window *window){
    if(!SDL_Vulkan_CreateSurface(window, vk_instance, nullptr, &vk_surface)) return false;

    return true;
}

VkPhysicalDevice Graphics::get_best_device(void){
    VkPhysicalDevice chosen_device = nullptr;

    uint32_t num_devices = 0;
    if(vkEnumeratePhysicalDevices(vk_instance, &num_devices, NULL) != VK_SUCCESS) return nullptr;
    VkPhysicalDevice *devices = new VkPhysicalDevice[num_devices];
    if(vkEnumeratePhysicalDevices(vk_instance, &num_devices, devices) == VK_SUCCESS){
        VkPhysicalDeviceProperties properties;
        uint64_t max_vram = 0;
        bool has_dpgu = false;
        for(size_t i = 0; i < num_devices; i++){
            vkGetPhysicalDeviceProperties(devices[i], &properties);

            char version_string[30] = {};
            version_to_str(properties.apiVersion, version_string, 30 * sizeof(char));
            Core::debug::log(Core::debug::Info, "API Version: %s\n", version_string);
            Core::debug::log(Core::debug::Info, "Driver Version: %d\n", properties.driverVersion);
            Core::debug::log(Core::debug::Info, "Vendor ID: %d\n", properties.vendorID);
            Core::debug::log(Core::debug::Info, "Device ID: %d\n", properties.deviceID);
            Core::debug::log(Core::debug::Info, "Device type: %s\n", device_type_to_str(properties.deviceType));
            Core::debug::log(Core::debug::Info, "Device Name: %s\n", properties.deviceName);

            VkPhysicalDeviceMemoryProperties mem_properties;
            vkGetPhysicalDeviceMemoryProperties(devices[i], &mem_properties);
            Core::debug::log(Core::debug::Info, "Memory type count: %d\n", mem_properties.memoryTypeCount);
            Core::debug::log(Core::debug::Info, "Memory heap count: %d\n", mem_properties.memoryHeapCount);
            Core::debug::log(Core::debug::Info, "Memory heap size: %llu\n\n\n", mem_properties.memoryHeaps->size / (1024 * 1024));

            if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
                has_dpgu = true;
                if(mem_properties.memoryHeaps->size > max_vram){
                    chosen_device = devices[i];
                    max_vram = mem_properties.memoryHeaps->size;
                }
            }else{
                if(has_dpgu) continue;
                else{
                    chosen_device = devices[i];
                }
            }
        }
    }
    delete[] devices;

    return chosen_device;
}

void Graphics::choose_queue_family(const VkPhysicalDevice device, uint32_t *chosen_index, uint32_t *chosen_count){
    uint32_t num_queues = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &num_queues, NULL);
    if(num_queues > 0){
        VkQueueFamilyProperties *properties = new VkQueueFamilyProperties[num_queues];
        vkGetPhysicalDeviceQueueFamilyProperties(device, &num_queues, properties);

        for(uint32_t i = 0; i < num_queues; i++){
            Core::debug::log(Core::debug::Info, "Family %d\n", i);
            char flag_string[64] = {};
            int num_flags = 0;
            queue_flags_to_str(properties[i].queueFlags, flag_string, 64 * sizeof(char), &num_flags);
            Core::debug::log(Core::debug::Info, "Number of flags: %d\nFlags: %s\n", num_flags, flag_string);
            Core::debug::log(Core::debug::Info, "Queue count: %d\n\n\n", properties[i].queueCount);

            bool has_flag = [&](const VkQueueFlags flag){
                if(properties[i].queueFlags & flag) return true;
                return false;
            }(VK_QUEUE_GRAPHICS_BIT);
            if(has_flag){
                *chosen_index = i;
                *chosen_count = properties[i].queueCount;
            }
        }

        delete[] properties;
    }
    
    return;
}

bool Graphics::get_device_extensions(const VkPhysicalDevice device, Core::n_vector<const char *> *extensions, uint32_t *num_extensions){
    bool found_extension = false;

    uint32_t total_extensions = 0;
    if(vkEnumerateDeviceExtensionProperties(device, NULL, &total_extensions, NULL) != VK_SUCCESS) return false;
    VkExtensionProperties *properties = new VkExtensionProperties[total_extensions];
    if(vkEnumerateDeviceExtensionProperties(device, NULL, &total_extensions, properties) == VK_SUCCESS){
        for(uint32_t i = 0; i < total_extensions; i++){
            if(strcmp(properties[i].extensionName, "VK_KHR_swapchain") == 0){
                extensions->push_back("VK_KHR_swapchain");
                (*num_extensions)++;
                found_extension = true;
                break;
            } 
        }
    }

    if(found_extension == false) return false;
    
    delete[] properties;
    return true;
}

bool Graphics::validate_device_surface(const VkPhysicalDevice device, const uint32_t family_index){
    // Validate Device and Surface Support
    VkBool32 is_supported;
    if(vkGetPhysicalDeviceSurfaceSupportKHR(device, family_index, vk_surface, &is_supported) != VK_SUCCESS) return false;
    if(is_supported == VK_FALSE){
        Core::debug::log(Core::debug::Fatal, "Device is not supported for the presentation");
        return false;
    } 

    // Validate Device and Surface capabilites (The height and widht of course change depending on the screen)
    VkSurfaceCapabilitiesKHR capabilities;
    if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vk_surface, &capabilities) == VK_SUCCESS){
        Core::debug::log(Core::debug::Info, "Min Image count: %d\n", capabilities.minImageCount);
        Core::debug::log(Core::debug::Info, "Max Image count: %d\n", capabilities.maxImageCount);
        Core::debug::log(Core::debug::Info, "Current Image Extent: (width = %d, height %d)\n", capabilities.currentExtent.width, capabilities.currentExtent.height);
        Core::debug::log(Core::debug::Info, "Min Image Extent: (width = %d, height %d)\n", capabilities.minImageExtent.width, capabilities.minImageExtent.height);
        Core::debug::log(Core::debug::Info, "Max Image Extent: (width = %d, height %d)\n", capabilities.maxImageExtent.width, capabilities.maxImageExtent.height);
        Core::debug::log(Core::debug::Info, "Max Image Layers: %d\n", capabilities.maxImageArrayLayers);
        char string[1024] = {};
        transform_flags_to_str(capabilities.supportedTransforms, string, 1024 * sizeof(char));
        Core::debug::log(Core::debug::Info, "Transform flags: %s\n", string);
        Core::debug::log(Core::debug::Info, "Current flag: %s\n", get_current_transform_flag(capabilities.currentTransform));
        alpha_flags_to_str(capabilities.supportedCompositeAlpha, string, 1024 * sizeof(char));
        Core::debug::log(Core::debug::Info, "Alpha flags: %s\n", string);
        usage_flags_to_str(capabilities.supportedUsageFlags, string, 1024 * sizeof(char));
        Core::debug::log(Core::debug::Info, "Usage flags: %s\n", string);
    }else{
        Core::debug::log(Core::debug::Error, "Couldn't get physical device surface capabilites\n");
        return false;
    }

    // Validate Device and Surface formats (The number of formats change depending on the screen that is used)
    uint32_t num_formats = 0;
    if(vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_surface, &num_formats, NULL) != VK_SUCCESS) return false;
    VkSurfaceFormatKHR *surface_formats = new VkSurfaceFormatKHR[num_formats];
    
    if(vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_surface, &num_formats, surface_formats) == VK_SUCCESS){
        for(uint32_t i = 0; i < num_formats; i++){
            Core::debug::log(Core::debug::Info, "Surface %d\n", i);
            Core::debug::log(Core::debug::Info, "Format = %d\n", surface_formats[i].format);
            Core::debug::log(Core::debug::Info, "Color = %d\n\n\n", surface_formats[i].colorSpace);
        }
    }
    delete[] surface_formats;
    
    // Validate Device and Surface Present modes
    uint32_t num_present_modes = 0;
    if(vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_surface, &num_present_modes, NULL) != VK_SUCCESS) return false;
    VkPresentModeKHR *present_modes = new VkPresentModeKHR[num_present_modes];
    if(vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_surface, &num_present_modes, present_modes) == VK_SUCCESS){
        for(uint32_t i = 0; i < num_present_modes; i++){
            Core::debug::log(Core::debug::Info, "Preset %d\n", i);
            Core::debug::log(Core::debug::Info, "Mode: %s\n\n\n", present_mode_to_str(present_modes[i]));
        }
    }
    delete[] present_modes;

    return true;
}

bool Graphics::add_device_features(VkPhysicalDeviceFeatures *features, const VkPhysicalDevice device, uint32_t num_features, ...){
    VkPhysicalDeviceFeatures available_features {};
    vkGetPhysicalDeviceFeatures(device, &available_features);
    enumerate_available_features(available_features);
    
    va_list args;
    va_start(args, num_features);
    for(uint32_t i = 0; i < num_features; i++){
        CoreFeatures retreived_feature = va_arg(args, CoreFeatures);
        if(device_has_feature(available_features, retreived_feature) == true){
            device_enable_feature(features, retreived_feature);
        }else{
            va_end(args);
            return false;
        }
    }
    va_end(args);

    return true;
}

bool Graphics::initialize_device(void){
    VkPhysicalDevice chosen_device = get_best_device();
    uint32_t chosen_queue_index = 0, chosen_queue_count = 0;
    choose_queue_family(chosen_device, &chosen_queue_index, &chosen_queue_count);
    Core::n_vector<const char *> device_extensions;
    uint32_t num_extensions = 0;
    if(!get_device_extensions(chosen_device, &device_extensions, &num_extensions)) return false;
    if(!validate_device_surface(chosen_device, chosen_queue_index)) return false;
    VkPhysicalDeviceFeatures device_features {};
    if(!add_device_features(&device_features, chosen_device, 2, Geometry_Shader, Tesselation_Shader)) return false;

    float priority = 1.0f;
    VkDeviceQueueCreateInfo queue_create_info {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.pNext = NULL;
    queue_create_info.flags = 0;
    queue_create_info.queueFamilyIndex = chosen_queue_index;
    queue_create_info.queueCount = chosen_queue_count;
    queue_create_info.pQueuePriorities = &priority; 

    VkDeviceCreateInfo device_create_info {};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pNext = NULL;
    device_create_info.flags = 0;
    device_create_info.queueCreateInfoCount = 1;
    device_create_info.pQueueCreateInfos = &queue_create_info;
    device_create_info.enabledLayerCount = 0;
    device_create_info.ppEnabledLayerNames = NULL;
    device_create_info.enabledExtensionCount = num_extensions;
    device_create_info.ppEnabledExtensionNames = device_extensions.vector_data();
    device_create_info.pEnabledFeatures = &device_features;


    if(vkCreateDevice(chosen_device, &device_create_info, NULL, &logical_device) != VK_SUCCESS) return false;

    return true;
}

void Graphics::destroy_device(void){
    vkDestroyDevice(logical_device, NULL);

    return;
}

void Graphics::destroy_surface(void){
    SDL_Vulkan_DestroySurface(vk_instance, vk_surface, nullptr);

    return;
}

void Graphics::close_debug_messenger(void){
    #ifdef DEBUG
        PFN_vkDestroyDebugUtilsMessengerEXT destroy_debug_messenger = nullptr;
        destroy_debug_messenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vk_instance, "vkDestroyDebugUtilsMessengerEXT");
        if(!destroy_debug_messenger){
            Core::debug::log(Core::debug::Fatal, "Couldn't create destroy debug messenger\n");
        }
        destroy_debug_messenger(vk_instance, messenger, nullptr);
    #endif
    return;
}

void Graphics::close_graphics(void){
    destroy_device();
    destroy_surface();
    close_debug_messenger();
    vkDestroyInstance(vk_instance, nullptr);

    return;
}