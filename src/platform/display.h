#ifndef DISPLAY_H
#define DISPLAY_H

#include "vec.h"
typedef struct _Display
{
#if _WIN32
#include <Windows.h>
	HWND hwnd;
	HDC hdc;
	HDC memHdc;
#endif
}Display, *pDisplay;



#if _WIN32
int getScreen(pDisplay display)
{
	assert(display);
	display->hwnd = GetDesktopWindow();
	display->hdc = GetWindowDC(display->hwnd);
	display->memHdc = CreateCompatibleDC(display->hdc);
	return 0;
}
int releaseDisplay(pDisplay display)
{
	assert(display);
	assert(display->hwnd);
	if (display->memHdc)
	{
		DeleteDC(display->memHdc);
		display->memHdc = NULL;
	}
	if (display->hdc)
	{
		ReleaseDC(display->hwnd, display->hdc);
		display->hdc = NULL;
	}
	display->hwnd = NULL; // don't need release?
	return 0;
}
int getRect(pDisplay display, vec4i *pos)
{
	assert(display);
	RECT rect;
	GetWindowRect(display->hwnd, &rect);
	pos->posX = rect.left;
	pos->posY = rect.top;
	pos->height = rect.bottom - rect.top;
	pos->width = rect.right - rect.left;
	return 0;
}
#endif




struct _ADisplay
{
	int		(*getScreen)(pDisplay display);
	int		(*release)(pDisplay display);
	int		(*getRect)(pDisplay display, vec4i* pos);
}ADisplay = {
	getScreen,
	releaseDisplay,
	getRect
};

#endif // !DISPLAY_H