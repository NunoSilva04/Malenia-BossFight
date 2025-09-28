#include "GameLauncher.h"

#include <windows.h>
#include <Shlwapi.h>

bool isDirectXInstalled(){
    char sysPath[200];
    GetSystemDirectoryA(sysPath, 200);
    PathAppendA(sysPath, "d3dcompiler_47.dll");
    return PathFileExistsA(sysPath);
}

void installDirectX(){
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    char sysPath[200] = "Redistribute\\DXSETUP.exe";
    if(!PathFileExistsA(sysPath)){
        exit(-1);
    }

    SHELLEXECUTEINFOA sei;
    ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.lpVerb = "runas";
    sei.lpFile = sysPath;
    sei.lpParameters = " /silent";
    sei.nShow = SW_SHOWNORMAL;
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;

    if(!ShellExecuteExA(&sei)){
        exit(-1);
    }

    WaitForSingleObject(sei.hProcess, INFINITE);
    CloseHandle(sei.hProcess);
}