#ifndef _System_h_
#define _System_h_
#include "config.h"

#if __linux__

// load dll macro
#include <dlfcn.h>
#define getProc(p,name)     dlsym(p,name)
#define getModule(p)        dlopen(p,RTLD_NOW)

// sleep macro
#include <unistd.h>
#define Sleep(ms)           usleep(ms * 1000)
#elif _WIN32
#include <windows.h>
#define getProc(p,name)		(ptr)GetProcAddress((HMODULE)p, (char*)name)
void* getModule (char* p);
#endif
#endif