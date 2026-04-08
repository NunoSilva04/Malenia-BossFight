/**********************************************************************************
*   File: n_time.h
* 
*   Description: 
*        High resolution timer for benchmarking calculations.
* 
*   See: 
*       docs/core/n_time.md 
*       for full documentation of the header
* 
*************************************************************************************/

#ifndef __N_TIME_H__
#define __N_TIME_H__

namespace Core{
    namespace n_time{
        typedef struct Time_Data_t Time_Data;

        Time_Data *time_start(void);
        void get_time(Time_Data *time_data);
        double time_diff(Time_Data *start_time, Time_Data *end_time);
        void time_end(Time_Data *time_data);
    };
};

#endif