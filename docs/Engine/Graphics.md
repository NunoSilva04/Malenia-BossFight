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

#### Function: `bool initialize_graphics(char const * const * array_extensions, uint32_t num_extensions)`  
- **Access:** `Public`
- **Parameters:**
    - `[in]char const * const * array_extensions` - Pointer to an array that holds the names of all of the extensions.
    - `[in]uint32_t num_extensions` - Number of extensions in the array.
- **Return:**  `bool`   
- **Description**: Initializes graphics by [initialize_graphics](Graphics.md#function-bool-initialize_instancechar-const--const--array_extensions-uint32_t-num_extensions).

#### Function: `void close_graphics(void)` 
- **Access:** `Public` 
- **Parameters:**  `void`  
- **Return:**  `void`   
- **Description**: Closes graphics and frees all of the resources used.

#### Function: `bool initialize_instance(char const * const * array_extensions, uint32_t num_extensions)` 
- **Access:** `Private` 
- **Parameters:** 
    - `[in]char const * const * array_extensions` - Pointer to an array that holds the names of all of the extensions.
    - `[in]uint32_t num_extensions` - Number of extensions in the array. 
- **Return:**  `bool`   
- **Description**: Initializes the vulkan instance with the proper extensions.

### Static Functions 
None.

## Dependencies
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
- [SDL3](https://github.com/libsdl-org/SDL)