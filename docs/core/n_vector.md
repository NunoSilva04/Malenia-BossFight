# n_vector.h

Vector Template Class. Implements a dynamically allocated array.  
Cannot perform copy from contructors or from other vectors instantiation.

## Class Parameters
- `T` - Type of the elements of the vector

## Member Types

| Members | Type | Description | Default Value |
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

### Function: `void print(void)`  
- **Parameters:**  `void`  
- **Return:**  `void`   
- **Description**: Prints the contents of the vector.

### Function: `void push_back(const T value)`  
- **Parameters:**  `[in]const T value`   
- **Return:**  `void`   
- **Description**: Pushes the value into the end of the vector. In case the size is equal to the capacity of the vector, it will create a new vector with double the size and copy the contents of the old vector into the new vector.

### Function: `pop_first`
- **Description**: Deletes the first element of the vector
- **Constraints**: Has O(N) complexity, making it slow and inneficient for large data sizes 

#### **Overloads** 

`bool pop_first(void):` 
- **Parameters:**  `void`  
- **Return:**  `bool`
- **Description:** Pops the first element of the vector, returning true, and creates a new vector with one less element and copies the content of the old vector into the new vector. If the vector is empty it will simply return false.

`bool pop_first(T *popped_value)`
- **Parameters:**  `[out]T *popped_value`  
- **Return:**  `bool`
- **Description:** Pops the first element of the vector, returning true, and creates a new vector with one less element and copies the content of the old vector into the new vector. Populates the `popped_value` with the element popped from the vector If the vector is empty it will simply return false.

### Function `void empty_vector(void)`
- **Parameters:**  `void`   
- **Return:**  `void`   
- **Description**: Releases `data` and creates a new vector with a initialize size of 32.

### Function `bool is_vector(void)`
- **Parameters:**  `void`   
- **Return:**  `bool`   
- **Description**: Checks if the vector is empty or not. Returns true if it is, returns false if it isn't.