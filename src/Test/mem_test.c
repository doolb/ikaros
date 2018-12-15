#include "test.h"
#include "System.h"

test_start (mem) {
	mem_init ();

	char* buf = mmalloc (256);
	test_func ("alloc mem %d", buf, 256);

	snprintf (buf, 256, "this is a test %d", 123);
	test_func ("realloc mem %d", mralloc (&buf, 512), 512);

	mfree (buf);

	mem_clear ();
}
test_end ()

