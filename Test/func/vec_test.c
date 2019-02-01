#include "test.h"
#include "vec.h"

test_start (vec) {
	vec v = null;
	vec v1 = null;
	v.r = v.g = v.a = 1.0f;
	v1.rgba = v.rgba;
	test_func ("Test rgb / a", (v1.v [0] == 1 && v1.v [1] == 1 && v1.v [3] == 1));

	vec3i ia = null;
	vec3i ib = null;
	ia.x = 1; ia.y = 2; ia.z = 3;
	ib.xy = ia.xy; ib.z = 2;
	test_func ("Test vec3i ", (ia.v [0] == 1 && ia.v [1] == 2 && ib.v [2] == 2));

	zerov (&v); onev (&v); mulv (&v, 3);
	test_func ("Test one zeor mul", (v.v [0] == 3 && v.v [1] == 3 && v.v [2] == 3));

	addv (&v, 1, &ia);
	test_func ("Test add", (v.v [0] == 4 && v.v [1] == 5 && v.v [2] == 6));

	subv (&v, 1, &v1);
	test_func ("Test sub", (v.v [0] == 3 && v.v [1] == 4 && v.v [2] == 6));
}
test_end ()