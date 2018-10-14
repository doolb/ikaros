#pragma once
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <assert.h>

#include "../config.h"


#define TEST __declspec( dllexport )

typedef int(*test_fun)();

#define test_start()	printf("Run %s..\n", __func__)
#define test_ok()		return 0;

#define test_func(desc,block)				\
	printf("[..] " ## desc ## "\r");		\
	block									\
	printf("[ok]\n");

#define test_func_d(desc,block)				\
	printf("[..] " ## desc ## "\r");		\
	block									\
	Sleep(1000); printf("[ok]\n");
