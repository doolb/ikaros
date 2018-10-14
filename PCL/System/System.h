#pragma once
#include "../config.h"

#if 1
#include <windows.h>
#define getProc(p,name) (ptr)GetProcAddress((HMODULE)p, (char*)name)

#define getModule (ptr)GetModuleHandleA

#define loadModule (ptr)LoadLibraryA

#endif

