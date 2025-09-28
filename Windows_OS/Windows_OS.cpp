#include "Windows_OS.h"
//#include "DirectX.h"
#include <cstdio>
#include <cwchar>
#include <winuser.h>

bool enumDisplaysAndGetWindowPref(windowPref& window){
    bool foundDisplay = false;
    unsigned int maxHz = 0;
    DISPLAY_DEVICE displayDevice;
    ZeroMemory(&displayDevice, sizeof(displayDevice));
    displayDevice.cb = sizeof(DISPLAY_DEVICE);

    for (DWORD i = 0; EnumDisplayDevices(NULL, i, &displayDevice, 0); i++) {
        foundDisplay = true;
        std::printf("Display %lu:\n", i);
        std::wprintf(L"\tDevice Name:   %hs\n", displayDevice.DeviceName);
        std::wprintf(L"\tDevice String: %hs\n", displayDevice.DeviceString);
        std::printf("\tState Flags:   0x%lx\n", displayDevice.StateFlags);
        if (displayDevice.StateFlags & DISPLAY_DEVICE_ACTIVE)
            std::puts("\t-> Active Display");
        if (displayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
            std::puts("\t-> Primary Monitor");

        unsigned int j = 0;
        DEVMODE devMode;
        ZeroMemory(&devMode, sizeof(DEVMODE));
        devMode.dmSize = sizeof(DEVMODE);
        if(EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode)){
            unsigned int hz = devMode.dmDisplayFrequency;
            std::printf("\tDM x position: %ld\n", devMode.dmPosition.x);
            std::printf("\tDM y position: %ld\n", devMode.dmPosition.y);
            std::printf("\tBits per pixel: %ld\n", devMode.dmBitsPerPel);
            std::printf("\tPixels per width: %ld\n", devMode.dmPelsWidth);
            std::printf("\tPixels per height: %ld\n", devMode.dmPelsHeight);
            std::printf("\tFrequency: %u\n", devMode.dmDisplayFrequency);
            if(hz > maxHz && (displayDevice.StateFlags & DISPLAY_DEVICE_ACTIVE)){
                maxHz = hz;
                window.x = devMode.dmPosition.x;
                window.y = devMode.dmPosition.y;
                window.width = devMode.dmPelsWidth;
                window.height = devMode.dmPelsHeight;
                window.frequency = maxHz;
            }
            j++;
        }

        std::puts("");

        // Reset for next iteration
        ZeroMemory(&displayDevice, sizeof(DISPLAY_DEVICE));
        displayDevice.cb = sizeof(DISPLAY_DEVICE);
    }

    if(!foundDisplay){
        std::printf("Couldn't find a display\n");
        return false;
    }

    return true;
}

HWND initHWND(HINSTANCE hInstance, const windowPref& window){
    if(SetProcessDPIAware() == false){
        std::printf("Couldn't set process dpi aware\n");
    }

    WNDCLASSEXA windowClass;
    ZeroMemory(&windowClass, sizeof(WNDCLASSEXA));
    windowClass.cbSize = sizeof(WNDCLASSEXA);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hInstance = hInstance;
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.lpszClassName = "MainWindow";
    
    
    if(!RegisterClassExA(&windowClass)){
        printf("Couldn't register window Class\n");
        return 0;
    }

    //std::FILE *fp = std::fopen("test.txt", "w");
    //if(fp == NULL){
    //    return 0;
    //}
    //fprintf(fp, "window.x = %d\nwindow.y = %d\nwindow.width = %d\nwindow.height = %d\n", window.x, window.y, window.width, window.height);
    //fclose(fp);

    return CreateWindowExA(0, "MainWindow", "BossFight", WS_POPUP, 0, 0, window.width, window.height, NULL, NULL, hInstance, NULL);
}

HMONITOR getMonitor(HWND hWnd){
    return MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST); 
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    uint32_t size = 0;
    u_char *lpb = nullptr;
    switch(msg){
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_SETFOCUS:
            std::printf("In focus\n");
            break;

        case WM_KILLFOCUS:
            std::printf("Out of focus\n");
            break;

        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
            if(wParam == VK_ESCAPE){
                PostQuitMessage(0);
            }
            break;

        case WM_SYSKEYUP:
        case WM_KEYUP:
            break;

        case WM_INPUT:
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));

            lpb = new u_char[size];
            if(lpb == nullptr){
                std::printf("No lpb\n");
                break;
            }

            if(GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &size, sizeof(RAWINPUTHEADER)) == size){
                RAWINPUT *raw = (RAWINPUT*)lpb;
                //if(InputManager::preparseData != nullptr){
                    //USAGE usages[15];
                    //u_long usageLength = 15;
                    //if(HidP_GetUsages(HidP_Input, 0x09, 0, usages, &usageLength, InputManager::preparseData, (PCHAR)raw->data.hid.bRawData, raw->data.hid.dwSizeHid) == HIDP_STATUS_SUCCESS){
                    //    for(u_long i = 0; i < usageLength; i++){
                    //        std::printf("Button usage pressed: %u\n", usages[i]);
                    //    }
                    //}else{
                    //    std::printf("couldn't get usage\n");
                    //}
//
                    ////Value caps
                    //HIDP_VALUE_CAPS valueCaps[32];
                    //u_short valueCapsLength = 9;
                    //if(HidP_GetValueCaps(HidP_Input, valueCaps, &valueCapsLength, InputManager::preparseData) == HIDP_STATUS_INVALID_PREPARSED_DATA){
                    //    std::printf("Couldn't get value caps\n");
                    //    return false;
                    //}

                    //for(uint32_t i = 0; i < valueCapsLength; i++){
                    //    if (!valueCaps[i].IsRange) {
                    //        USAGE usage = valueCaps[i].NotRange.Usage;
                    //         ULONG value = 0;
//
                    //        if (HidP_GetUsageValue(HidP_Input, valueCaps[i].UsagePage, 0, usage, &value, InputManager::preparseData, (PCHAR)raw->data.hid.bRawData, raw->data.hid.dwSizeHid) == HIDP_STATUS_SUCCESS){
                    //            //std::printf("Usage 0x%02X value: %u\n", usage, value);
                    //        }
                    //    }
                    //}

                    //u_long xValue = 0;;
                    //if (HidP_GetUsageValue(HidP_Input, 0x01, 0, 0x30, &xValue, InputManager::preparseData, (PCHAR)raw->data.hid.bRawData, raw->data.hid.dwSizeHid) == HIDP_STATUS_SUCCESS) {
                    //    if(abs((int) xValue - 128) > 10){
                    //        std::printf("X\n");
                    //    }
                    //}

                    //u_long lyValue = 0;
                    //if (HidP_GetUsageValue(HidP_Input, 0x01, 0, 0x31, &lyValue, InputManager::preparseData, (PCHAR)raw->data.hid.bRawData, raw->data.hid.dwSizeHid) == HIDP_STATUS_SUCCESS) {
                    //    if(abs((int) lyValue - 128) > 10){
                    //        std::printf("Y: %lu\n", lyValue);
                    //    }
                    //}

                    //u_long rxValue = 0;
                    //if (HidP_GetUsageValue(HidP_Input, 0x01, 0, 0x35, &rxValue, InputManager::preparseData, (PCHAR)raw->data.hid.bRawData, raw->data.hid.dwSizeHid) == HIDP_STATUS_SUCCESS) {
                    //    if(abs((int) rxValue - 128) > 30){
                    //        std::printf("rxValue: %lu\n", rxValue);
                    //    }
                    //}

                    //u_long ryValue = 0;
                    //if (HidP_GetUsageValue(HidP_Input, 0x01, 0, 0x34, &ryValue, InputManager::preparseData, (PCHAR)raw->data.hid.bRawData, raw->data.hid.dwSizeHid) == HIDP_STATUS_SUCCESS) {
                    //    if(abs((int) ryValue - 128) > 10){
                    //        std::printf("ryValue\n");
                    //    }
                    //}
                //}
            }

            delete[] lpb;
            break;

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}