#include "Usage.h"
#include <cstdio>
#include <windows.h>
#include <psapi.h>
#include <cstdint>

static ULONGLONG lastSysKernel, lastSysUser;
static ULONGLONG lastProcKernel, lastProcUser;

void System::printMemoryUsage(){
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))){
        uint64_t memoryUsed = pmc.WorkingSetSize / (1024 * 1024);
        if(memoryUsed >= 1024){
            memoryUsed /= 1024;
            std::printf("Memory Used: %I64u GB\n", memoryUsed);
        }else{
            std::printf("Memory Used: %I64u MB\n", memoryUsed);
        }
    }
}

void System::initCPU_Usage(){
    //FILETIME idleTime, kernelTime, userTime;
    //FILETIME procCreation, procExit, procKernel, procUser;
//
    //GetSystemTimes(&idleTime, &kernelTime, &userTime);
    //GetProcessTimes(GetCurrentProcess(), &procCreation, &procExit, &procKernel, &procUser);
    
    //lastSysKernel = static_cast<ULONGLONG>(kernelTime.dwHighDateTime << 32 | kernelTime.dwLowDateTime);
    //lastSysUser   = static_cast<ULONGLONG>(userTime.dwHighDateTime << 32 | userTime.dwLowDateTime);
    //lastProcKernel = static_cast<ULONGLONG>(procKernel.dwHighDateTime << 32 | procKernel.dwLowDateTime);
    //lastProcUser   = static_cast<ULONGLONG>(procUser.dwHighDateTime << 32 | procUser.dwLowDateTime);
}

void System::getCPU_Usage(){
    //Sleep(100);
    //FILETIME currIdleTime, currKernelTime, currUserTime;
    //FILETIME currProcCreation, currProcExit, currProcKernel, currProcUser;
//
    //GetSystemTimes(&currIdleTime, &currKernelTime, &currUserTime);
    //GetProcessTimes(GetCurrentProcess(), &currProcCreation, &currProcExit, &currProcKernel, &currProcUser);

    //ULONGLONG sysKernel = static_cast<ULONGLONG>(currKernelTime.dwHighDateTime << 32 | currKernelTime.dwLowDateTime);
    //ULONGLONG sysUser   = static_cast<ULONGLONG>(currUserTime.dwHighDateTime << 32 | currUserTime.dwLowDateTime);
    //ULONGLONG procKernel = static_cast<ULONGLONG>(currProcKernel.dwHighDateTime << 32 | currProcKernel.dwLowDateTime);
    //ULONGLONG procUser   = static_cast<ULONGLONG>(currProcUser.dwHighDateTime << 32 | currProcUser.dwLowDateTime);

    //ULONGLONG sysDelta = (sysKernel - lastSysKernel) + (sysUser - lastSysUser);
    //ULONGLONG procDelta = (procKernel - lastProcKernel) + (procUser - lastProcUser);

    //lastSysKernel = sysKernel;
    //lastSysUser   = sysUser;
    //lastProcKernel = procKernel;
    //lastProcUser   = procUser;
//
    //std::printf("CPU usage: %.2f%\n", (static_cast<float>(procDelta) / static_cast<float>(sysDelta)) * 100.0f);
}