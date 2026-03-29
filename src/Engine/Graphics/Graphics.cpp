#include "Graphics.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>
#include "core.h"

// Layers i am interested in:
static const char *layer_names[] = {
    "VK_LAYER_KHRONOS_validation",
    "VK_LAYER_LUNARG_crash_diagnostic"
};

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

enum Alpha_Flags{
    OPAQUE, 
    PRE_MULTIPLED,
    POST_MULTIPLED,
    INHERITED,
};

static VkDynamicState dynamic_states[] = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR,
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

static void enumerate_device_capabilities(const VkSurfaceCapabilitiesKHR capabilities){
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

    return;
}

static VkImageUsageFlags get_swapchain_usage_flags(const VkImageUsageFlags flags, const uint32_t num_flags, ...){
    VkImageUsageFlags swapchain_flags = 0;

    va_list args;
    va_start(args, num_flags);
    for(uint32_t i = 0; i < num_flags; i++){
        Usage_Flags asked_flag = va_arg(args, Usage_Flags);
        switch(asked_flag){
            case COLOR_ATTACHMENT:
                if(flags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) swapchain_flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            break;

            case TRANSFER_DESTINATION:
                if(flags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) swapchain_flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            break;

            default:
                Core::debug::log(Core::debug::Error, "Flag not specified or not an available flag\n");
            break;
        }
    }
    va_end(args);

    return swapchain_flags ;
}

static VkCompositeAlphaFlagBitsKHR get_swapchain_alpha_flag(Alpha_Flags desired_flag){
    VkCompositeAlphaFlagBitsKHR chosen_flag;
    switch(desired_flag){
        case OPAQUE:
            chosen_flag = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        break;

        case PRE_MULTIPLED:
        case POST_MULTIPLED:
        case INHERITED:
        default:
            Core::debug::log(Core::debug::Error, "Flag has not been implemented or the flag is not valid. Default: Opaque flag chosen\n");
            chosen_flag = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        break;
    }

    return chosen_flag;
}

static void enumerate_device_formats(VkSurfaceFormatKHR *surface_formats, uint32_t num_formats){
    for(uint32_t i = 0; i < num_formats; i++){
        Core::debug::log(Core::debug::Info, "Surface %d\n", i);
        Core::debug::log(Core::debug::Info, "Format = %d\n", surface_formats[i].format);
        Core::debug::log(Core::debug::Info, "Color = %d\n\n\n", surface_formats[i].colorSpace);
    }

    return;
}

bool get_device_format_color_space(VkSurfaceFormatKHR *surface_formats, uint32_t num_formats, VkFormat desired_format, Graphics::Swapchain_Info *swapchain_info){
    bool found_format = false;

    for(uint32_t i = 0; i < num_formats; i++){
        if(desired_format == surface_formats[i].format){
            found_format = true;
            swapchain_info->format = surface_formats[i].format;
            swapchain_info->color_space = surface_formats[i].colorSpace;
            break;
        }
    }

    return found_format;
}

static void enumerate_device_present_modes(const VkPresentModeKHR *present_modes, const uint32_t num_present_modes){
    for(uint32_t i = 0; i < num_present_modes; i++){
        Core::debug::log(Core::debug::Info, "Preset %d\n", i);
        Core::debug::log(Core::debug::Info, "Mode: %s\n\n\n", present_mode_to_str(present_modes[i]));
    }

    return;
}

static VkPresentModeKHR get_present_mode(const VkPresentModeKHR *present_modes, const uint32_t num_present_modes, VkPresentModeKHR desired_preset){
    VkPresentModeKHR chosen_present;
    bool has_present = false;
    for(uint32_t i = 0; i < num_present_modes; i++){
        if(present_modes[i] == desired_preset){
            chosen_present = desired_preset;
            has_present = true;
            break;
        }
    }
    if(!has_present){
        Core::debug::log(Core::debug::Error, "Couldn't get the desired preset. Default: Immediate present chosen\n");
        chosen_present = VK_PRESENT_MODE_IMMEDIATE_KHR;
        return chosen_present;
    }else return chosen_present;
}

static bool create_shader_module(const VkDevice logical_device, VkShaderModule *shader_module, const char *file_name){
    Core::n_vector<char> file_data = Core::File::read_binary_file(file_name);

    VkShaderModuleCreateInfo shader_module_create_info {};
    shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info.pNext = NULL;
    shader_module_create_info.flags = 0;
    shader_module_create_info.codeSize = file_data.vector_size();
    shader_module_create_info.pCode = (const uint32_t *)file_data.vector_data();

    if(vkCreateShaderModule(logical_device, &shader_module_create_info, NULL, shader_module) != VK_SUCCESS) return false;

    return true;
}

static void destroy_shader_module(const VkDevice logical_device, VkShaderModule shader_module){
    vkDestroyShaderModule(logical_device, shader_module, NULL);

    return;
}

static VkPipelineShaderStageCreateInfo create_shader_stage_info(VkShaderStageFlagBits stage_flag, VkShaderModule module){
    VkPipelineShaderStageCreateInfo shader_stage_info {};
    shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_info.pNext = NULL;
    shader_stage_info.flags = 0;
    shader_stage_info.stage = stage_flag;
    shader_stage_info.module = module;
    shader_stage_info.pName = "main";
    shader_stage_info.pSpecializationInfo = NULL;

    return shader_stage_info;
}

VkPipelineDynamicStateCreateInfo create_dynamic_state_info(uint32_t num_states){    
    VkPipelineDynamicStateCreateInfo dynamic_state_info {};
    dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_info.pNext = NULL;
    dynamic_state_info.flags = 0;
    dynamic_state_info.dynamicStateCount = num_states;
    dynamic_state_info.pDynamicStates = dynamic_states;

    return dynamic_state_info;
}

VkPipelineVertexInputStateCreateInfo create_vertex_state_info(void){
    VkPipelineVertexInputStateCreateInfo vertex_state_info {};
    vertex_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_state_info.pNext = NULL;
    vertex_state_info.flags = 0;
    vertex_state_info.vertexBindingDescriptionCount = 0;
    vertex_state_info.pVertexBindingDescriptions = nullptr;
    vertex_state_info.vertexAttributeDescriptionCount = 0;
    vertex_state_info.pVertexAttributeDescriptions = 0;

    return vertex_state_info;
}

static VkPipelineInputAssemblyStateCreateInfo create_assembly_state_info(void){
    VkPipelineInputAssemblyStateCreateInfo assembly_state_info {};
    assembly_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    assembly_state_info.pNext = NULL;
    assembly_state_info.flags = 0;
    assembly_state_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    assembly_state_info.primitiveRestartEnable = VK_FALSE;

    return assembly_state_info; 
}

VkPipelineViewportStateCreateInfo create_viewport_state_info(const Graphics::Swapchain_Info swapchain_info){
    // Viewport
    static VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapchain_info.curr_image_extent.width);
    viewport.height = static_cast<float>(swapchain_info.curr_image_extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Scissors
    static VkRect2D scissors;
    scissors.offset = {0, 0};
    scissors.extent = swapchain_info.curr_image_extent;

    VkPipelineViewportStateCreateInfo viewport_state_info;
    viewport_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_info.pNext = NULL;
    viewport_state_info.flags = 0;
    viewport_state_info.viewportCount = 1;
    viewport_state_info.pViewports = &viewport;
    viewport_state_info.scissorCount = 1;
    viewport_state_info.pScissors = &scissors;

    return viewport_state_info;
}

static VkPipelineRasterizationStateCreateInfo create_rasterizer_state_info(void){
    VkPipelineRasterizationStateCreateInfo rasterizer_state_info;
    rasterizer_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer_state_info.pNext = NULL;
    rasterizer_state_info.flags = 0;
    rasterizer_state_info.depthClampEnable = VK_FALSE;
    rasterizer_state_info.rasterizerDiscardEnable = VK_FALSE;
    rasterizer_state_info.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer_state_info.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer_state_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer_state_info.depthBiasEnable = VK_FALSE;
    rasterizer_state_info.depthBiasConstantFactor = 0.0f;
    rasterizer_state_info.depthBiasClamp = 0.0f;
    rasterizer_state_info.depthBiasSlopeFactor = 0.0f;
    rasterizer_state_info.lineWidth = 1.0f;

    return rasterizer_state_info;
}  

static VkPipelineColorBlendStateCreateInfo create_color_blend_state_info(bool enabled){
    static VkPipelineColorBlendAttachmentState color_blend_attachement {};
    if(enabled){
        color_blend_attachement.blendEnable = VK_TRUE;
        color_blend_attachement.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        color_blend_attachement.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        color_blend_attachement.colorBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachement.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachement.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachement.alphaBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachement.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    }else{
        color_blend_attachement.blendEnable = VK_FALSE;
        color_blend_attachement.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachement.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachement.colorBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachement.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachement.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachement.alphaBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachement.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    }

    VkPipelineColorBlendStateCreateInfo color_blend_state_info {};
    color_blend_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_state_info.pNext = NULL;
    color_blend_state_info.flags = 0;
    color_blend_state_info.logicOpEnable = VK_FALSE;
    color_blend_state_info.logicOp = VK_LOGIC_OP_COPY;
    color_blend_state_info.attachmentCount = 1;
    color_blend_state_info.pAttachments = &color_blend_attachement;
    color_blend_state_info.blendConstants[0] = 0.0f;
    color_blend_state_info.blendConstants[1] = 0.0f;
    color_blend_state_info.blendConstants[2] = 0.0f;
    color_blend_state_info.blendConstants[3] = 0.0f;
    
    return color_blend_state_info;
}

static VkPipelineMultisampleStateCreateInfo create_multisample_state_info(void){
    VkPipelineMultisampleStateCreateInfo multisample_state_info {};
    multisample_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state_info.pNext = NULL;
    multisample_state_info.flags = 0;
    multisample_state_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisample_state_info.sampleShadingEnable = VK_FALSE;
    multisample_state_info.minSampleShading = 1.0f;
    multisample_state_info.pSampleMask = nullptr;
    multisample_state_info.alphaToCoverageEnable = VK_FALSE;
    multisample_state_info.alphaToOneEnable = VK_FALSE;

    return multisample_state_info;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
    VkDebugUtilsMessageTypeFlagsEXT             message_types,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void*                                       pUserData)
{
    (void) pUserData;  // This is just because i am getting annoyed at the warning generated when compiled for the unused parameter
    printf("Severity: %s\n", severity_to_str(message_severity));
    printf("Type: %s\n", msg_type_to_str(message_types));
    printf("Message Id: %d\n", pCallbackData->messageIdNumber);
    printf("Message: %s\n\n", pCallbackData->pMessage);
    
    return VK_FALSE;
}

Graphics::Graphics(){
    vk_instance = nullptr;
    messenger = nullptr;
    vk_surface = nullptr;
    physical_device_info = {};
    logical_device = nullptr;
    logical_device_queue = nullptr;
    swapchain_info = {};
    swapchain = nullptr;
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
    if(!initialize_device_get_queue_handle()){
        Core::debug::log(Core::debug::Fatal, "Couldn't initialize device\n");
        return false;
    }
    if(!create_swapchain()){
        Core::debug::log(Core::debug::Fatal, "Couldn't create Swap Chain\n");
        return false;
    }
    if(!create_images_and_image_views()){
        Core::debug::log(Core::debug::Fatal, "Couldn't create images and image views\n");
        return false;
    }
    if(!create_graphics_pipeline()){
        Core::debug::log(Core::debug::Fatal, "Couldn't create pipeline\n");
        return false;
    }
    if(!create_frame_buffers()){
        Core::debug::log(Core::debug::Fatal, "Couldn't create frame buffers\n");
        return false;
    }
    if(!create_command_pool_and_buffer()){
        Core::debug::log(Core::debug::Fatal, "Couldn't create Command Pool and buffers\n");
        return false;
    }
    if(!create_sync_objects()){
        Core::debug::log(Core::debug::Fatal, "Couldn't create Sync objects\n");
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

bool Graphics::get_best_device(void){
    VkPhysicalDevice temp_device = nullptr;
    uint32_t num_devices = 0;
    if(vkEnumeratePhysicalDevices(vk_instance, &num_devices, NULL) != VK_SUCCESS) return false;
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
                    temp_device = devices[i];
                    max_vram = mem_properties.memoryHeaps->size;
                }
            }else{
                if(has_dpgu) continue;
                else{
                    temp_device = devices[i];
                }
            }
        }
    }
    delete[] devices;

    physical_device_info.device = temp_device;
    return true;
}

bool Graphics::choose_queue_family(void){
    uint32_t num_queues = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device_info.device, &num_queues, NULL);
    if(num_queues > 0){
        VkQueueFamilyProperties *properties = new VkQueueFamilyProperties[num_queues];
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device_info.device, &num_queues, properties);

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
                physical_device_info.queue_index = i;
                physical_device_info.queue_count = properties[i].queueCount;
                physical_device_info.queue_count_used = 1;
            }
        }

        delete[] properties;
    }else{
        Core::debug::log(Core::debug::Fatal, "Couldn't get number of queues of the device\n");
    }
    
    return true;
}

bool Graphics::get_device_extensions(void){
    bool found_extension = false;

    uint32_t total_extensions = 0;
    if(vkEnumerateDeviceExtensionProperties(physical_device_info.device, NULL, &total_extensions, NULL) != VK_SUCCESS) return false;
    VkExtensionProperties *properties = new VkExtensionProperties[total_extensions];
    if(vkEnumerateDeviceExtensionProperties(physical_device_info.device, NULL, &total_extensions, properties) == VK_SUCCESS){
        for(uint32_t i = 0; i < total_extensions; i++){
            if(strcmp(properties[i].extensionName, "VK_KHR_swapchain") == 0){
                physical_device_info.extensions_names.push_back("VK_KHR_swapchain");
                physical_device_info.extension_count++;
                found_extension = true;
                break;
            } 
        }
    }

    if(found_extension == false) return false;
    
    delete[] properties;
    return true;
}

bool Graphics::is_device_supported(void){
    VkBool32 is_supported;
    if(vkGetPhysicalDeviceSurfaceSupportKHR(physical_device_info.device, physical_device_info.queue_index, vk_surface, &is_supported) != VK_SUCCESS) return false;
    if(is_supported == VK_FALSE){
        Core::debug::log(Core::debug::Fatal, "Device is not supported for the presentation");
        return false;
    } 

    return true;
}

bool Graphics::add_device_features(uint32_t num_features, ...){
    VkPhysicalDeviceFeatures available_features {};
    vkGetPhysicalDeviceFeatures(physical_device_info.device, &available_features);
    Core::debug::log(Core::debug::Info, "Features: \n");
    enumerate_available_features(available_features);
    
    va_list args;
    va_start(args, num_features);
    for(uint32_t i = 0; i < num_features; i++){
        CoreFeatures retreived_feature = va_arg(args, CoreFeatures);
        if(device_has_feature(available_features, retreived_feature) == true){
            device_enable_feature(&physical_device_info.features, retreived_feature);
        }else{
            va_end(args);
            return false;
        }
    }
    va_end(args);

    return true;
}

bool Graphics::initialize_device_get_queue_handle(void){
    if(!get_best_device()) return false;
    if(!choose_queue_family()) return false;
    if(!get_device_extensions()) return false;
    if(!is_device_supported()) return false;
    if(!add_device_features(2, Geometry_Shader, Tesselation_Shader)) return false;

    float priority = 1.0f;
    VkDeviceQueueCreateInfo queue_create_info {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.pNext = NULL;
    queue_create_info.flags = 0;
    queue_create_info.queueFamilyIndex = physical_device_info.queue_index;
    queue_create_info.queueCount = physical_device_info.queue_count_used;
    queue_create_info.pQueuePriorities = &priority; 

    VkDeviceCreateInfo device_create_info {};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pNext = NULL;
    device_create_info.flags = 0;
    device_create_info.queueCreateInfoCount = 1;
    device_create_info.pQueueCreateInfos = &queue_create_info;
    device_create_info.enabledLayerCount = 0;
    device_create_info.ppEnabledLayerNames = NULL;
    device_create_info.enabledExtensionCount = physical_device_info.extension_count;
    device_create_info.ppEnabledExtensionNames = physical_device_info.extensions_names.vector_data();
    device_create_info.pEnabledFeatures = &physical_device_info.features;

    if(vkCreateDevice(physical_device_info.device, &device_create_info, NULL, &logical_device) != VK_SUCCESS) return false;
    vkGetDeviceQueue(logical_device, physical_device_info.queue_index, 0, &logical_device_queue);

    return true;
}

bool Graphics::get_swapchain_info(void){
    // Device and Surface capabilites (The height and widht of course change depending on the screen)
    VkSurfaceCapabilitiesKHR capabilities;
    if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device_info.device, vk_surface, &capabilities) == VK_SUCCESS){
        enumerate_device_capabilities(capabilities);
        swapchain_info.min_image_count = capabilities.minImageCount;
        swapchain_info.max_image_count = capabilities.maxImageCount;
        swapchain_info.curr_image_extent = capabilities.currentExtent;
        swapchain_info.max_image_array_layers = capabilities.maxImageArrayLayers;
        swapchain_info.defined_image_array_layers = 1;
        swapchain_info.image_usage_flags = get_swapchain_usage_flags(capabilities.supportedUsageFlags, 2, COLOR_ATTACHMENT, TRANSFER_DESTINATION);
        swapchain_info.surface_transform = capabilities.currentTransform;
        swapchain_info.alpha_flags = get_swapchain_alpha_flag(OPAQUE);
    }else{
        Core::debug::log(Core::debug::Error, "Couldn't get physical device surface capabilites\n");
        return false;
    }

    // Device and Surface formats (The number of formats change depending on the screen that is used)
    uint32_t num_formats = 0;
    if(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device_info.device, vk_surface, &num_formats, NULL) != VK_SUCCESS) return false;
    VkSurfaceFormatKHR *surface_formats = new VkSurfaceFormatKHR[num_formats];
    
    if(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device_info.device, vk_surface, &num_formats, surface_formats) == VK_SUCCESS){
        enumerate_device_formats(surface_formats, num_formats);
        get_device_format_color_space(surface_formats, num_formats, VK_FORMAT_R8G8B8A8_UNORM, &swapchain_info);
    }else{
        Core::debug::log(Core::debug::Error, "Couldn't get physical device surface formats\n");
        return false;
    }
    delete[] surface_formats;
    
    // Device and Surface Present modes
    uint32_t num_present_modes = 0;
    if(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device_info.device, vk_surface, &num_present_modes, NULL) != VK_SUCCESS) return false;
    VkPresentModeKHR *present_modes = new VkPresentModeKHR[num_present_modes];
    if(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device_info.device, vk_surface, &num_present_modes, present_modes) == VK_SUCCESS){
        enumerate_device_present_modes(present_modes, num_present_modes);
        swapchain_info.present_mode = get_present_mode(present_modes, num_present_modes, VK_PRESENT_MODE_FIFO_KHR);
    }else{
        Core::debug::log(Core::debug::Error, "Couldn't get physical device surface present modes\n");
        return false;
    }
    delete[] present_modes;

    // Missing information not obtainable through vulkan queries used above.
    swapchain_info.queue_family_index = physical_device_info.queue_index;
    swapchain_info.queue_family_count_used = physical_device_info.queue_count_used;
    swapchain_info.is_clipped = VK_TRUE;

    return true;
}

bool Graphics::create_swapchain(void){
    if(!get_swapchain_info()) return false;

    VkSwapchainCreateInfoKHR swapchain_create_info {};
    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.pNext = NULL;
    swapchain_create_info.flags = 0;
    swapchain_create_info.surface = vk_surface;
    if(swapchain_info.min_image_count < 2) swapchain_create_info.minImageCount = 2;     // Just to ensure i only use double buffering
    else swapchain_create_info.minImageCount = 2;                                       // Just to ensure i only use double buffering
    swapchain_create_info.imageFormat = swapchain_info.format;
    swapchain_create_info.imageColorSpace = swapchain_info.color_space;
    swapchain_create_info.imageExtent = swapchain_info.curr_image_extent;
    swapchain_create_info.imageArrayLayers = swapchain_info.defined_image_array_layers;
    swapchain_create_info.imageUsage = swapchain_info.image_usage_flags;
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_create_info.queueFamilyIndexCount = swapchain_info.queue_family_count_used;
    swapchain_create_info.pQueueFamilyIndices = &swapchain_info.queue_family_index;
    swapchain_create_info.preTransform = swapchain_info.surface_transform;
    swapchain_create_info.compositeAlpha = swapchain_info.alpha_flags;
    swapchain_create_info.presentMode = swapchain_info.present_mode;
    swapchain_create_info.clipped = swapchain_info.is_clipped;
    swapchain_create_info.oldSwapchain = NULL;

    if(vkCreateSwapchainKHR(logical_device, &swapchain_create_info, NULL, &swapchain) != VK_SUCCESS) return false;

    return true;
}

bool Graphics::create_images_and_image_views(void){
    uint32_t num_images = 0;
    if(vkGetSwapchainImagesKHR(logical_device, swapchain, &num_images, NULL) != VK_SUCCESS) return false;
    if(num_images != 2) return false;

    images.vector_resize(num_images);
    image_views.vector_resize(num_images);

    if(vkGetSwapchainImagesKHR(logical_device, swapchain, &num_images, images.vector_data()) == VK_SUCCESS){
        for(uint32_t i = 0; i < num_images; i++){
            VkImageViewCreateInfo image_view_create_info {};
            image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            image_view_create_info.pNext = NULL;
            image_view_create_info.flags = 0;
            image_view_create_info.image = images.vector_data_id(i);
            image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            image_view_create_info.format = swapchain_info.format;
            image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            image_view_create_info.subresourceRange.baseMipLevel = 0;
            image_view_create_info.subresourceRange.levelCount = 1;
            image_view_create_info.subresourceRange.baseArrayLayer = 0;
            image_view_create_info.subresourceRange.layerCount = 1;

            if(vkCreateImageView(logical_device, &image_view_create_info, NULL, image_views.vector_data(i)) != VK_SUCCESS) return false;
        }
    }

    return true;
}

bool Graphics::create_pipeline_layout(void){
    VkPipelineLayoutCreateInfo pipeline_layout_create_info {};
    pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_create_info.pNext = NULL;
    pipeline_layout_create_info.flags = 0;
    pipeline_layout_create_info.setLayoutCount = 0;
    pipeline_layout_create_info.pSetLayouts = nullptr;
    pipeline_layout_create_info.pushConstantRangeCount = 0;
    pipeline_layout_create_info.pPushConstantRanges = nullptr;

    if(vkCreatePipelineLayout(logical_device, &pipeline_layout_create_info, NULL, &pipeline_layout) != VK_SUCCESS) return false;

    return true;
}

bool Graphics::create_render_pass(void){
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

    if(vkCreateRenderPass(logical_device, &render_pass_create_info, NULL, &render_pass) != VK_SUCCESS) return false;

    return true;
}

bool Graphics::create_graphics_pipeline(void){
    if(!create_shader_module(logical_device, &vertex_shader_module, "Shaders/VertexShader.spv")) return false;
    if(!create_shader_module(logical_device, &fragment_shader_module, "Shaders/FragmentShader.spv")) return false;
    Core::n_vector<VkPipelineShaderStageCreateInfo> shader_stage_info;
    shader_stage_info.push_back(create_shader_stage_info(VK_SHADER_STAGE_VERTEX_BIT, vertex_shader_module));
    shader_stage_info.push_back(create_shader_stage_info(VK_SHADER_STAGE_FRAGMENT_BIT, fragment_shader_module));
    VkPipelineDynamicStateCreateInfo dynamic_state_info = create_dynamic_state_info(2);
    VkPipelineVertexInputStateCreateInfo vertex_state_info = create_vertex_state_info();
    VkPipelineInputAssemblyStateCreateInfo assembly_state_info = create_assembly_state_info();
    VkPipelineViewportStateCreateInfo viewport_state_info = create_viewport_state_info(swapchain_info);
    VkPipelineRasterizationStateCreateInfo rasterizer_state_info = create_rasterizer_state_info();
    VkPipelineMultisampleStateCreateInfo multisample_state_info = create_multisample_state_info();
    VkPipelineColorBlendStateCreateInfo color_blend_state_info = create_color_blend_state_info(false);

    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info {};
    graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline_create_info.pNext = NULL;
    graphics_pipeline_create_info.flags = 0;
    graphics_pipeline_create_info.stageCount = static_cast<uint32_t>(shader_stage_info.vector_size());
    graphics_pipeline_create_info.pStages = shader_stage_info.vector_data();
    graphics_pipeline_create_info.pVertexInputState = &vertex_state_info;
    graphics_pipeline_create_info.pInputAssemblyState = &assembly_state_info;
    graphics_pipeline_create_info.pTessellationState = NULL;
    graphics_pipeline_create_info.pViewportState = &viewport_state_info;
    graphics_pipeline_create_info.pRasterizationState = &rasterizer_state_info;
    graphics_pipeline_create_info.pMultisampleState = &multisample_state_info;
    graphics_pipeline_create_info.pDepthStencilState = nullptr;
    graphics_pipeline_create_info.pColorBlendState = &color_blend_state_info;
    graphics_pipeline_create_info.pDynamicState = &dynamic_state_info;
    if(!create_pipeline_layout()) return false;
    graphics_pipeline_create_info.layout = pipeline_layout;
    if(!create_render_pass()) return false;
    graphics_pipeline_create_info.renderPass = render_pass;
    graphics_pipeline_create_info.subpass = 0;
    graphics_pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
    graphics_pipeline_create_info.basePipelineIndex = -1;

    if(vkCreateGraphicsPipelines(logical_device, VK_NULL_HANDLE, 1, &graphics_pipeline_create_info, NULL, &graphics_pipeline) != VK_SUCCESS) return false;

    return true;
}

bool Graphics::create_frame_buffers(void){
    frame_buffers.vector_resize(image_views.vector_size());

    for(size_t i = 0; i < frame_buffers.vector_size(); i++){
        VkFramebufferCreateInfo frame_buffer_create_info {};
        frame_buffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frame_buffer_create_info.pNext = NULL;
        frame_buffer_create_info.flags = 0;
        frame_buffer_create_info.renderPass = render_pass;
        frame_buffer_create_info.attachmentCount = 1;
        frame_buffer_create_info.pAttachments = image_views.vector_data(i);
        frame_buffer_create_info.width = swapchain_info.curr_image_extent.width;
        frame_buffer_create_info.height = swapchain_info.curr_image_extent.height;
        frame_buffer_create_info.layers = swapchain_info.max_image_array_layers;

        if(vkCreateFramebuffer(logical_device, &frame_buffer_create_info, NULL, frame_buffers.vector_data(i)) != VK_SUCCESS) return false;
    }

    return true;
}

bool Graphics::create_command_pool_and_buffer(void){
    // Command Pool
    VkCommandPoolCreateInfo command_pool_create_info {};
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.pNext = NULL;
    command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_create_info.queueFamilyIndex = physical_device_info.queue_index;

    if(vkCreateCommandPool(logical_device, &command_pool_create_info, nullptr, &command_pool) != VK_SUCCESS) return false;

    // Command Buffer
    VkCommandBufferAllocateInfo command_buffer_info {};
    command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_info.pNext = NULL;
    command_buffer_info.commandPool = command_pool;
    command_buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_info.commandBufferCount = 1;

    if(vkAllocateCommandBuffers(logical_device, &command_buffer_info, &command_buffer) != VK_SUCCESS) return false;

    return true;
}

void Graphics::record_command_buffer(VkCommandBuffer command_buffer, uint32_t image_index){
    // Beging phase
    VkCommandBufferBeginInfo command_buffer_begin_info {};
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.pNext = NULL;
    command_buffer_begin_info.flags = 0;
    command_buffer_begin_info.pInheritanceInfo = nullptr;
    if(vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info) != VK_SUCCESS) return;

    // Render pass phase
    VkRenderPassBeginInfo render_pass_begin_info {};
    render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_begin_info.pNext = NULL;
    render_pass_begin_info.renderPass = render_pass;
    render_pass_begin_info.framebuffer = frame_buffers.vector_data_id(image_index);
    render_pass_begin_info.renderArea.offset.x = 0;
    render_pass_begin_info.renderArea.offset.y = 0;
    render_pass_begin_info.renderArea.extent = swapchain_info.curr_image_extent;
    render_pass_begin_info.clearValueCount = 1;
    VkClearValue clear_color;
    clear_color.color.float32[0] = 0.0f;
    clear_color.color.float32[1] = 0.0f;
    clear_color.color.float32[2] = 0.0f;
    clear_color.color.float32[3] = 1.0f;
    render_pass_begin_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    // Bind pipeline phase
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

    // Set the dynamic states again phase(Viewport and Scissors)
    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapchain_info.curr_image_extent.width);
    viewport.height = static_cast<float>(swapchain_info.curr_image_extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);

    VkRect2D scissors;
    scissors.offset = {0, 0};
    scissors.extent = swapchain_info.curr_image_extent;
    vkCmdSetScissor(command_buffer, 0, 1, &scissors);

    // Draw the triangle phase
    vkCmdDraw(command_buffer, 3, 1, 0, 0);

    //End phase
    vkCmdEndRenderPass(command_buffer);

    if(vkEndCommandBuffer(command_buffer) != VK_SUCCESS) return;

    Core::debug::log(Core::debug::Info, "Command recorded\n");
    return;
}

bool Graphics::create_sync_objects(void){
    // Semaphores
    VkSemaphoreCreateInfo semaphore_create_info {};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = NULL;
    semaphore_create_info.flags = 0;

    if(vkCreateSemaphore(logical_device, &semaphore_create_info, nullptr, &image_available_semaphore) != VK_SUCCESS) return false;
    if(vkCreateSemaphore(logical_device, &semaphore_create_info, nullptr, &render_finished_semaphore) != VK_SUCCESS) return false;

    // Fence
    VkFenceCreateInfo fence_create_info {};
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.pNext = NULL;
    fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if(vkCreateFence(logical_device, &fence_create_info, nullptr, &init_flight_fence) != VK_SUCCESS) return false;

    return true;
}

void Graphics::temp_render_func(void){
    // Fence segment
    vkWaitForFences(logical_device, 1, &init_flight_fence, VK_TRUE, UINT64_MAX);
    vkResetFences(logical_device, 1, &init_flight_fence);

    // Image segment
    uint32_t image_index;
    vkAcquireNextImageKHR(logical_device, swapchain, UINT64_MAX, image_available_semaphore, VK_NULL_HANDLE, &image_index);
    
    // Command buffer segment
    vkResetCommandBuffer(command_buffer, 0);
    record_command_buffer(command_buffer, image_index);

    // Queue submission segment
    VkSubmitInfo queue_submit_info {};
    queue_submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    queue_submit_info.pNext = NULL;
    queue_submit_info.waitSemaphoreCount = 1;
    queue_submit_info.pWaitSemaphores = &image_available_semaphore;
    VkPipelineStageFlags stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    queue_submit_info.pWaitDstStageMask = &stage_flags;
    queue_submit_info.commandBufferCount = 1;
    queue_submit_info.pCommandBuffers = &command_buffer;
    queue_submit_info.signalSemaphoreCount = 1;
    queue_submit_info.pSignalSemaphores = &render_finished_semaphore;

    if(vkQueueSubmit(logical_device_queue, 1, &queue_submit_info, init_flight_fence) != VK_SUCCESS){
        printf("Failed to submit queue\n");
        exit(EXIT_FAILURE);
    }

    // Present segment
    VkPresentInfoKHR present_info {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.pNext = NULL;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &render_finished_semaphore;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain;
    present_info.pImageIndices = &image_index;
    present_info.pResults = NULL;

    vkQueuePresentKHR(logical_device_queue, &present_info);

    return;
}

void Graphics::destroy_sync_objects(void){
    vkDestroySemaphore(logical_device, image_available_semaphore, nullptr);
    vkDestroySemaphore(logical_device, render_finished_semaphore, nullptr);
    vkDestroyFence(logical_device, init_flight_fence, nullptr);

    return;
}

void Graphics::destroy_command_pool(void){
    vkDestroyCommandPool(logical_device, command_pool, nullptr);

    return;
}

void Graphics::destroy_frame_buffers(void){
    for(size_t i = 0; i < frame_buffers.vector_size(); i++){
        vkDestroyFramebuffer(logical_device, frame_buffers.vector_data_id(i), nullptr);
    }

    return;
}

void Graphics::destroy_graphics_pipeline_and_layout(void){
    destroy_shader_module(logical_device, vertex_shader_module);
    destroy_shader_module(logical_device, fragment_shader_module);

    vkDestroyPipeline(logical_device, graphics_pipeline, nullptr);
    vkDestroyPipelineLayout(logical_device, pipeline_layout, nullptr);
    vkDestroyRenderPass(logical_device, render_pass, nullptr);

    return;
}

void Graphics::destroy_images_and_image_views(void){
    for(size_t i = 0; i < image_views.vector_size(); i++){
        vkDestroyImageView(logical_device, image_views.vector_data_id(i), nullptr);
    }
    image_views.empty_vector();
    images.empty_vector();

    return;
}

void Graphics::destroy_swapchain(void){
    vkDestroySwapchainKHR(logical_device, swapchain, NULL);

    return;
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
    vkDeviceWaitIdle(logical_device);
    destroy_sync_objects();
    destroy_command_pool();
    destroy_frame_buffers();
    destroy_graphics_pipeline_and_layout();
    destroy_images_and_image_views();
    destroy_swapchain();
    destroy_device();
    destroy_surface();
    close_debug_messenger();
    vkDestroyInstance(vk_instance, nullptr);

    return;
}