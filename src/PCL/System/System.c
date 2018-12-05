#include "System.h"

#if _WIN32
void* getModule (char* p) { return p == NULL ? GetModuleHandleA (p) : LoadLibraryA (p); }
#endif