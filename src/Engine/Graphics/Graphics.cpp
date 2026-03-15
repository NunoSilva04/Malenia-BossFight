#include "Graphics.h"
#include <cstdio>
#include <iostream>
#include <string>
#include "core.h"

//Layers i am interested in:
static const char *layer_names[] = {
    "VK_LAYER_KHRONOS_validation",
    "VK_LAYER_LUNARG_crash_diagnostic"
};

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

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void*                                       pUserData)
{
    printf("Severity: %s\n", severity_to_str(messageSeverity));
    printf("Message: %s\n", pCallbackData->pMessage);
    return VK_FALSE;
}

Graphics::Graphics(){

}

Graphics::~Graphics(){

}

bool Graphics::initialize_graphics(Core::n_vector<const char *> extensions){
    Core::n_vector<const char *> layers;
    #ifdef DEBUG
        layers = get_validation_layers();
        add_debug_extension(extensions);
    #endif

    if(!initialize_instance(extensions, layers)){
        Core::debug::log(Core::debug::Error, "Couldn't initialize instance\n");
        return false;
    } 
    //#ifdef DEBUG
    //    if(!initialize_debug_messenger()){
    //        Core::debug::log(Core::debug::Error, "Couldn't initialize debug messenger\n");
    //        return false;
    //    }
    //#endif

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

void Graphics::add_debug_extension(Core::n_vector<const char *> extensions){
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
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; 
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
        VkDebugUtilsMessengerCallbackDataEXT data;
        data.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT;
        data.pNext = NULL;
        data.flags = 0;
        data.pMessageIdName = NULL;
        data.pMessage = "Hello from debug messenger";

        submit_debug_message(vk_instance, VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT, VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT, &data);
    }

    return true;
}

void Graphics::close_graphics(void){
    //#ifdef DEBUG
    //    PFN_vkDestroyDebugUtilsMessengerEXT destroy_debug_messenger = nullptr;
    //    destroy_debug_messenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vk_instance, "vkDestroyDebugUtilsMessengerEXT");
    //    if(!destroy_debug_messenger){
    //        Core::debug::log(Core::debug::Fatal, "Couldn't create destroy debug messenger\n");
    //    }
    //    destroy_debug_messenger(vk_instance, messenger, nullptr);
    //#endif

    vkDestroyInstance(vk_instance, nullptr);

    return;
}

/*

uint32_t num_devices = 0;
    if(vkEnumeratePhysicalDevices(vk_instance, &num_devices, NULL) != VK_SUCCESS){
        printf("Couldn't enum physical devices\n");
    }
    printf("Num of physical devices: %d\n", num_devices);
    VkPhysicalDevice *physical_devices = new VkPhysicalDevice[num_devices]; 
    vkEnumeratePhysicalDevices(vk_instance, &num_devices, physical_devices);


    VkPhysicalDevice chosen_device;
    uint64_t max_vram = 0;
    bool has_dpgu = false;
    for(unsigned int i = 0; i < num_devices; i++){
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_devices[i], &properties);
        printf("API Version: %d\n", properties.apiVersion);
        printf("Driver Version: %d\n", properties.driverVersion);
        printf("Vendor ID: %d\n", properties.vendorID);
        printf("Device ID: %d\n", properties.deviceID);
        printf("Device type: %d\n", properties.deviceType);
        printf("Device Name: %s\n", properties.deviceName);

        VkPhysicalDeviceMemoryProperties mem_properties;
        vkGetPhysicalDeviceMemoryProperties(physical_devices[i], &mem_properties);
        printf("Memory type count: %d\n", mem_properties.memoryTypeCount);
        printf("Memory heap count: %d\n", mem_properties.memoryHeapCount);
        printf("Memory heap size: %llu\n", mem_properties.memoryHeaps->size / (1024 * 1024));

        printf("\n\n\n");

        if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
            has_dpgu = true;
            if(mem_properties.memoryHeaps->size > max_vram){
                chosen_device = physical_devices[i];
                max_vram = mem_properties.memoryHeaps->size;
            }
        }else{
            if(has_dpgu) continue;
            else{
                chosen_device = physical_devices[i];
            }
        }
    }

    delete[] physical_devices;

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(chosen_device, &properties);
    printf("Device chosen: %s\n", properties.deviceName);

    
    uint32_t queue_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(chosen_device, &queue_count, NULL);
    printf("Number of queue family properties: %d\n", queue_count);

    VkQueueFamilyProperties *family_properties = new VkQueueFamilyProperties[queue_count];
    uint32_t chosen_family_index = 0, chosen_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(chosen_device, &queue_count, family_properties);
    for(unsigned int i = 0; i < queue_count; i++){
        printf("Family %d:\n", i);
        std::cout << "Queue number: "  + std::to_string(family_properties[i].queueCount) << "\n";
        std::cout << "Queue flags: " + std::to_string(family_properties[i].queueFlags) << "\n\n\n";
        if(family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            chosen_family_index = i;
            chosen_family_count = family_properties[i].queueCount;
            printf("Chosen family for now is %d\n", i);
        }
    }

    delete[] family_properties;

    const float priority = 1.0f;
    VkDeviceQueueCreateInfo queue_info {};
    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info.pNext = NULL;
    queue_info.flags = 0;
    queue_info.queueFamilyIndex = chosen_family_index;
    queue_info.queueCount = chosen_family_count;
    queue_info.pQueuePriorities = &priority; 

    VkDeviceCreateInfo device_info {};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.pNext = NULL;
    device_info.queueCreateInfoCount = 1;
    device_info.pQueueCreateInfos = &queue_info;
    device_info.enabledExtensionCount = num_extensions;
    device_info.ppEnabledExtensionNames = array_extensions;

    VkDevice dev;
    if(vkCreateDevice(chosen_device, &device_info, NULL, &dev) != VK_SUCCESS){
        printf("Couldn't create device\n");
        return false;
    }

    printf("Created device\n");
    vkDestroyDevice(dev, NULL);
*/