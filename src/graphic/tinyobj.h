#ifndef TINYOBJ_H
#define TINYOBJ_H
#include <math.h>
#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "../../3rd/tinyobjloader-c/tinyobj_loader_c.h"
#include "../System.h"

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
	float v10[3];
	float v20[3];
	float len2;

	v10[0] = v1[0] - v0[0];
	v10[1] = v1[1] - v0[1];
	v10[2] = v1[2] - v0[2];

	v20[0] = v2[0] - v0[0];
	v20[1] = v2[1] - v0[1];
	v20[2] = v2[2] - v0[2];

	N[0] = v20[1] * v10[2] - v20[2] * v10[1];
	N[1] = v20[2] * v10[0] - v20[0] * v10[2];
	N[2] = v20[0] * v10[1] - v20[1] * v10[0];

	len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
	if (len2 > 0.0f) {
		float len = (float)sqrt((double)len2);

		N[0] /= len;
		N[1] /= len;
	}
}
static const char* get_file_data(size_t* len, const char* filename) {
	const char* ext = strrchr(filename, '.');

	size_t data_len = 0;
	const char* data = NULL;

	if (strcmp(ext, ".gz") == 0) {
		assert(0); /* todo */

	}
	else {
		data = map_file(filename, &data_len);
	}

	(*len) = data_len;
	return data;
}

#endif