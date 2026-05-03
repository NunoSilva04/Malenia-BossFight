#include "n_time.h"
#include <stdint.h>

#if defined(_WIN32)
    #include "windows.h"

    typedef struct os_struct{
        LARGE_INTEGER counter;
        LARGE_INTEGER frequency;
    }os_data;
#elif defined(__linux__) || defined(__APPLE__)
    #ifdef _POSIX_C_SOURCE
        #define NS_OLD_POSIX_C_SOURCE _POSIX_C_SOURCE
        #undef _POSIX_C_SOURCE
    #endif
    #define _POSIX_C_SOURCE 199309L
    #include <time.h>

    typedef struct os_struct{
        timespec time;
    }os_data;
#endif

struct Core::n_time::Time_Data_t{
    explicit Time_Data_t() noexcept {}
    ~Time_Data_t() {};
    Time_Data_t(const Time_Data_t &other) = delete;
    Time_Data_t &operator=(const Time_Data &other) = delete;
    Time_Data_t(Time_Data_t &&other) = delete;
    Time_Data_t& operator=(Time_Data_t &&other) = delete;

    os_data data;
};

Core::n_time::Time_Data *Core::n_time::time_start(void){
    Time_Data *time_data =  new Time_Data_t();

#if defined(_WIN32)
    QueryPerformanceFrequency(&time_data->data.frequency);
#elif defined(__linux__) || defined(__APPLE__)
    time_data->data.time.tv_sec = 0;
    time_data->data.time.tv_nsec = 0;   
#endif    

    return time_data;
}

void Core::n_time::get_time(Time_Data *time_data){
#if defined(_WIN32)
    QueryPerformanceCounter(&time_data->data.counter);
#elif defined(__linux__) || defined(__APPLE__)
    clock_gettime(CLOCK_MONOTONIC, &time_data->data.time);
#endif

    return;
}

double Core::n_time::time_diff(Time_Data *start_time, Time_Data *end_time){
    double result;
#if defined(_WIN32)
    result = static_cast<double>(end_time->data.counter.QuadPart - start_time->data.counter.QuadPart) / static_cast<double>(start_time->data.frequency.QuadPart);
#elif defined(__linux__) || defined(__APPLE__)
    result = static_cast<double>(end_time->data.time.tv_sec - start_time->data.time.tv_sec) +
         static_cast<double>((end_time->data.time.tv_nsec - start_time->data.time.tv_nsec)) / 1e9;
#endif

    return result;
}

void Core::n_time::time_end(Time_Data *time_data){
    delete time_data;

    return;
}