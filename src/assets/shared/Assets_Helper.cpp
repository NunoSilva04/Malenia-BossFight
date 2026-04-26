#include "Assets_Helper.h"
#include <stdio.h>
#include <stdlib.h>

static FILE *file = NULL;

bool open_assets_file(const char *assets_file, File_Error *error){
    file = fopen(assets_file, "rb");
    if(file == NULL){
        *error = NOT_FOUND;
        return false;
    }

    File_Format file_format {};
    fread(&file_format, sizeof(File_Format), 1, file);
    if(file_format.magic != magic){
        *error = CORRUPTED_FILE;
        return false;
    }

    if(file_format.version != version){
        *error = WRONG_VERSION;
        return false;
    }

    return true;
}

bool get_mesh_data(Mesh **data, uint32_t *num_meshes, File_Error *error){
    fread(num_meshes, sizeof(uint32_t), 1, file);
    if(*num_meshes <= 0){
        *error = NO_MESH_DATA;
        return false;
    }

    *data = (Mesh *) malloc((*num_meshes) * sizeof(Mesh));

    for(uint32_t i = 0; i < *num_meshes; i++){
        fseek(file, MAX_MESH_NAME, SEEK_CUR);
        uint32_t num_vertices = 0;
        fread(&num_vertices, sizeof(uint32_t), 1, file);
        create_mesh(&(*data)[i], num_vertices);
        (*data)[i].num_vertices = num_vertices;
        for(uint32_t j = 0; j < num_vertices; j++){
            fread(&(*data)[i].vertices[j], sizeof(Vertex), 1, file);
        }
    }

    return true;
}

void close_assets_file(void){
    fclose(file);

    return;
}