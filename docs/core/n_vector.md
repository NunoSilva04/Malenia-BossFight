# n_vector.h

Vector Template Class. Implements a dynamically allocated array.  
Cannot perform copy from contructors or from other vectors instantiation.

## Class Parameters
- `T` - Type of the elements of the vector

## Member Types

| Member | Type | Description | Value Value |
|--------|------|-------------|-------------|
| `data` | `T*` |Pointer to the dynamic allocated array| `nullptr` |
|`size`|`size_t`|Number of elements in the vector| `0` |
|`capacity`| `size_t`| Current capacity of the vector (initialized from `init_size`) | `32` |
| `init_size` | `static constexpr size_t` | The initial size of the vector | `32` |

## Function members

### Function: `n_vector()`  
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Initalizes the Vector Member Types and allocates `data` with capacity.

### Function: `~n_vector()`  
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Frees the memory stored in `data` and destroys the instance.


### Function: `n_vector(const n_vector&) = delete`  
- **Parameters:**  `const n_vector&`  
- **Return:**  `None`   
- **Description**: Disables copy construction.

### Function: `n_vector& operator=(const n_vector&) = delete`  
- **Parameters:**  `const n_vector&`  
- **Return:**  `None`   
- **Description**: Disables copy assignment.