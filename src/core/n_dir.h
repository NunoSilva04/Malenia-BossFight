/**********************************************************************************
*   File: n_dir.h
* 
*   Description: 
*       Directory handling for Windows, MacOS and Linux.
* 
*   See: 
*       docs/core/n_dir.md 
*       for detailed documentation.
* 
*************************************************************************************/

#ifndef __N_DIR_H__
#define __N_DIR_H__

#include <stdlib.h>

namespace Core{
    namespace n_dir{
        typedef struct Directory_Handle_t Dir_Handle;

        Dir_Handle *extract_dir_contents(const char *directory_name);
        void get_dir_file_paths(Dir_Handle *handle, size_t *num_files, char ***file_paths);
        void get_dir_file_names(Dir_Handle *handle, size_t *num_files, char ***file_names);
        bool create_dir(const char *dir_path);
    }
}

#endif