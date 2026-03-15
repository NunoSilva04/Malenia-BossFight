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

#### Function: `bool initialize_graphics(Core::n_vector<const char *> extensions)`  
- **Access:** `Public`
- **Parameters:**
    - `[in]Core::n_vector<const char *> extensions` - Vector that holds [SDL](https://github.com/libsdl-org/SDL) extensions.
- **Return:**  `bool`   
- **Description**: Initializes graphics by [initialize_graphics](Graphics.md#). If the project is being build on debug mode, then it will also call 

#### Function: `void close_graphics(void)` 
- **Access:** `Public` 
- **Parameters:**  `void`  
- **Return:**  `void`   
- **Description**: Closes graphics and frees all of the resources used.

#### Function: `Core::n_vector<const char *> get_validation_layers(void)` 
- **Access:** `Private` 
- **Parameters:** `void`
- **Return:**  `Core::n_vector<const char *>`   
- **Description**: Returns a vector with the following layer names`VK_LAYER_KHRONOS_validation` and `VK_LAYER_LUNARG_crash_diagnostic`. May return a empty vector is none of these layers are available. Used only in **debug** mode.

#### Function: `void add_debug_extension(Core::n_vector<const char *> extensions)` 
- **Access:** `Private` 
- **Parameters:**
    - `Core::n_vector<const char *> extensions` - Vector that holds necessary extensions. 
- **Return:**  `void`   
- **Description**: Adds `VK_EXT_debug_utils` to the vector. Used only in **debug** mode.

#### Function: `bool initialize_instance(Core::n_vector<const char *> extensions, Core::n_vector<const char *> layers)` 
- **Access:** `Private` 
- **Parameters:**
    - `Core::n_vector<const char *> extensions` - Vector that holds necessary extensions.
    - `Core::n_vector<const char *> layers` - Vector that holds necessary layers. 
- **Return:**  `bool`   
- **Description**: Initializes the instance with the extensions and layers provided. Returns `true` in case of success, `false` otherwise.

#### Function: `bool initialize_debug_messenger(void)` 
- **Access:** `Private` 
- **Parameters:** `void` 
- **Return:**  `bool`   
- **Description**: Initializes a debug call back function useful for debugging.

### Static Functions 
None.

## Dependencies
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
- [SDL3](https://github.com/libsdl-org/SDL)
- [n_vector](../core/n_vector.md)