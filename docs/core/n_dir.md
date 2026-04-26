# n_dir.h
Namespace of cross platform functionalities that allow the user to handle directories. Available for Windows, MacOS and Linux.  

## Member Types
### `typedef struct Directory_Handle_t Dir_Handle`
Opaque type of the structure `Directory_Handle_t`. 

## Member Variables
None.

## Member functions
### Instance Functions
#### Function: `Dir_Handle *extract_dir_contents(const char *directory_name)` 
- **Access:** `Public` 
- **Parameters:** `[in]const char *directory_name` - The directory name.
- **Return:**  `Dir_Handle *`   
- **Description:** Extracts all of the directory contents such as file names, file paths and more, populates `Dir_Handle` and returns a valid pointer to the structure. Will return `NULL` if it fails.

#### Function: `void get_dir_file_paths(Dir_Handle *handle, size_t *num_files, char ***file_paths)` 
- **Access:** `Public` 
- **Parameters:** 
    - `[in]Dir_Handle *handle` - A valid pointer to `Dir_Handle`. 
    - `[in, out]size_t *num_files` - The number of files in that directory.
    - `[in, out]char ***file_paths` - The path of files.
- **Return:**  `void`   
- **Description:** Fills `num_files` and `file_paths` with the correct information.  
*Note:* [extract_dir_contents](n_dir.md#function-dir_handle-extract_dir_contentsconst-char-directory_name) must have been called previously.

#### Function: `void get_dir_file_names(Dir_Handle *handle, size_t *num_files, char ***file_names)` 
- **Access:** `Public` 
- **Parameters:** 
    - `[in]Dir_Handle *handle` - A valid pointer to `Dir_Handle`. 
    - `[in, out]size_t *num_files` - The number of files in that directory.
    - `[in, out]char ***file_names` - The name of files.
- **Return:**  `void`   
- **Description:** Fills `num_files` and `file_names` with the correct information.
*Note:* [extract_dir_contents](n_dir.md#function-dir_handle-extract_dir_contentsconst-char-directory_name) must have been called previously. 

#### Function: `bool create_dir(const char *dir_path)` 
- **Access:** `Public` 
- **Parameters:** `const char *dir_path` - The path of the Directory.
- **Return:**  `bool`   
- **Description:** Returns true if it creates the directory successfully, returns false otherwise.  

## Dependencies
None.