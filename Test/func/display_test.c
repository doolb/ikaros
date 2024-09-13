#include "test.h"
#include "config.h"

#include "platform/display.h"

test_start(display)
{
	Display display = null;
	test_func("get screen", ADisplay.getScreen(&display) == 0);
	vec4i rect;
	ADisplay.getRect(&display, &rect);
	printf("width %ld, height %ld", rect.width, rect.height);
	ADisplay.release(&display);
}
test_end()