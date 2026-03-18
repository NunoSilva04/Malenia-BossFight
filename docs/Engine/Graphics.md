# Graphics.h

Graphics class responsible for all of the comunication with the GPU.  
Uses the third party library [Vulkan SDK](https://vulkan.lunarg.com/sdk/home).  
Uses the third party library [SDL3](https://github.com/libsdl-org/SDL). 

## Member Types
None.

## Member Variables
### Instance variables

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `vk_instance` | `VkInstance` |Vulkan instance| `None` |
|`messenger` | `VkDebugUtilsMessengerEXT` |Messenger instance for the debug callback function| `None` |
|`vk_surface` | `VkSurfaceKHR` |Surface instance| `None` |
|`logical_device` | `VkDevice` |Vulkan  logical device instance| `None` |

### Static variables
None

## Member Functions

### Instance Functions

#### Function: `Graphics()` 
- **Access:** `Public` 
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Initializes Graphics instance.

#### Function: `~Graphcis()`
- **Access:** `Public`   
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Destroys the Graphics instance.

#### Function: `bool initialize_graphics(SDL_Window *window, Core::n_vector<const char *> extensions)`  
- **Access:** `Public`
- **Parameters:** 
    - `[in]Core::n_vector<const char *> extensions` - Vector that holds [SDL](https://github.com/libsdl-org/SDL) extensions.
    - `[in]SDL_Window *window` - pointer to the [window](window.md#instance-variables).
- **Return:**  `bool`   
- **Description**: Initializes the graphics class by initializing the instance, surface and device.  
If the project is being build on debug mode, then it will also [get_validation_layers](Graphics.md#function-coren_vectorconst-char--get_validation_layersvoid), [add_debug_extension](Graphics.md#function-void-add_debug_extensioncoren_vectorconst-char--extensions) and [initialize_debug_messenger](Graphics.md#function-bool-initialize_debug_messengervoid).

#### Function: `void close_graphics(void)` 
- **Access:** `Public` 
- **Parameters:**  `void`  
- **Return:**  `void`   
- **Description**: Closes graphics and frees all of the resources used.

#### Function: `Core::n_vector<const char *> get_validation_layers(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `[in]Core::n_vector<const char *>`   
- **Description**: Returns a vector with the following layer names`VK_LAYER_KHRONOS_validation` and `VK_LAYER_LUNARG_crash_diagnostic`. May return a empty vector is none of these layers are available. Used only in **debug** mode.

#### Function: `void add_debug_extension(Core::n_vector<const char *> &extensions)` 
- **Access:** `Private` 
- **Parameters:**
    - `[out]Core::n_vector<const char *> &extensions` - Reference to the vector that holds necessary extensions. 
- **Return:**  `void`   
- **Description**: Adds `VK_EXT_debug_utils` to the vector. Used only in **debug** mode.

#### Function: `bool initialize_instance(Core::n_vector<const char *> extensions, Core::n_vector<const char *> layers)` 
- **Access:** `Private` 
- **Parameters:**
    - `[in]Core::n_vector<const char *> extensions` - Vector that holds necessary extensions.
    - `[in]Core::n_vector<const char *> layers` - Vector that holds necessary layers. 
- **Return:**  `bool`   
- **Description**: Initializes the instance with the extensions and layers provided. Returns `true` in case of success, `false` otherwise.

#### Function: `bool initialize_debug_messenger(void)` 
- **Access:** `Private` 
- **Parameters:** `void` 
- **Return:**  `bool`   
- **Description**: Initializes a debug call back function useful for debugging. Used only **debug** mode.

#### Function: `bool initialize_surface(SDL_Window *window)` 
- **Access:** `Private` 
- **Parameters:** `[in]SDL_Window *window` - Pointer to the [window](window.md#instance-variables). 
- **Return:**  `bool`   
- **Description**: Returns true if it creates the surface to the window. Returns false otherwise.

#### Function: `VkPhysicalDevice get_best_device(void)` 
- **Access:** `Private` 
- **Parameters:** `void` 
- **Return:**  `VkPhysicalDevice`   
- **Description**: Enumerates all available devices and returns the value of the best physical device. Preference is given to the discrete GPUs, if there are multiple, it will choose the one with the most amount of VRAM. If no discrete GPU is available then it will select the integrated GPU.

#### Function: `void choose_queue_family(const VkPhysicalDevice device, uint32_t *chosen_index, uint32_t *chosen_count)` 
- **Access:** `Private` 
- **Parameters:** 
    - `[in]const VkPhysicalDevice device` - the chosen device from [get_best_device](Graphics.md#function-vkphysicaldevice-get_best_devicevoid).
    - `[out]uint32_t *chosen_index` - pointer to the index of the queue family.
    - `[out]uint32_t *chosen_count` - pointer to the number of queues in the queue family.
- **Return:**  `void`   
- **Description**: Enumerates all of families of the chosen `device`. Chooses the family that includes `VK_QUEUE_GRAPHICS_BIT` and fills `chosen_index` and `chosen_count` with the appropriate values.

#### Function: `bool get_device_extensions(const VkPhysicalDevice device, Core::n_vector<const char *> *extensions, uint32_t *num_extensions)` 
- **Access:** `Private` 
- **Parameters:** 
    - `[in]const VkPhysicalDevice device` - the chosen device from [get_best_device](Graphics.md#function-vkphysicaldevice-get_best_devicevoid).
    - `[out]Core::n_vector<const char *> *extensions` - pointer to the vector of extensions.
    - `[out]uint32_t *num_extensions` - pointer to the number of extensions in the device.
- **Return:**  `bool`   
- **Description**: Enumerates all of the chosen `device` extensions. If the `VK_KHR_swapchain` is present then it add the extension to the vector, increment the number of extensions and returns true. Returns false if this extension is not present. 

#### Function: `bool validate_device_surface(const VkPhysicalDevice device, const uint32_t family_index)` 
- **Access:** `Private` 
- **Parameters:** 
    - `[in]const VkPhysicalDevice device` - The chosen device from [get_best_device](Graphics.md#function-vkphysicaldevice-get_best_devicevoid).
    - `[in]const uint32_t family_index` - Chosen queue family index retreived from [choose_queue_family](Graphics.md#function-void-choose_queue_familyconst-vkphysicaldevice-device-uint32_t-chosen_index-uint32_t-chosen_count).
- **Return:**  `bool`   
- **Description**: Validates the chosen `device` by checking the support, capabilities, format and present modes of the surface created. Returns true if all of the validations are true. Returns false otherwise.

#### Function: `bool add_device_features(VkPhysicalDeviceFeatures *features, const VkPhysicalDevice device, uint32_t num_features, ...)` 
- **Access:** `Private` 
- **Parameters:** 
    - `[out]VkPhysicalDeviceFeatures *features` - Pointer to the device features. 
    - `[in]const VkPhysicalDevice device` - The chosen device retreived from [get_best_device](Graphics.md#function-vkphysicaldevice-get_best_devicevoid).
    - `[in]uint32_t num_features` - The number of featers to add.
    - `[in]...` - Name of the features to be added. The number of features inputed **must** match the number of features declared in `num_features`.
- **Return:**  `bool`   
- **Description**: Queries all of the available features of the `chosen_device` and if the features requested are available then it will enable the requested features and return true. Returns false if at least one of the features requested is not available in the `chosen_device`. 

#### Function: `bool initialize_device(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `bool`   
- **Description**: Initializes the device by choosing the best device, choosing the appropriate queue family, validating the surface and adding the necessary features. Returns true if the initialization was successful. Returns false otherwise.

#### Function: `void destroy_device(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `void`   
- **Description**: Destroys the vulkan logical device.

#### Function: `void destroy_surface(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `void`   
- **Description**: Destroys the vulkan surface.

#### Function: `void close_debug_messenger(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `void`   
- **Description**: Closes the debug messenger. Used only in **debug** mode.

### Static Functions 
None.

## Dependencies
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
- [SDL3](https://github.com/libsdl-org/SDL)
- [n_vector](../core/n_vector.md)