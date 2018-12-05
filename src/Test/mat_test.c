#include "test.h"

#include "Math.h"

test_start(mat){
	mat4 mat = Zero;
	mat3 m3 = Zero;

	mat.m [0] [0] = 1; mat.M [1 * 4 + 1] = 1; mat.M [M4 (2, 2)] = 1; mat.m [3] [3] = 1;
	test_func ((mat.m_11 == 1 && mat.m_22 == 1 && mat.m_33 == 1 && mat.m_44 == 1), "Test memory align.");

	m3.M [M3 (0, 2)] = 2; m3.m [1] [1] = 1;
	test_func ((m3.m_22 == 1 && m3.m_13 == 2), "Test mat 3x3 memory align.");
}
test_end()