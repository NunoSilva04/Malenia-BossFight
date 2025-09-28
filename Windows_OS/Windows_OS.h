#ifndef __WINDOWS_OS_H__
#define __WINDOWS_OS_H__

#include <windows.h>
#include <cstdint>

typedef struct windowPreferances{
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    uint32_t frequency;
}windowPref;

bool enumDisplaysAndGetWindowPref(windowPref&);
HWND initHWND(HINSTANCE hInstance, const windowPref&);
HMONITOR getMonitor(HWND hWnd);
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif