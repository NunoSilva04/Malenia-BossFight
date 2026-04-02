#include "Graphics.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>
#include "core.h"
#include "Graphics_Helper.h"

Graphics::Graphics(){
    vk_instance = nullptr;
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
    if(vk_debug::get_validation_layers(layers)) vk_debug::add_debug_extension(extensions);
    else{
        Core::debug::log(Core::debug::Fatal, "Couldn't get validation layers\n");
        return false;
    }
    if(!initialize_instance(extensions, layers)){
        Core::debug::log(Core::debug::Error, "Couldn't initialize instance\n");
        return false;
    } 
    if(!vk_debug::initialize_debug_messenger(vk_instance)){
        Core::debug::log(Core::debug::Error, "Couldn't initialize debug messenger\n");
        return false;
    }
    
    if(!initialize_surface(window)){
        Core::debug::log(Core::debug::Fatal, "Couldn't initialize surface\n");
        return false;
    }
    if(!initialize_device_get_queue_handle()){
        Core::debug::log(Core::debug::Fatal, "Couldn't initialize device\n");
        return false;
    }
    vk_debug::create_object_name_fn(logical_device);

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
            vk_helper::version_to_str(properties.apiVersion, version_string, 30 * sizeof(char));
            Core::debug::log(Core::debug::Info, "API Version: %s\n", version_string);
            Core::debug::log(Core::debug::Info, "Driver Version: %d\n", properties.driverVersion);
            Core::debug::log(Core::debug::Info, "Vendor ID: %d\n", properties.vendorID);
            Core::debug::log(Core::debug::Info, "Device ID: %d\n", properties.deviceID);
            Core::debug::log(Core::debug::Info, "Device type: %s\n", vk_helper::device_type_to_str(properties.deviceType));
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
            vk_helper::queue_flags_to_str(properties[i].queueFlags, flag_string, 64 * sizeof(char), &num_flags);
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
    vk_helper::enumerate_available_features(available_features);
    
    va_list args;
    va_start(args, num_features);
    for(uint32_t i = 0; i < num_features; i++){
        vk_helper::CoreFeatures retreived_feature = va_arg(args, vk_helper::CoreFeatures);
        if(vk_helper::device_has_feature(available_features, retreived_feature) == true){
            vk_helper::device_enable_feature(&physical_device_info.features, retreived_feature);
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
    if(!add_device_features(2, vk_helper::Geometry_Shader, vk_helper::Tesselation_Shader)) return false;

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
        vk_helper::enumerate_device_capabilities(capabilities);
        swapchain_info.min_image_count = capabilities.minImageCount;
        swapchain_info.max_image_count = capabilities.maxImageCount;
        swapchain_info.curr_image_extent = capabilities.currentExtent;
        swapchain_info.max_image_array_layers = capabilities.maxImageArrayLayers;
        swapchain_info.defined_image_array_layers = 1;
        swapchain_info.image_usage_flags = vk_helper::get_swapchain_usage_flags(capabilities.supportedUsageFlags, 2, vk_helper::COLOR_ATTACHMENT, vk_helper::TRANSFER_DESTINATION);
        swapchain_info.surface_transform = capabilities.currentTransform;
        swapchain_info.alpha_flags = vk_helper::get_swapchain_alpha_flag(vk_helper::OPAQUE);
    }else{
        Core::debug::log(Core::debug::Error, "Couldn't get physical device surface capabilites\n");
        return false;
    }

    // Device and Surface formats (The number of formats change depending on the screen that is used)
    uint32_t num_formats = 0;
    if(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device_info.device, vk_surface, &num_formats, NULL) != VK_SUCCESS) return false;
    VkSurfaceFormatKHR *surface_formats = new VkSurfaceFormatKHR[num_formats];
    
    if(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device_info.device, vk_surface, &num_formats, surface_formats) == VK_SUCCESS){
        vk_helper::enumerate_device_formats(surface_formats, num_formats);
        vk_helper::get_device_format_color_space(surface_formats, num_formats, VK_FORMAT_R8G8B8A8_UNORM, swapchain_info.format, swapchain_info.color_space);
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
        vk_helper::enumerate_device_present_modes(present_modes, num_present_modes);
        swapchain_info.present_mode = vk_helper::get_present_mode(present_modes, num_present_modes, VK_PRESENT_MODE_FIFO_KHR);
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
    if(!vk_helper::create_shader_module(logical_device, &vertex_shader_module, "Shaders/VertexShader.spv")) return false;
    if(!vk_helper::create_shader_module(logical_device, &fragment_shader_module, "Shaders/FragmentShader.spv")) return false;
    Core::n_vector<VkPipelineShaderStageCreateInfo> shader_stage_info;
    shader_stage_info.push_back(vk_helper::create_shader_stage_info(VK_SHADER_STAGE_VERTEX_BIT, vertex_shader_module));
    shader_stage_info.push_back(vk_helper::create_shader_stage_info(VK_SHADER_STAGE_FRAGMENT_BIT, fragment_shader_module));
    VkPipelineDynamicStateCreateInfo dynamic_state_info = vk_helper::create_dynamic_state_info(2);
    VkPipelineVertexInputStateCreateInfo vertex_state_info = vk_helper::create_vertex_state_info();
    VkPipelineInputAssemblyStateCreateInfo assembly_state_info = vk_helper::create_assembly_state_info();
    VkPipelineViewportStateCreateInfo viewport_state_info = vk_helper::create_viewport_state_info(swapchain_info.curr_image_extent);
    VkPipelineRasterizationStateCreateInfo rasterizer_state_info = vk_helper::create_rasterizer_state_info();
    VkPipelineMultisampleStateCreateInfo multisample_state_info = vk_helper::create_multisample_state_info();
    VkPipelineColorBlendStateCreateInfo color_blend_state_info = vk_helper::create_color_blend_state_info(false);

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
    vk_helper::destroy_shader_module(logical_device, vertex_shader_module);
    vk_helper::destroy_shader_module(logical_device, fragment_shader_module);

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
    vk_debug::destroy_debug_messenger(vk_instance);
    vkDestroyInstance(vk_instance, nullptr);

    return;
}