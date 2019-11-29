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
#include <ctype.h>

#define MAX_PATH 260

#elif _WIN32
#include <windows.h>
#define getProc(p,name)		(ptr)GetProcAddress((HMODULE)p, (char*)name)
static inline void* getModule (char* p) { return p == NULL ? GetModuleHandleA (p) : LoadLibraryA (p); }
#endif

#include "config.h"

// memory allocator
#ifdef USE_RPMALLOC
#include <rpmalloc.h>
#define mmalloc			rpmalloc
#define mcalloc			rpcalloc
#define mralloc			rprealloc
#define mfree(name)		rpfree(name);name = NULL;

#define mem_init		rpmalloc_initialize
#define mem_clear		rpmalloc_finalize
#else
#define mmalloc			malloc
#define mcalloc			calloc
#define mralloc			realloc
#define mfree(name)		free(name);name = NULL;

#define mem_init()		
#define mem_clear()		
#endif
#include "mappedfile.h"

#ifndef assert
#include <assert.h>
#endif // !assert

#define idx		(*_idx)
#define line	(*_line)
#define column	(*_column)
#define start	(*_start)
#define end	(*_end)
// string tools
// find sub string from end
static inline char* strrnstr(char* _Str, int _str_len, char* _SubStr, int _sub_str_len) {
	assert(_Str && _SubStr && _str_len && _sub_str_len);
	if(_sub_str_len > _str_len)
		return NULL;
	if (_sub_str_len == _str_len)
		return strncmp(_Str, _SubStr, _str_len) == 0 ? _Str : NULL;
	for (int i = _str_len - _sub_str_len; i >= 0; i--) {
		if(strncmp(_Str + i, _SubStr, _sub_str_len) == 0)
			return _Str + i;
	}
	return NULL;
}

static inline int parseEmpty(char* buff, int buff_len, int *_idx, int *_line, int *_column) {
	assert(buff && buff_len && _idx && _line && _column);
	while (idx < buff_len)
	{
		char ch = buff[idx];
		if(ch == '\0') break;
		else if (ch == '\n') { line++; column = -1; }
		else if(isgraph(ch))return idx;
		idx++;
		column++;
	}
	return EOF;
}

static inline int parseWord(char* buff, int buff_len, int *_idx, int *_line, int *_column, int* _start, int *_end) {
	assert(buff && buff_len && _idx && _line && _column && _start && _end);
	bool inword = false;
	start = -1;
	end = -1;
	while (idx < buff_len) {
		char ch = buff[idx];
		if (ch == '\0') break;
		else if (ch == '\n') { line++; column = -1; }
		else if (isalnum(ch) || ch == '_') {
			if (!inword) {
				inword = true;
				start = idx;
			}
		}
		else if (inword) {
			inword = false;
			end = idx - 1;
		}
		if (start != -1 && end != -1 && !inword) {
			//idx++;
			//column++;
			return idx;
		}
	//Next_Char:
		idx++;
		column++;
	}
	return idx;
}

static inline int parseSymbol(char* buff, int buff_len, int *_idx, int *_line, int *_column, int* _start, int *_end,char sch, char ech) {
	assert(buff && buff_len && _idx && _line && _column && _start && _end);
	int symbols = 0;
	start = -1;
	end = -1;
	while (idx < buff_len) {
		char ch = buff[idx];
		if (ch == '\0') break;
		else if (ch == '\n') { line++; column = -1; }
		else if (ch == sch) {
			if(!symbols) start = idx + 1;
			symbols++;
		}
		else if (ch == ech) {
			symbols--;
			if(!symbols) end = idx - 1;
		}
		if (start != -1 && end != -1 && !symbols) {
			idx++;
			column++;
			return idx;
		}
	//Next_Char:
		idx++;
		column++;
	}
	return EOF;
}

static inline int parseCfgString(char* buff, int buff_len, int *_idx, int *_line, int *_column, int* _start, int *_end) {
	assert(buff && buff_len && _idx && _line && _column && _start && _end);
	int singles = 0, doubles = 0;
	start = -1;
	end = -1;
	while (idx < buff_len)
	{
		char ch = buff[idx];
		if (ch == '\0') break;
		else if (ch == '\n') { line++; column = -1; break;} // current dont support '\'
		else if (ch == '"') {
			if(*_start == -1) (*_start) = idx + 1;
			doubles = doubles ? 0 : 1;
		}
		else if (ch == '\'') {
			if (start == -1) start = idx + 1;
			singles = singles ? 0 : 1;
		}

		if (start != -1 && singles == 0 && doubles == 0) {
			end = idx - 1;
			idx++;
			column++;
			return idx;
		}
		idx++;
		column++;
	}
	*_start = -1;
	return EOF;
}
#undef idx
#undef line
#undef column
#undef start
#undef end

// time
#ifdef __CYGWIN32__
double RealElapsedTime(void) { // returns 0 seconds first time called
	static struct timeval t0;
	struct timeval tv;
	gettimeofday(&tv, 0);
	if (!t0.tv_sec)
		t0 = tv;
	return tv.tv_sec - t0.tv_sec + (tv.tv_usec - t0.tv_usec) / 1000000.;
}
#else
#include <windows.h>
double RealElapsedTime(void) { // granularity about 50 microsecs on my machine
	static LARGE_INTEGER freq, start;
	LARGE_INTEGER count;
	if (!QueryPerformanceCounter(&count))
		perror("QueryPerformanceCounter");
	if (!freq.QuadPart) { // one time initialization
		if (!QueryPerformanceFrequency(&freq))
			perror("QueryPerformanceFrequency");
		start = count;
	}
	return (double)(count.QuadPart - start.QuadPart) / freq.QuadPart;
}
#endif

// log
#ifndef No_Debug
#define _debug_(fmt, ...)		printf("%s,%d,%s," ## fmt ## "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define _debug_(fmt, ...)		
#endif
#define _info_(fmt, ...)		printf("%s,%d,%s," ## fmt ## "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

// status 
typedef enum _Status
{
	Status_OK,
	Status_Memory_Overflow,
	Status_Paramter_Error,
}Status;

// class define
#define IDispose(T)				\
	T*		(*New)();			\
	void	(*Del)(T* *shader);	\
	int		(*Init)(T*)
#define IDispose_Def(New, Del, Init)	New,Del,Init

#define IParse(T)				\
	bool	(*Parse)(T *ptr, string source)

#define IParseFile(T)			\
	bool	(*ParseFile)(T *ptr, string filename)

#define ILoad(T)				\
	T*		(*Load)(string filename)

#define ITable(TTable, TValue)	\
	bool	(*Add)(TTable *table, void* key, int key_bit, TValue *value);	\
	void*	(*Get)(TTable *table, void* key, int key_bit)
#define ITable_Def(Add, Get)			Add,Get

#define IArray(T)				\
	int		count;				\
	int		max;				\
	T*		items;
#define IArray_New(arr)			\
	(arr)->count = 0;			\
	(arr)->max = 0;				\
	(arr)->items = NULL;
#define IArray_Del(arr, func_free)		\
	(arr)->count = 0;					\
	(arr)->max = 0;						\
	if((arr)->items != NULL) func_free  \
	(arr)->items = NULL;
#endif