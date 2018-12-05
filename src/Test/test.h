#pragma once
//#include <curses.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "System.h"


typedef void(*test_fun)();

#define test_start(name) API void name##_test(){printf("Run %s..\n", __func__);
#define test_end()		return;}

#if __GNUC__
#define test_func(block,desc,arg...)		\
	printf("[..] "desc"\r",##arg);			\
	assert(block)	;						\
	printf("[ok]\n");
#else
#define test_func(block,desc,...)	\
	printf("[..] "desc"\r",__VA_ARGS__);			\
	assert(block)	;						\
	printf("[ok]\n");
#endif