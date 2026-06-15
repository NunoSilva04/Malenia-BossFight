#include "str_array.h"
#include <stdlib.h>
#include <string.h>
#include "debug_helper.h"

#define DEFAULT_STR_ARRAY_SIZE 32
#define DEFAULT_STR_SIZE 256

static void reallocate_str_array(str_array *data){
    data->capacity *= 2;

    char **temp = data->strings;
    data->strings = (char **) malloc(data->capacity * sizeof(char *));
    for(uint32_t i = 0; i < data->capacity; i++){
        data->strings[i] = (char *) malloc(DEFAULT_STR_SIZE * sizeof(char));
        memset(data->strings[i], 0, DEFAULT_STR_SIZE * sizeof(char));
    }

    for(uint32_t i = 0; i < data->num_strings; i++){
        strcpy(data->strings[i], temp[i]);
        free(temp[i]);
    }
    free(temp);

    return;
}

str_array *allocate_str_array(uint32_t size){
    str_array *data = (str_array *) malloc(sizeof(str_array));
    if(data == NULL)
        return NULL;

    data->num_strings = 0;

    if(size == 0){
        data->capacity = DEFAULT_STR_ARRAY_SIZE;
        data->strings = (char **) malloc(DEFAULT_STR_ARRAY_SIZE * sizeof(char *));
        for(uint32_t i = 0; i < DEFAULT_STR_ARRAY_SIZE; i++){
            data->strings[i] = (char *) malloc(DEFAULT_STR_SIZE * sizeof(char));
            memset(data->strings[i], 0, DEFAULT_STR_SIZE * sizeof(char));
        }
    }else{
        data->capacity = size;
        data->strings = (char **) malloc(size * sizeof(char *));
        for(uint32_t i = 0; i < size; i++){
            data->strings[i] = (char *) malloc(DEFAULT_STR_SIZE * sizeof(char));
            memset(data->strings[i], 0, DEFAULT_STR_SIZE * sizeof(char));
        }
    }

    return data;
}   

void str_array_push_back(str_array *data, const char *string){
    if(data->num_strings == data->capacity)
        reallocate_str_array(data);

    strcpy(data->strings[data->num_strings], string);
    data->num_strings++;

    return;
}

bool str_array_check_str(str_array const *data, const char *string){
    for(uint32_t i = 0; i < data->num_strings; i++){
        if(strcmp(data->strings[i], string) == 0)
            return true;
    }

    return false;
}

void free_str_array(str_array *data){
    if(data == NULL)
        return;

    for(uint32_t i = 0; i < data->capacity; i++){
        free(data->strings[i]);
    }
    free(data->strings);
    free(data);

    return;
}

void print_str_array(str_array const* data){
    Core::debug::log(Core::debug::Info, "Str_Array Capacity: %d\n", data->capacity);
    Core::debug::log(Core::debug::Info, "Str_Array Number of Strings: %d\n", data->num_strings);
    for(uint32_t i = 0; i < data->num_strings; i++){
        Core::debug::log(Core::debug::Info, "Str_Array String[%d]: %s\n", i, data->strings[i]);        
    }

    return;
}