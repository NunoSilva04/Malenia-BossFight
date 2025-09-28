#define _CRTDBG_MAP_ALLOC

#include "debugUtils.hpp"
#include <Windows.h>

FILE *outputFile = nullptr; 
HANDLE hOutputFile;

/*
 * In freopen_s we are basically redirecting the stdout or any other argument to another output
 * This can only be done with once, meaning if we did:
 * freopen_s(&fp, "CONOUT$", "w", stdout)
 * freopen_s(&outputFile, "output.txt", "w", stdout)
 * it would only be redirected to the last instance so the output file
 * 
 * So in this case "CONOUT$" redirects to the console
 * in here we don't need to track the new stream, in this case fp, all we need to do is free the console after we are done
 * 
 * And if we had something like "log.txt" it would create a file and write it there instead
 * When using file and redirecting to them we must track the new stream meaning the new fp
 * 
 * fflush only flushes what currently it has in memory into the desired stream. Meaning we can do it multiple times
 * can be done to both stdout with console or to the file in question
 */
bool openConsole(void){
    if(!AllocConsole()){
        return false;
    }

    FILE *fp = nullptr;
    errno_t error = freopen_s(&fp, "CONOUT$", "w", stdout);
    if(error != 0 || fp == nullptr){
        return false;
    }

    return true;
}

bool createOutputFile(const char *fileName){
    if(fileName == nullptr){
        return false;
    }

    outputFile = fopen(fileName, "w");
    if(outputFile == nullptr){
        return false;
    }

    return true;
}

bool createOutputHandleFile(const char* filename){
    #ifdef _DEBUG
        #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #endif

    hOutputFile = CreateFileA(filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 
                            FILE_ATTRIBUTE_NORMAL, NULL);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, hOutputFile);                        
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    return true;
}

FILE *getOutputFileStream(void){
    return outputFile;
}

void closeOutputFile(void){
    fflush(outputFile);
    fclose(outputFile);
}

void closeConsole(void){
    fflush(stdout);
    FreeConsole();
}

void closeOutputFileHanlde(void){
    _CrtDumpMemoryLeaks();
    CloseHandle(hOutputFile);
}