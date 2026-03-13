/**********************************************************************************
*   File: debug_helper.h
* 
*   Description: 
*       Debug helper library macros and utilities.
* 
*   See: 
*       docs/core/debug_helper.md 
*       for full documentation of the header
* 
*************************************************************************************/

#ifndef __DEBUG_HELPER_H__
#define __DEBUG_HELPER_H__

#include <stdio.h>
#include <cstdarg>
#include <cstdlib>

namespace Core{
    namespace debug{
        typedef enum Log_Level{
            Fatal,
            Error,
            Warning,
            Info,
        }Log_Level;
    
        void log(Log_Level level, const char *string, ...);
        void terminate(Log_Level level);
    };
};

#endif 