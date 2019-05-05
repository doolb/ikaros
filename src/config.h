#ifndef _config_h_
#define _config_h_
#define mfloat	float
#define mint	int
#define null	{ 0 }

#if __GNUC__
#define API __attribute__((visibility("default")))
#else
#define API __declspec( dllexport )
#endif

typedef unsigned long long 		ptr;
typedef void*					pvoid;
typedef unsigned char			byte;

// define interger
typedef long long llong;	// 64-bit
#if _MSC_VER
typedef unsigned long long ulong;		// 64-bit unsigned
#endif
typedef unsigned int uint;			// 32-bit

// get the offset of a member in type
#ifndef offsetof
#define offsetof(Type, member) ((size_t) &((Type *)0)->member)
#endif

#ifndef Array_Size
#define Array_Size(a) (sizeof(a) / sizeof((a)[0]))
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#if !defined(bool)
#define bool int
#define true 1
#define false 0
#endif // !bool
#define disable -1

#ifndef string
#define string char*
typedef wchar_t wchar;
#define wstring wchar_t*
#endif

// max,min,and clamp
#if !defined(Max)
#define Max(a,b) ((a)>(b) ? (a) : (b))
#define Min(a,b) ((a)>(b) ? (b) : (a))
#define Clamp(val, min, max) (Max((min),Min((val),(max))))
#endif


#define STRUCT_FUNC 1	// enable struct function support
#define _(Type) Type##_Func
#define _Func(Type) struct Type##_Func

// get the ptr of a member in type
// #define containof(ptr,Type,member) ({                       \
//     const typeof( ((Type *)0)->member ) *__mptr = (ptr);    \
//     (Type *)( (char *)__mptr - offsetof(Type,member) );})   
#define containof(ptr,Type,member) \
    (Type *)( (char *)ptr - offsetof(Type,member) )

// get the ptr of a member in type
#define List_Entry(ptr,Type,member) \
    (Type *)( (char *)ptr - offsetof(Type,member) )

// log output
#define LOG_DEBUG 1
#define LOG_WARRING 2
#define LOG_ERROR 4
static uint g_log = LOG_DEBUG | LOG_WARRING | LOG_ERROR;
#define logd(...) g_log & LOG_DEBUG ? printf(__VA_ARGS__) : (void)0
#define logw(...) g_log & LOG_WARRING ? printf(__VA_ARGS__) : (void)0
#define loge(...) g_log & LOG_ERROR ? printf(__VA_ARGS__) : (void)0
#endif