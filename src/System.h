#ifndef _System_h_
#define _System_h_

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
static inline void* getModule (char* p) { return p == NULL ? GetModuleHandleA (p) : LoadLibraryA (p); }
#endif

#include "config.h"

// memory allocator
#include <rpmalloc.h>
#define mmalloc			rpmalloc
#define mcalloc			rpcalloc
#define mfree(name)		rpfree(name);name = NULL;

#define mem_init		rpmalloc_initialize
#define mem_clear		rpmalloc_finalize

#ifndef assert
#include <assert.h>
#endif // !assert

static inline int mralloc (void **org, size_t size) {
	assert (org && size);
	void* _new = rprealloc (*org, size);
	if (_new == NULL) return 0; // allocate fail, return

	// we success, set the new buffer
	*org = _new;
	return 1;
}

#endif