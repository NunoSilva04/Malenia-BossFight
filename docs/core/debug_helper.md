# debug_helper.h
Debug namespace responsible for logging messages to the console.

## Member Types
### `typedef enum Log_Level Log_Level`
Enumerates all of the possible Log levels 

| Events         | Description         |
| ---------------|---------------|
| Fatal| Very high severity Error. Terminates the program. |
| Error| High severity error. If 10 or more errors are outputted, the program will be terminated. |
| Warning | Low severity error. If 10 warnings are outputted, the progra will be terminated. |
| Info | No severity. Outputs information to the console |

## Member functions

#### Function: `void log(Log_Level level, const char *string, ...)`  
- **Parameters:**  
    - `[in]Log_Level level` - Severity of the message.
    - `[in]const char *string` - String to output.
    - `[in]...` - Variadic arguments for the string.
- **Return:**  `void`   
- **Description**: Logs string into the console. 
    - If `level == Error`, increments internal error counter
    - If `level == Warning`, increments internal warning counter.

#### Function: `void terminate(Log_Level level)`  
- **Parameters:** `[in]Log_Level level` - Severity of the message.
- **Return:**  `void`   
- **Description**: Terminates the program if `level == Fatal` or if already too many errors were detected.

## Dependencies
None.