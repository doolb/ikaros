#ifndef MESH_H
#define MESH_H
#include <math.h>
#include "System.h"
#include <float.h>
#include "tinyobj.h"

typedef struct _Mesh {
	uint id;
	uint numTriangles;
	uint vertices, indices;
	float bmin[3], bmax[3], maxExtent;
}Mesh, *pMesh;

struct mesh_obj {
	vec3f vertex, normal;
	vec2f uv;
};

#define FACE_VERTEX_NUM		3
#define VERTEX_COORD_NUM	3
#define TEXTURE_COORD_NUM	2

static inline int LoadObjAndConvert(pMesh ms, const char* filename) {
	tinyobj_attrib_t attrib;
	tinyobj_shape_t* shapes = NULL;
	size_t num_shapes;
	tinyobj_material_t* materials = NULL;
	size_t num_materials;

	size_t data_len = 0;
	const char* data = get_file_data(&data_len, filename);
	if (data == NULL) {
		exit(-1);
		/* return 0; */
	}
	printf("filesize: %d\n", (int)data_len);

	{
		unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
		int ret = tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
			&num_materials, data, data_len, flags);
		if (ret != TINYOBJ_SUCCESS) {
			return 0;
		}

		printf("# of shapes    = %d\n", (int)num_shapes);
		printf("# of materials = %d\n", (int)num_materials);

		/*
		{
		  int i;
		  for (i = 0; i < num_shapes; i++) {
			printf("shape[%d] name = %s\n", i, shapes[i].name);
		  }
		}
		*/
	}
	ms->bmin[0] = ms->bmin[1] = ms->bmin[2] = FLT_MAX;
	ms->bmax[0] = ms->bmax[1] = ms->bmax[2] = -FLT_MAX;
	float *bmin = ms->bmin;
	float *bmax = ms->bmax;

	struct mesh_obj *vb = malloc(sizeof(struct mesh_obj) * attrib.num_vertices);
	assert(vb);
	vec3i *vf = malloc(sizeof(vec3i) * attrib.num_face_num_verts + 1);
	assert(vf);

	// each face
	for (uint f = 0; f < attrib.num_face_num_verts; f++) {
		// each point
		assert(attrib.face_num_verts[f] == FACE_VERTEX_NUM);
		for (int v = 0; v < FACE_VERTEX_NUM; v++) {
			tinyobj_vertex_index_t idx = attrib.faces[FACE_VERTEX_NUM * f + v];
			int vidx = idx.v_idx;
			vf[f].v[v] = vidx; // store indices

			// each coordinate
			for (int i = 0; i < VERTEX_COORD_NUM; i++) { // xyz
				vb[vidx].vertex.v[i] = attrib.vertices[VERTEX_COORD_NUM * vidx + i];

				// max extern
				ms->bmin[i] = (vb[vidx].vertex.v[i] < ms->bmin[i]) ? vb[vidx].vertex.v[i] : ms->bmin[i];
				ms->bmax[i] = (vb[vidx].vertex.v[i] > ms->bmax[i]) ? vb[vidx].vertex.v[i] : ms->bmax[i];
			}

			// texcoord
			for (int i = 0; i < TEXTURE_COORD_NUM; i++) {
				vb[idx.v_idx].uv.v[i] = attrib.texcoords[TEXTURE_COORD_NUM * idx.vt_idx + i];
			}
		}
		tinyobj_vertex_index_t idx0 = attrib.faces[3 * f + 0];
		tinyobj_vertex_index_t idx1 = attrib.faces[3 * f + 1];
		tinyobj_vertex_index_t idx2 = attrib.faces[3 * f + 2];

		// normal
		if (attrib.num_normals > 0 &&
			idx0.vn_idx >= 0 && idx1.vn_idx >= 0 && idx2.vn_idx >= 0) {
			assert(idx0.vn_idx < attrib.num_normals);
			assert(idx1.vn_idx < attrib.num_normals);
			assert(idx2.vn_idx < attrib.num_normals);
			for (int i = 0; i < 3; i++) { // xyz
				vb[idx0.v_idx].normal.v[i] = attrib.normals[3 * idx0.vn_idx + i]; // point a
				vb[idx1.v_idx].normal.v[i] = attrib.normals[3 * idx1.vn_idx + i]; // point b
				vb[idx2.v_idx].normal.v[i] = attrib.normals[3 * idx2.vn_idx + i]; // point c
			}
		}
		else {
			CalcNormal(vb[idx0.v_idx].normal.v, vb[idx0.v_idx].vertex.v, vb[idx1.v_idx].vertex.v, vb[idx2.v_idx].vertex.v);
			vb[idx1.v_idx].normal = vb[idx0.v_idx].normal;
			vb[idx2.v_idx].normal = vb[idx0.v_idx].normal;
		}
	}

	ms->maxExtent = 0.5f * (ms->bmax[0] - ms->bmin[0]);
	if (ms->maxExtent < 0.5f * (ms->bmax[1] - ms->bmin[1]))
		ms->maxExtent = 0.5f * (ms->bmax[1] - ms->bmin[1]);
	if (ms->maxExtent < 0.5f * (ms->bmax[2] - ms->bmin[2]))
		ms->maxExtent = 0.5f * (ms->bmax[2] - ms->bmin[2]);

	ms->numTriangles = attrib.num_faces; // all face indices count
	glGenVertexArrays(1, &ms->id);
	glBindVertexArray(ms->id);

	glGenBuffers(1, &ms->vertices);
	glBindBuffer(GL_ARRAY_BUFFER, ms->vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct mesh_obj) * attrib.num_vertices, vb, GL_STATIC_DRAW);

	glGenBuffers(1, &ms->indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ms->indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vec3i) * attrib.num_face_num_verts, vf, GL_STATIC_DRAW);
	GLenum err = glGetError();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct mesh_obj), NULL);
	glEnableVertexAttribArray(0);
	err = glGetError();

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct mesh_obj), sizeof(vec3f));
	glEnableVertexAttribArray(1);
	err = glGetError();

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct mesh_obj), sizeof(vec3f) + sizeof(vec3f));
	glEnableVertexAttribArray(2);
	err = glGetError();
	glBindVertexArray(0);

	free(vf);
	free(vb);

	printf("bmin = %f, %f, %f\n", (double)ms->bmin[0], (double)ms->bmin[1], (double)ms->bmin[2]);
	printf("bmax = %f, %f, %f\n", (double)ms->bmax[0], (double)ms->bmax[1], (double)ms->bmax[2]);

	tinyobj_attrib_free(&attrib);
	tinyobj_shapes_free(shapes, num_shapes);
	tinyobj_materials_free(materials, num_materials);

	return 1;
}

static pMesh loadMesh(string filename) {
	pMesh ms = mmalloc(sizeof(Mesh));
	if (0 == LoadObjAndConvert(ms, filename)) {
		loge("failed to load & conv\n");
		mfree(ms);
		return NULL;
	}
	return ms;
}

_Func(Mesh) {
	ILoad(Mesh);
}_(Mesh) = {
	loadMesh
};
#endif