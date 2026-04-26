# glTF_helper.h
Helper API for handling [glTF](https://github.com/jkuhlmann/cgltf/tree/master).

## Member Types
None. 

## Member Variables
None.

## Member functions
### Instance Functions
#### Function: `bool read_glTF_file(const char *file_path, const char *file_name)` 
- **Access:** `Public` 
- **Parameters:**
    - `[in]const char *file_path` - The file path.
    - `[in]const char *file_name` - The name of the file.
- **Return:**  `bool`   
- **Description:** Parses the data from the provided glTF file and creates a `.assets` file with the file name given by calling [create_assets_file](TODOOOOOO). Returns true in case of success, returns false otherwise.

## Dependencies
None.