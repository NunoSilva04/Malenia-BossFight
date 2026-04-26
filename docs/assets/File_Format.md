# File_Format.h
Responsible for defining, creating and handling `.assets` files.  
For a detailed description of the format of the `.assets` files click [here](assets_file_format.md)

## `glTF_Helper` namespace
Namespace with helper functions for an easier debbuging of the [glTF](https://github.com/jkuhlmann/cgltf/tree/master) library.

### Instance Functions
#### Function: `inline const char *gltf_result_to_str(cgltf_result result)` 
- **Access:** `Public` 
- **Parameters:** `[in]cgltf_result result` - The cgltf_result type.
- **Return:**  `const char *`   
- **Description:** Returns a string corresponding to the appropriate result type.

#### Function: `inline const char *gltf_options_file_type_to_str(cgltf_file_type type)` 
- **Access:** `Public` 
- **Parameters:** `[in]cgltf_file_type type` - The cgltf_file_type type.
- **Return:**  `const char *`   
- **Description:** Returns a string corresponding to the appropriate file type.

#### Function: `inline const char *gltf_primitive_type_to_str(cgltf_primitive_type type)` 
- **Access:** `Public` 
- **Parameters:** `[in]cgltf_primitive_type type` - The cgltf_primitive_type type.
- **Return:**  `const char *`   
- **Description:** Returns a string corresponding to the appropriate primitive type.

#### Function: `inline const char *gltf_component_type_to_str(cgltf_component_type type)` 
- **Access:** `Public` 
- **Parameters:** `[in]cgltf_component_type type` - The cgltf_component_type type.
- **Return:**  `const char *`   
- **Description:** Returns a string corresponding to the appropriate component type.

#### Function: `inline const char *gltf_bool_to_str(cgltf_bool value)` 
- **Access:** `Public` 
- **Parameters:** `[in]cgltf_bool value` - The cgltf_bool type.
- **Return:**  `const char *`   
- **Description:** Returns a string corresponding to the appropriate boolean type.

#### Function: `inline const char *gltf_accessor_type_to_str(cgltf_type type)` 
- **Access:** `Public` 
- **Parameters:** `[in]cgltf_type type` - The cgltf_type type.
- **Return:**  `const char *`   
- **Description:** Returns a string corresponding to the appropriate accessor type.

#### Function: `inline const char *gltf_buffer_view_type_to_str(cgltf_buffer_view_type type)` 
- **Access:** `Public` 
- **Parameters:** `[in]cgltf_buffer_view_type type` - The cgltf_buffer_view_type type.
- **Return:**  `const char *`   
- **Description:** Returns a string corresponding to the appropriate buffer view type.

#### Function: `inline const char *gltf_attribute_type_to_str(cgltf_attribute_type type)` 
- **Access:** `Public` 
- **Parameters:** `[in]cgltf_attribute_type type` - The cgltf_attribute_type type.
- **Return:**  `const char *`   
- **Description:** Returns a string corresponding to the appropriate attribute type.


## Member Types
None. 

## Member Variables
None.

## Member functions
### Instance Functions
#### Function: `bool create_assets_file(const char *gltf_file_path, const char *gltf_file_name, cgltf_data *const data, const cgltf_options *options)` 
- **Access:** `Public` 
- **Parameters:**
    - `[in]const char *gltf_file_path` - The gltf file path.
    - `[in]cgltf_data *const data` - The glTF file data.
    - `[in]const cgltf_options *options` - The user options.
- **Return:**  `bool`   
- **Description:** Creates a new `.assets` file with the name of the provided file path and and returns true. Returns false otherwise.

## Dependencies
- [glTF](https://github.com/jkuhlmann/cgltf/tree/master)