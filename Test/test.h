#pragma once
//#include <curses.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#if __GNUC__
#define API __attribute__((visibility("default")))
#else
#define API __declspec( dllexport )
#endif

typedef void (*_test_fun)();

#define test_start(name) API void name##_test(){printf("Run %s..\n", __func__);
#define test_end()		return;}

#if __GNUC__
#define test_func(desc,block,arg...)		\
	printf("[..] "desc"\r",##arg);			\
	assert(block)	;						\
	printf("[ok]\n");
#define test_func_l(desc,block,log,arg...)	\
	printf("[..] "desc"\r",##arg);			\
	if(!(block)) log						\
	assert(block)	;						\
	printf("[ok]\n");
#else
#define test_func(desc,block,...)	\
	printf("[..] "desc"\r",__VA_ARGS__);	\
	assert(block)	;						\
	printf("[ok]\n");

#define test_func_l(desc,block, log,...)	\
	printf("[..] "desc"\r",__VA_ARGS__);	\
	if(!(block)) log						\
	assert(block)	;						\
	printf("[ok]\n");
#endif

