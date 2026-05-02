/**********************************************************************************
*   File: file_system.h
* 
*   Description: 
*        Handles different types of files.
* 
*   See: 
*       docs/core/file_system.md 
*       for detailed documentation.
* 
*************************************************************************************/
#include <stdint.h>

namespace Core{
    namespace File{
        uint32_t *read_spv_file(const char *file_path, uint32_t *file_size);
    };
};