# Rect.h
Template structure that creates a rectangle

## Template Parameters

- `T` - Type of the rectangle. Can only be int, float or double. 

## Member Types 
None

## Member Variables

### Instance Variables

| Members | Type | Description | Default Value |
|--------|------|-------------|-------------|
| `x` | `T` |X position of the rectangle in screen coordinates| `0` |
|`y`|`T`|Y position of the rectangle in screen coordinates| `0` |
|`width`| `T`| Width of the rectangle | `20` |
| `heigth` | `T` |Height of the rectangle| `10` |

### Static Variables
None

## Member functions

### Instance Functions

#### Function: `rect(): x(0), y(0), width(20), heigth(10)`
- **Access:** `Public`  
- **Parameters:**  `None`  
- **Return:**  `None`  
- **Description**: Creates an instance of `rect` and initializes the rect to the values assigned in the initializer list.

#### Function: `rect(T x, T y, T w, T h)` 
- **Access:** `Public` 
- **Parameters:** 
    - `[in]T x` - X position of the rectangle.
    - `[in]T y` - Y Position of the rectangle.
    - `[in]T width` - Width of the rectangle.
    - `[in]T heigth` - Height of the rectangle 
- **Return:**  `None`  
- **Description**: Creates an instance of `rect` and assigns each parameter to it's corresponding value.

#### Function: `void print(void)`  
- **Access:** `Public`
- **Parameters:**  `void`  
- **Return:**  `void`  
- **Description**: Prints the `rect` variables.

### Static Functions
None

## Dependencies
None