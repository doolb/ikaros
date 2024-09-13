/// @file timer.h
/// @copyright BSD 2-clause. See LICENSE.txt for the complete license text.
/// @author Dane Larsen
/// @brief Two functions for profiling function call time.

#ifndef TIMER_H
#define TIMER_H

#include <time.h>

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

#endif
