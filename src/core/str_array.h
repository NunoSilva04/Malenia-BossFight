/**********************************************************************************
*   File: str_array.h
* 
*   Description: 
*        An array of strings structure responsible for managing multiple strings.
* 
*   See: 
*       docs/Core/str_array.md 
*       for detailed documentation.
* 
*************************************************************************************/

#ifndef __STR_ARRAY_H__
#define __STR_ARRAY_H__

#include <stdint.h>

typedef struct {
    uint32_t num_strings;
    uint32_t capacity;
    char **strings;
}str_array;

str_array *allocate_str_array(uint32_t size);
void str_array_push_back(str_array *data, const char *string);
bool str_array_check_str(str_array const *data, const char *string);
void free_str_array(str_array *data);
void print_str_array(str_array const* data);

#endif  // __STR_ARRAY_H__