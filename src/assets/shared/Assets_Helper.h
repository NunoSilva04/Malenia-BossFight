/**********************************************************************************
*   File: Assets_Helper.h
* 
*   Description: 
*       Responsible for reading the ".assets" file.
* 
*   See: 
*       docs/assets/Assets_Shared.md 
*       for detailed documentation.
* 
*************************************************************************************/

#ifndef __ASSETTS_HELPER_H__
#define __ASSETTS_HELPER_H__

#include <stdint.h>
#include "Mesh.h"

#define MAX_MESH_NAME 256 

static constexpr uint32_t magic = 0x0C1B5D81;   
static constexpr uint32_t version = 0x01000000;

typedef enum File_Error{
    NOT_FOUND,
    CORRUPTED_FILE,
    WRONG_VERSION,
    NO_MESH_DATA
}File_Error;

typedef struct File_Format{
    File_Format() noexcept{
        this->magic = ::magic;
        this->version = ::version;
    }

    uint32_t magic;
    uint32_t version;
}File_Format;

bool open_assets_file(const char *assets_file, File_Error *error);
bool get_mesh_data(Mesh **data, uint32_t *num_meshes, File_Error *error);
void close_assets_file(void);

#endif // __ASSETTS_HELPER_H__