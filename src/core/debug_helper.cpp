#include "debug_helper.h"

static int num_errors = 0;
static int num_warnings = 0;

void Core::debug::log(Log_Level level, const char *string, ...){
    #if DEBUG
    switch(level){
        case Fatal:
            printf("Fatal:  ");
        break;

        case Error:
            num_errors++;
            printf("Error:  ");
        break;

        case Warning:
            num_warnings++;
            if(num_warnings >= 10){
                num_warnings = 0;
                num_errors++;
            }
            printf("Warning:  ");
        break;

        case Info:
        default:
            printf("Info:  ");
        break;
    };

    va_list args;
    va_start(args, string);
    vprintf(string, args);
    va_end(args);

    terminate(level);

    #endif  //DEBUG
}

void Core::debug::terminate(Log_Level level){
    if(level == Fatal){
        printf("Fatal error: Terminating process\n");
        exit(EXIT_FAILURE);
    }else if(num_errors >= 10){
        printf("Too many errors: Terminating process\n");
        exit(EXIT_FAILURE);
    }
}