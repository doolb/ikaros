#ifndef _config_h_
#define _config_h_
#define mfloat	float
#define mint	int
#define Zero	{ 0 }
#define null	(void*)0

#if __GNUC__
#define API __attribute__((visibility("default")))
#else
#define API __declspec( dllexport )
#endif

typedef unsigned long long 		ptr;
#endif