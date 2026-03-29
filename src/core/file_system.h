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
#include "n_vector.h"

namespace Core{
    namespace File{
        Core::n_vector<char> read_binary_file(const char *file_path);
    };
};