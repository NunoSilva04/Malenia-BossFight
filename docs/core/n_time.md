# n_time.h
Namespace of a cross-platform, high resolution timer functionalities for benchmarking calculations, such as FPS calculations.  

## Member Types
### `typedef struct Time_Data_t Time_Data`

Opaque pointer to the time data struture.

## Member Variables
None.

## Member functions
### Instance Functions

#### Function: `Time_Data *time_start(void)` 
- **Access:** `Public` 
- **Parameters:** `void` 
- **Return:**  `Time_Data *`   
- **Description:** Initializes the `Time_Data` structure and returns a pointer to it.  
<u>**Note:**</u> Must be the first function to be called before any other.

#### Function: `void get_time(Time_Data *time_data)` 
- **Access:** `Public` 
- **Parameters:** `Time_Data *time_data` - A valid pointer to a `Time_Data` structure.  
- **Return:**  `void`   
- **Description:** Gets the current high resolution timestamp and stores it in **time_data**.

#### Function: `double time_diff(Time_Data *start_time, Time_Data *end_time)` 
- **Access:** `Public` 
- **Parameters:** 
    - `Time_Data *start_time` - A valid pointer to a `Time_Data` structure. The first high resolution timestamp measured.
    - `Time_Data *end_time` - A valid pointer to a `Time_Data` structure. The last high resolution timestamp measured.
- **Return:**  `double`   
- **Description:** Calculates the time difference and returns it, in seconds.

#### Function: `void time_end(Time_Data *time_data)` 
- **Access:** `Public` 
- **Parameters:** `Time_Data *time_data` - A valid pointer to a `Time_Data` structure.
- **Return:**  `double`   
- **Description:** Frees the pointer to the `Time_Data` structure.  
<u>**Note:**</u> Must be the last function to be called.

## Dependencies
None.