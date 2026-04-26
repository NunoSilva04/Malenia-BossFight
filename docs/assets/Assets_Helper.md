# Assets_Helper.h
Responsible for reading and interpreting all of the information of the `.assets` file.  
For a detailed description of the format of the `.assets` files click [here](assets_file_format.md)

## Global Variables
- `#define MAX_MESH_NAME 256 ` - Maximum number of character in the mesh name. 
- `static constexpr uint32_t magic = 0x0C1B5D81` - Magic number to validate the integrity of the `.assets` file.
- `static constexpr uint32_t version = 0x01000000` - Version of the current `.assets` file.

## Types
### `typedef enum File_Error File_Error`
Enumerates all of the type of File errors when reading a `.assets` file.

| Type | Description |
| ---- | ----------- |
| NOT_FOUND | Couldn't find `.assets` file|
|CORRUPTED_FILE | When the magic number in the file is not the same as the one defined in the code |
| WRONG_VERSION | Version mistmatch |
| NO_MESH_DATA | The asset has no mesh |

### `typedef struct File_Format File_Format`
Structure that holds the initial file format data.

| Members | Type | Access | Description |
|---------| -----| -------|------------|
| magic | uint32_t | public | Magic number for the `.assets` file|
| magic | version | public | Magic number for the `.assets` file | 

#### `File_Format` Functions
##### Instance Functions
###### Function: `File_Format() noexcept` 
- **Access:** `Public` 
- **Parameters:** `void`
- **Return:**  `Nothing`   
- **Description:** Initializes the `File_Format` structure.

##### Static Functions
None.

### Functions
#### Function: `bool open_assets_file(const char *assets_file, File_Error *error)` 
- **Access:** `Public` 
- **Parameters:**
    - `[in]const char *assets_file` - The `.assets` file path.
    - `[in, out]File_Error *error` - Pointer to the type of `File_Error`. Can be `NULL`.
- **Return:**  `bool`   
- **Description:** Opens the `.assets` file and checks it's integrity by checking the version and magic number of the file. Returns true if no errors and returns false otherwise. **IMPORTANT:** Only one file can be opened at a time. If you wish to open a different file make sure to call [close_assets_file](Assets_Helper.md#function-void-close_assets_filevoid).   
*Note:* To get detailed information of the type of error simply pass the appropriate variable.

#### Function: `bool get_mesh_data(Mesh **data, uint32_t *num_meshes, File_Error *error)` 
- **Access:** `Public` 
- **Parameters:**
    - `[in, out]Mesh **data` - Pointer to the [Mesh](Mesh.md).
    - `[in, out]uint32_t *num_meshes` - Pointer to the number of meshes in the file.
    - `[in, out]File_Error *error` - Pointer to the type of `File_Error`. Can be `NULL`.
- **Return:**  `bool`   
- **Description:** Fills the specified parameters with the mesh data in case of success and returns true. Returns false otherwise.  
*Note:* To get detailed information of the type of error simply pass the appropriate variable.

#### Function: `void close_assets_file(void)` 
- **Access:** `Public` 
- **Parameters:** `void`
- **Return:**  `void`   
- **Description:** Closes the opened file.  

## Dependencies
- [Mesh](Mesh.md)