# file_system.h
File namespace that handles how to read, write and create different types of files.

## Member functions
### Instance Functions

#### Function: `uint32_t *read_spv_file(const char *file_path, uint32_t *file_size)`  
- **Access:** `Public`
- **Parameters:**  
    - `[in]const char *file_path` - Path to the file.
    - `[in, out]uint32_t *file_size` - The size of the file.  
- **Return:**  `uint32_t *` - Pointer to an array with the file data.   
- **Description**: Reads a SPIR-V file and returns a vector which holds the binary data of the file. The file data is allocated inside the function, meaning that it's the users responsability to free the memory afterwards. Returns `NULL` in case of failure.

## Dependencies
None.