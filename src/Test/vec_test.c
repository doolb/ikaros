#include "test.h"
#include "vec.h"

test_start(vec){
	vec v = Zero;
	vec v1 = Zero;
	v.r = v.g = v.a = 1.0f;
	v1.rgba = v.rgba;
	test_func ((v1.v [0] == 1 && v1.v [1] == 1 && v1.v [3] == 1), "Test rgb / a");

	vec3i ia = Zero;
	vec3i ib = Zero;
	ia.x = 1; ia.y = 2; ia.z = 3;
	ib.xy = ia.xy; ib.z = 2;
	test_func ((ia.v [0] == 1 && ia.v [1] == 2 && ib.v [2] == 2), "Test vec3i ");

	zerov (&v); onev (&v); mulv (&v, 3);
	test_func ((v.v [0] == 3 && v.v [1] == 3 && v.v [2] == 3), "Test one zeor mul");

	addv (&v, 1, &ia);
	test_func ((v.v [0] == 4 && v.v [1] == 5 && v.v [2] == 6), "Test add");

	subv (&v, 1, &v1);
	test_func ((v.v [0] == 3 && v.v [1] == 4 && v.v [2] == 6), "Test sub");
}
test_end()