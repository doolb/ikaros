#include "test.h"

test_start (size) {
	printf ("size %s: %lu\n", "int", sizeof (int));
	printf ("size %s: %lu\n", "long", sizeof (long));
	printf ("size %s: %lu\n", "long int", sizeof (long int));
	printf ("size %s: %lu\n", "long long", sizeof (long long));
	printf ("size %s: %lu\n", "float", sizeof (float));
	printf ("size %s: %lu\n", "double", sizeof (double));
	printf ("size %s: %lu\n", "long double", sizeof (long double));

	printf ("my define..\n");
	printf ("size %s: %lu\n", "uint", sizeof (uint));
	printf ("size %s: %lu\n", "llong", sizeof (llong));
	printf ("size %s: %lu\n", "ulong", sizeof (ulong));
}
test_end ()