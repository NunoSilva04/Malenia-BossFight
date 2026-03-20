# Graphics.h

Graphics class responsible for all of the comunication with the GPU.  
Uses the third party library [Vulkan SDK](https://vulkan.lunarg.com/sdk/home).  
Uses the third party library [SDL3](https://github.com/libsdl-org/SDL). 

## Member Types
### `typedef struct Physical_Device_Information_t Physical_Device_Info`
Structure that holds all of the necessary information for the creation of the logical device.

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `device` | `VkPhysicalDevice` | The handle of the chosen physical device | `None` |
|`queue_index` | `uint32_t` | Index of the queue family of the physical device | `None` |
|`queue_count` | `uint32_t` | Maximum number of queues in queue family | `None` |
|`queue_count_used` | `uint32_t` | The number of queues to be used | `None` |
|`extensions_names` | `Core::n_vector<const char *>` | Name of the extensions for the creation of the device| `None` |
|`extension_count` | `uint32_t` | Number of extensions | `None` |
|`features` | `VkPhysicalDeviceFeatures` | Features to be enabled upon the creation of the device | `None` |


### `typedef struct Swapchain_Info_t Swapchain_Info`
Structure that holds all of the necessary information for the creation of the swapchain.

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `min_image_count` | `uint32_t` | Minimum number of images | `None` |
|`max_image_count` | `uint32_t` | Maximum number of images| `None` |
|`curr_image_extent` | `VkExtent2D` | Current screen/image size | `None` |
|`max_image_array_layers` | `uint32_t` | Maximum number of layers | `None` |
|`defined_image_array_layers` | `uint32_t` | Number of layers that will be used | `None` |
|`image_usage_flags` | `VkImageUsageFlags` | Bitmask for the usage flags | `None` |
|`surface_transform` | `VkSurfaceTransformFlagBitsKHR` | Current transformed flag in use | `None` |
|`alpha_flags` | `VkCompositeAlphaFlagBitsKHR` | Current alpha flag in use | `None` |
|`format` | `VkFormat` | The format of the swapchain | `None` |
|`color_space` | `VkColorSpaceKHR` | Color space of the specified `format` | `None` |
|`present_mode` | `VkPresentModeKHR` | Present mode for the swapchain | `None` |
|`queue_family_count_used` | `uint32_t` | Number of queue families that will be used | `None` |
|`queue_family_index` | `uint32_t` | Index of the specified queue family | `None` |
|`is_clipped` | `VkBool32` | Boolean value to determine if the swapchain will be clipped or not | `None` |


## Member Variables
### Instance variables

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `vk_instance` | `VkInstance` |Vulkan instance| `None` |
|`messenger` | `VkDebugUtilsMessengerEXT` |Messenger instance for the debug callback function| `None` |
|`vk_surface` | `VkSurfaceKHR` |Surface instance| `None` |
|`physical_device_info` | `Physical_Device_Info` |Instance of the struct `physical_device_info`| `None` |
|`logical_device` | `VkDevice` |Vulkan  logical device instance| `None` |
|`logical_device_queue` | `VkQueue` | Queue handle retreived upon the creation of the device | `None` |
|`swapchain_info` | `Swapchain_Info` | Instance of th struct `Swapchain_Info` | `None` |
|`swapchain` | `VkSwapchainKHR` | Handle for the swapchain | `None` |
|`images` | `Core::n_vector<VkImage>` | Vector for the handle of the vulkan images | `None` |
|`image_views` | `Core::n_vector<VkImage>` | Vector for the handle of the vulkan image views | `None` |

### Static variables
None

## Member Functions

### Instance Functions

#### Function: `Graphics()` 
- **Access:** `Public` 
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Initializes Graphics instance and initializes all of it's members to 0.

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
- **Description**: Initializes the graphics class by initializing the instance, surface, device, swapchain and command pools/buffers.  
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

#### Function: `bool get_best_device(void)` 
- **Access:** `Private` 
- **Parameters:** `void` 
- **Return:**  `bool`   
- **Description**: Enumerates all available devices and chooses the best physical device. The best device is saved on the `Physical_Device_Info` structure. Preference is given to the discrete GPUs, if there are multiple, it will choose the one with the most amount of VRAM. If no discrete GPU is available then it will select the integrated GPU. Returns true if at least one physical device was found. Returns false otherwise.

#### Function: `bool choose_queue_family(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `bool`   
- **Description**: Enumerates all of families of the chosen best device. Chooses the family that includes `VK_QUEUE_GRAPHICS_BIT` and fills `physical_device_info` with the appropriate queue family index, maximum number of queues, and specifies the number of queues to be used. Returns false if no family includes `VK_QUEUE_GRAPHICS_BIT`. Returns true otherwise

#### Function: `bool get_device_extensions(void))` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `bool`   
- **Description**: Enumerates all of the chosen `device` extensions. If the `VK_KHR_swapchain` is present then it add the extension to the `physical_device_info` vector, increment the number of extensions and returns true. Returns false if this extension is not present. 

#### Function: `bool is_device_supported(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `bool`   
- **Description**: Checks if the chosen physical device is supported on the surface that was created. Returns true if it supports and returns false otherwise.

#### Function: `bool add_device_features(uint32_t num_features, ...)` 
- **Access:** `Private` 
- **Parameters:** 
    - `[in]uint32_t num_features` - The number of featers to add.
    - `[in]...` - Name of the features to be added. The number of features inputed **must** match the number of features declared in `num_features`.
- **Return:**  `bool`   
- **Description:** Queries all of the available features of the chosen physical device and if the features requested are available then it will enable the requested features and return true. Returns false if at least one of the features requested is not available in the chosen physical device.
- **Note:** The name of the features to be added are expressed as a `enum` in the **Graphics.cpp** file.

#### Function: `bool initialize_device_get_queue_handle(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `bool`   
- **Description:** Initializes the logical device and retreives the corresponding queue handle. Returns true if the logical device was successfully created. Returns false otherwise.

#### Function: `friend bool get_device_format_color_space(VkSurfaceFormatKHR *surface_formats, uint32_t num_formats, VkFormat desired_format, Graphics::Swapchain_Info *swapchain_info)` 
- **Access:** `None (Friend function, non-member function with access to private members)`
- **Parameters:** 
    - `[in]VkSurfaceFormatKHR *surface_formats` - Pointer to an array holding all of the available formats of the chosen physical device.
    - `[in]uint32_t num_formats` - The number of formats of the array. 
    - `[in]VkFormat desired_format` - The desired format.
    - `[out]Graphics::Swapchain_Info *swapchain_info` - Pointer to the Graphics class instance `swapchain_info`.
- **Return:**  `bool`   
- **Description:** Queries all of the available formats of the chosen physical device and if one of them matches the desired format, it will fill `swapchain_info` with the format and the corresponding color space and will return true. Returns false if the desired format doesn't match any of the avaible formats.

#### Function: `bool get_swapchain_info(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `bool`   
- **Description:** Retreives the chosen physical device capabilities, formats and present modes and fills the `Swapchain_Info` structure with the information queried. Returns true if it was able to retreive the capabilities, formats and present modes of the chosen physical device. Returns false if it couldn't retreive information from at least one of these. 

#### Function: `bool create_swapchain(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `bool`   
- **Description:** Creates the swapchain with the retreived information from `get_swapchain_info`. Returns false if it couldn't create the swapchain or if `get_swapchain_info` returns false. Returns true otherwise. 
- **Note:** At the moment, the minimum image count being enabled is forced to 2 to ensure double buffering. 

#### Function: `bool create_images_and_image_views(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `bool`   
- **Description:** Creates the appropriate number of image views and returns true if it was successful. The number of image views created must match the number of minimum image count when creating the swapchain otherwise it will return false. Will also return false if at least one of the image views couldn't be created.

#### Function: `bool create_command_pool(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `bool`   
- **Description:** TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO.

#### Function: `void create_command_buffer(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `bool`   
- **Description:** TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO.

#### Function: `void destroy_command_pool(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `void`   
- **Description:** TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO.

#### Function: `void destroy_images_and_image_views(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `void`   
- **Description:** TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO.

#### Function: `void destroy_swapchain(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `void`   
- **Description:** Destroys the number of image views created.

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