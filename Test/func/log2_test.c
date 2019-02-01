#include "test.h"
#include "log2.h"

test_start (log2) {
	printf ("is_pow_2: %d, %s\n", 3, is_pow_2 (3) != 0 ? "true" : "false");
	printf ("is_pow_2: %d, %s\n", 4, is_pow_2 (4) != 0 ? "true" : "false");
	printf ("is_pow_2: %d, %s\n", 1515, is_pow_2 (1515) != 0 ? "true" : "false");
	printf ("is_pow_2: %d, %s\n", 20363, is_pow_2 (20363) != 0 ? "true" : "false");
	printf ("is_pow_2: %d, %s\n", 394340, is_pow_2 (394340) != 0 ? "true" : "false");


	printf ("log2: %d, %d\n", 394340, ilog2 (394340));
	printf ("log2: %d, %d\n", 1134, ilog2 (1134));
	printf ("log2: %d, %d\n", 2342, ilog2 (2342));
	printf ("log2: %d, %d\n", 134324, ilog2 (134324));
	printf ("log2: %d, %d\n", 23542, ilog2 (23542));

	test_func ("pow 2 0xff: %d, %llu", pow2_ff (2) == 3, 2, pow2_ff (2));
	test_func ("pow 2 0xff: %d, %llu", pow2_ff (4) == 15, 4, pow2_ff (4));
	test_func ("pow 2 0xff: %d, %llu", pow2_ff (9) == 511, 9, pow2_ff (9));
	test_func ("pow 2 0xff: %d, %llu", pow2_ff (12) == 4095, 12, pow2_ff (12));
	test_func ("pow 2 0xff: %d, %llu", pow2_ff (22) == 4194303, 22, pow2_ff (22));
	test_func ("pow 2 0xff: %d, %llu", pow2_ff (33) == 8589934591, 33, pow2_ff (33));
	test_func ("pow 2 0xff: %d, %llu", pow2_ff (63) == 9223372036854775807, 63, pow2_ff (63));
}
test_end ();