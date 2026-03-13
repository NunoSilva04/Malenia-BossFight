# n_vector.h

Vector Template Class. Implements a dynamically allocated array.  

## Template Parameters
- `T` - Type of the elements of the vector

## Member Types
None.

## Member variables
### Instance variables

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `data` | `T*` |Pointer to the dynamic allocated array| `nullptr` |
|`size`|`size_t`|Number of elements in the vector| `0` |
|`capacity`| `size_t`| Current capacity of the vector (initialized from `init_size`) | `32` |

### Static variables
| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `init_size` | `static constexpr size_t` | The initial size of the vector | `32` |

## Member Functions

### Instance Functions

#### Function: `n_vector()`  
- **Access:** `Public`
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Initalizes the Vector Member Types and allocates `data` with capacity.

#### Function: `~n_vector()` 
- **Access:** `Public` 
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Frees the memory stored in `data` and destroys the instance.


#### Function: `n_vector(const n_vector& other)`  
- **Access:** `Public`
- **Parameters:**  `const n_vector& other` - `n_vector` that will be copied. 
- **Return:**  `None`   
- **Description**: Copy Constructor. Copies by value the contents of the `other` vector.

#### Function: `n_vector& operator=(const n_vector& other)`  
- **Access:** `Public`
- **Parameters:**  `const n_vector& other` - `n_vector` that will be assigned.
- **Return:**  `None`   
- **Description**: Copy assignment. Deletes the data initially stored, and copies by value the contents of `other`;

#### Function: `void print(void)`  
- **Access:** `Public`
- **Parameters:**  `void`  
- **Return:**  `void`   
- **Description**: Prints the contents of the vector.

#### Function: `void push_back(const T value)`  
- **Access:** `Public`
- **Parameters:**  `[in]const T value` - Value to be added.
- **Return:**  `void`   
- **Description**: Pushes the value into the end of the vector. In case the size is equal to the capacity of the vector, it will create a new vector with double the size and copy the contents of the old vector into the new vector.

#### Function: `pop_first`
- **Description**: Removes the first element of the vector and copies the remaining elements to a new vector.
- **Constraints**: Has O(N) complexity, making it slow and inneficient for large data sizes.

##### **Overloads** 

`bool pop_first(void):` 
- **Access:** `Public`
- **Parameters:**  `void`  
- **Return:**  `bool`
- **Description:** If the vector is empty it will simply return false. Else, it will return true.

`bool pop_first(T *popped_value)`
- **Access:** `Public`
- **Parameters:**  `[out]T *popped_value` - Pointer to the value popped. 
- **Return:**  `bool`
- **Description:** Copies the first element into `popped_value` and returns true. If the vector is empty, returns false.

#### Function: `void empty_vector(void)`
- **Access:** `Public`
- **Parameters:**  `void`   
- **Return:**  `void`   
- **Description**: Releases `data` and creates a new vector with a initialize size of 32.

#### Function: `bool is_vector(void)`
- **Access:** `Public`
- **Parameters:**  `void`   
- **Return:**  `bool`   
- **Description**: Checks if the vector is empty or not. Returns true if it is, returns false if it isn't.

#### Function: `size_t vector_size(void) const`
- **Access:** `Public`
- **Parameters:**  `void`   
- **Return:**  `size_t`   
- **Description**: Returns the size of the vector.

#### Function: `size_t vector_capacity(void)`
- **Access:** `Public`
- **Parameters:**  `void`   
- **Return:**  `size_t`   
- **Description**: Returns the vector capacity.

#### Function: `T vector_data_id(size_t id)`
- **Access:** `Public`
- **Parameters:**  `[in] size_t id` - Position of the vector.
- **Return:**  `T`   
- **Description**: Returns the data of the vector at a specific location. If `id` is bigger or equal to the size of the vector, it will return garbage.

### Static Functions
None

## Dependencies
None