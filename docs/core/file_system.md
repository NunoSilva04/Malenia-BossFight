# file_system.h
File namespace that handles how to read, write and create different types of files.

## Member functions
### Instance Functions

#### Function: `Core::n_vector<char> read_binary_file(const char *file_path)`  
- **Access:** `Public`
- **Parameters:**  `[in]const char *file_path` - path to the file.  
- **Return:**  `Core::n_vector<char>` - Vector of binary data.   
- **Description**: Reads the file and returns a vector which holds the binary data of the file. Returns an empty string in case it couldn't read the file.

## Dependencies
- [n_vector](n_vector.md)