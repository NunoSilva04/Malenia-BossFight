#ifndef __DEBUG_UTILS_H__
#define __DEBUG_UTILS_H__

#include <cstdio>
#include <crtdbg.h>

bool openConsole(void);
bool createOutputFile(const char *);
bool createOutputHandleFile(const char*);
FILE *getOutputFileStream(void);
void closeConsole(void);
void closeOutputFile(void);
void closeOutputFileHanlde(void);

#endif