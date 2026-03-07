#include "Graphics.h"
#include <cstdio>
#include <iostream>
#include <string>

Graphics::Graphics(){

}

Graphics::~Graphics(){

}

bool Graphics::initialize_graphics(char const * const * array_extensions, uint32_t num_extensions){
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
    create_info.enabledLayerCount = 0;
    create_info.ppEnabledLayerNames = nullptr;
    create_info.enabledExtensionCount = num_extensions;
    create_info.ppEnabledExtensionNames = array_extensions;

    VkResult result =  vkCreateInstance(&create_info, nullptr, &vk_instance);
    if(result != VK_SUCCESS){
        printf("Couldn't create vk_instance\n");
        return false;
    }

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
    vkGetPhysicalDeviceQueueFamilyProperties(chosen_device, &queue_count, family_properties);
    for(unsigned int i = 0; i < queue_count; i++){
        printf("Family %d:\n", i);
        std::cout << "Queue number: "  + std::to_string(family_properties[i].queueCount) << "\n";
        std::cout << "Queue flags: " + std::to_string(family_properties[i].queueFlags) << "\n\n\n";
    }

    delete[] family_properties;

    /*
    * #TODO: finish the create device part. Workflow that i think you are supposed to do:
    *       queue family -> queue create info -> create device
    */

    //VkDeviceQueueCreateInfo queue_info {};
    //queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    //queue_info.pNext = NULL;
    //queue_info.flags = 0;
    //queue_info.queueFamilyIndex = 
    //queue_info.queueCount = 
    //queue_info.pQueuePriorities = 
//
    //VkDeviceCreateInfo device_info;
    //device_info.pNext = &queue_info;
//
    //VkDevice dev;
    //if(vkCreateDevice(chosen_device, &device_info, NULL, &dev) != VK_SUCCESS){
    //    printf("Couldn't create device\n");
    //    return false;
    //}

    return true;
}

void Graphics::close_graphics(void){
    vkDestroyInstance(vk_instance, nullptr);

    return;
}