#include <windows.h>
#include <cstdio>
#include "GameLauncher.h"
#include "Usage.h"
#include "Windows_OS.h"
#include "Game.h"
#include "DebugUtils.hpp"

LARGE_INTEGER start_frame, end_frame, frequency; 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow){
    if(!openConsole()){
        MessageBoxA(nullptr, "Couldn't open console", "Error", MB_OK);
        return 0;
    }
    if(!createOutputHandleFile("MemoryLogs.txt")){
        MessageBoxA(nullptr, "Couldn't create output file handle", "Error", MB_OK);
        return 0;
    }

    if(!isDirectXInstalled()){
        installDirectX();
    }

    windowPref window;
    if(!enumDisplaysAndGetWindowPref(window)){
        return 0;
    }
    HWND hWnd;
    hWnd = initHWND(hInstance, window);
    HMONITOR hMonitor;
    hMonitor = getMonitor(hWnd);
    ShowWindow(hWnd, CmdShow);

    Game bossFight;
    if(!bossFight.initializeGame(hWnd, hMonitor, window)){
        std::printf("Couldn't initialize game\n");
        return 0;
    }
    System::printMemoryUsage();

    //Time 
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start_frame);
    QueryPerformanceCounter(&end_frame);
    int framesPerSecond = 0;
    double timer = 0, prev_frame;

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while(true){
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            if(msg.message == WM_QUIT){
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }else{
            framesPerSecond += 1;
            prev_frame = static_cast<float>((end_frame.QuadPart - start_frame.QuadPart)) / frequency.QuadPart;
            timer += prev_frame;
            if(timer > 1.0f){
                System::printMemoryUsage();
                std::printf("FPS: %ld\n", framesPerSecond);
                timer = 0;
                framesPerSecond = 0;

            }
            QueryPerformanceCounter(&start_frame);
            bossFight.updateGame(static_cast<float>(prev_frame));
            bossFight.renderGame();          
            QueryPerformanceCounter(&end_frame);
        }
    }

    bossFight.cleanGame();
    closeConsole();
    closeOutputFileHanlde();
    return msg.wParam;
}