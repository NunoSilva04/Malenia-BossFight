#include "Graphics_Helper.h"
#include <cstring>

/**********************************************************************************
*
*
*               vk_debug
*
*
*************************************************************************************/
// Layers i am interested in:
static const char *layer_names[] = {
    "VK_LAYER_KHRONOS_validation",
    "VK_LAYER_LUNARG_crash_diagnostic"
};

static VkDebugUtilsMessengerEXT messenger = nullptr;
static PFN_vkSetDebugUtilsObjectNameEXT set_name = nullptr;

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

bool vk_debug::get_validation_layers(Core::n_vector<const char *> &layers){
    #ifdef DEBUG
        uint32_t all_layers = 0;
        if(vkEnumerateInstanceLayerProperties(&all_layers, NULL) != VK_SUCCESS) return false;
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
        if(layers.vector_size() != 2) return false;
    #endif

    return true;
}

void vk_debug::add_debug_extension(Core::n_vector<const char *> &extensions){
    #ifdef DEBUG
        extensions.push_back("VK_EXT_debug_utils");
    #endif

    return;
}

bool vk_debug::initialize_debug_messenger(const VkInstance vk_instance){
    #ifdef DEBUG
        VkDebugUtilsMessengerCreateInfoEXT messenger_info {};
        messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        messenger_info.pNext = NULL;
        messenger_info.flags = 0;
        messenger_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
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
    #endif

    return true;
}

void vk_debug::create_object_name_fn(const VkDevice logical_device){
    #ifdef DEBUG
        set_name = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetDeviceProcAddr(logical_device, "vkSetDebugUtilsObjectNameEXT"));
        if(set_name == nullptr) return;
    #endif

    return;
}

void vk_debug::set_object_name(const VkDevice logical_device, VkObjectType type, uint64_t handle, const char *name){
    #ifdef DEBUG
        if(set_name == nullptr) return;

        VkDebugUtilsObjectNameInfoEXT object_name_info {};
        object_name_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        object_name_info.pNext = nullptr;
        object_name_info.objectType = type;
        object_name_info.objectHandle = handle;
        object_name_info.pObjectName = name;
        set_name(logical_device, &object_name_info);
    #endif

    return;
}

void vk_debug::destroy_debug_messenger(const VkInstance vk_instance){
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

/**********************************************************************************
*
*
*               vk_helper
*
*
*************************************************************************************/

static VkDynamicState dynamic_states[] = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR,
};

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

void vk_helper::enumerate_available_features(const VkPhysicalDeviceFeatures features){
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

bool vk_helper::device_has_feature(const VkPhysicalDeviceFeatures available_features, const CoreFeatures current_feature){
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

void vk_helper::device_enable_feature(VkPhysicalDeviceFeatures *features, const CoreFeatures feature_to_enable){
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

void vk_helper::enumerate_device_capabilities(const VkSurfaceCapabilitiesKHR capabilities){
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

VkImageUsageFlags vk_helper::get_swapchain_usage_flags(const VkImageUsageFlags flags, const uint32_t num_flags, ...){
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

VkCompositeAlphaFlagBitsKHR vk_helper::get_swapchain_alpha_flag(Alpha_Flags desired_flag){
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

void vk_helper::enumerate_device_formats(VkSurfaceFormatKHR *surface_formats, uint32_t num_formats){
    for(uint32_t i = 0; i < num_formats; i++){
        Core::debug::log(Core::debug::Info, "Surface %d\n", i);
        Core::debug::log(Core::debug::Info, "Format = %d\n", surface_formats[i].format);
        Core::debug::log(Core::debug::Info, "Color = %d\n\n\n", surface_formats[i].colorSpace);
    }

    return;
}

bool vk_helper::get_device_format_color_space(VkSurfaceFormatKHR *surface_formats, uint32_t num_formats, VkFormat desired_format, VkFormat &format, VkColorSpaceKHR &color_space){
    bool found_format = false;

    for(uint32_t i = 0; i < num_formats; i++){
        if(desired_format == surface_formats[i].format){
            found_format = true;
            format = surface_formats[i].format;
            color_space = surface_formats[i].colorSpace;
            break;
        }
    }

    return found_format;
}

void vk_helper::enumerate_device_present_modes(const VkPresentModeKHR *present_modes, const uint32_t num_present_modes){
    for(uint32_t i = 0; i < num_present_modes; i++){
        Core::debug::log(Core::debug::Info, "Preset %d\n", i);
        Core::debug::log(Core::debug::Info, "Mode: %s\n\n\n", vk_helper::present_mode_to_str(present_modes[i]));
    }

    return;
}

VkPresentModeKHR vk_helper::get_present_mode(const VkPresentModeKHR *present_modes, const uint32_t num_present_modes, VkPresentModeKHR desired_preset){
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

bool vk_helper::create_shader_module(const VkDevice logical_device, VkShaderModule *shader_module, const char *file_name){
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

void vk_helper::destroy_shader_module(const VkDevice logical_device, VkShaderModule shader_module){
    vkDestroyShaderModule(logical_device, shader_module, NULL);

    return;
}

VkPipelineShaderStageCreateInfo vk_helper::create_shader_stage_info(VkShaderStageFlagBits stage_flag, VkShaderModule module){
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

VkPipelineDynamicStateCreateInfo vk_helper::create_dynamic_state_info(uint32_t num_states){    
    VkPipelineDynamicStateCreateInfo dynamic_state_info {};
    dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_info.pNext = NULL;
    dynamic_state_info.flags = 0;
    dynamic_state_info.dynamicStateCount = num_states;
    dynamic_state_info.pDynamicStates = dynamic_states;

    return dynamic_state_info;
}

VkPipelineVertexInputStateCreateInfo vk_helper::create_vertex_state_info(void){
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

VkPipelineInputAssemblyStateCreateInfo vk_helper::create_assembly_state_info(void){
    VkPipelineInputAssemblyStateCreateInfo assembly_state_info {};
    assembly_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    assembly_state_info.pNext = NULL;
    assembly_state_info.flags = 0;
    assembly_state_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    assembly_state_info.primitiveRestartEnable = VK_FALSE;

    return assembly_state_info; 
}

VkPipelineViewportStateCreateInfo vk_helper::create_viewport_state_info(const VkExtent2D extent){
    // Viewport
    static VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Scissors
    static VkRect2D scissors;
    scissors.offset = {0, 0};
    scissors.extent = extent;

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

VkPipelineRasterizationStateCreateInfo vk_helper::create_rasterizer_state_info(void){
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

VkPipelineColorBlendStateCreateInfo vk_helper::create_color_blend_state_info(bool enabled){
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

VkPipelineMultisampleStateCreateInfo vk_helper::create_multisample_state_info(void){
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