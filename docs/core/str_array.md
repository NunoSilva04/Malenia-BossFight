# str_array.h
Implementation of a dynamically allocated array of strings.  

## Member Types
### `typedef struct str_array`

Structure that holds the contents of the array of strings.

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `num_strings` | `uint32_t` |The number of strings in the array| `0` |
|`capacity`|`uint32_t`|The capacity of the array of strings| `-` |
|`strings`| `char **`| Pointer to the strings | `NULL` |

### Functions

#### Function: `str_array *allocate_str_array(uint32_t size)` 
- **Parameters:** `[in]uint32_t size` - The size of the structure
- **Return:**  `str_array *`   
- **Description:** Allocates a new `str_array` with the size provided. If no size is provided it will returns the default value of 32. Returns a valid pointer to the appropriate type or NULL in case of failure.  
<u>**Note:**</u> Although you can instantiate the structure yourself, it is advised agaisnt it and the creation and cleanup of `str_array` should be done throught the appropriate functions ([str_arra](str_array#function-str_array-allocate_str_arrayuint32_t-size) and [free_str_array](str_array#function-void-free_str_arraystr_array-data)).

#### Function: `void str_array_push_back(str_array *data, const char *string)` 
- **Parameters:** 
    - `[in, out]str_array *data` - A valid pointer to a `str_array` structure.
    - `[in]const char *string` - A valid pointer to a string.
- **Return:**  `void`   
- **Description:** Inserts the string into the `str_array` structure. If the structure is not big enough, it doubles the size of the array.

#### Function: `bool str_array_check_str(str_array const *data, const char *string)` 
- **Parameters:** 
    - `[in]str_array const *data` - A valid pointer to a `str_array` structure.
    - `[in]const char *string` - A valid pointer to string.
- **Return:**  `bool`   
- **Description:** Checks if the string is present in the array and returns true if it is. Returns false otherwise.

#### Function: `void free_str_array(str_array *data)` 
- **Parameters:** `[in]str_array *data` - A valid pointer to a `str_array` structure.
- **Return:**  `void`   
- **Description:** Frees the pointer to the `str_array` structure.  
<u>**Note:**</u> Although you can free the structure yourself, it is advised agaisnt it and the creation and cleanup of `str_array` should be done throught the appropriate functions ([str_arra](str_array#function-str_array-allocate_str_arrayuint32_t-size) and [free_str_array](str_array#function-void-free_str_arraystr_array-data)).

#### Function: `void print_str_array(str_array const* data)` 
- **Parameters:** `[in]str_array *data` - A valid pointer to a `str_array` structure.
- **Return:**  `void`   
- **Description:** Prints all of the strings in the provided structure.

## Dependencies
None.