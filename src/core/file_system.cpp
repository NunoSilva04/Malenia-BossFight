#include "file_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t *Core::File::read_spv_file(const char *file_path, uint32_t *file_size){
    FILE *file = fopen(file_path, "rb");
    if(file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    *file_size = (uint32_t) ftell(file);
    
    uint32_t *file_data = (uint32_t *) malloc((*file_size) * sizeof(uint8_t));
    memset(file_data, 0, (*file_size) * sizeof(uint8_t));
    
    rewind(file);
    size_t out = fread(file_data, sizeof(uint8_t), (*file_size), file);
    if(out != (size_t) ((*file_size) / sizeof(uint8_t)))
        return NULL;

    fclose(file);

    return file_data;
}