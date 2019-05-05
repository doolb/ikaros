#ifndef SHADER_H
#define SHADER_H
#include <string.h>

#include "config.h"
#include "System.h"
#include "../list.h"

#define Shader_name_max 64

typedef enum _ShaderProperty_Type {
	ShaderProperty_2D,
	ShaderProperty_vec4,
	ShaderProperty_vec3,
	ShaderProperty_vec2,
}ShaderProperty_Type;

typedef struct _ShaderProperty {
	char name[Shader_name_max];
	ShaderProperty_Type type;
	int id;
	Def_ListHead
}ShaderProperty, *pShaderProperty;

typedef struct _Shader {
	// name and id
	int		id;
	char	name[Shader_name_max];
	GLenum	cullType;
	// depth
	bool	isZWrite;
	GLenum	zTest;
	// blend
	GLenum	blendSrc, blendDst;
	// fallback
	struct _Shader *fallback;
	ListHead properties;
}Shader, *pShader;



static int g_shader_id;
static GLenum g_gl_err;
#define setShaderAttrib(shader_id$int, attrib, cb_id$int) \
	do {				\
		g_shader_id = glGetAttribLocation(shader_id$int, attrib);	\
		if (g_shader_id > -1) {									\
			cb_id$int;	\
			g_gl_err = glGetError();	\
			if(g_gl_err)				\
				printf("%x\n", err);	\
			glEnableVertexAttribArray(g_shader_id);	\
		}		\
		else	\
			logw(attrib"no found in shader!\n");	\
	} while (0);

static inline void initShader_gl(pShader shader) {
	shader->id = -1;
	shader->name[0] = 0;

	shader->cullType = GL_BACK; // Don¡¯t render polygons facing away from the viewer
	shader->isZWrite = true;	// written to the depth buffer
	shader->zTest = GL_LEQUAL;	// draw objects in from or at the distance as existing objects; hide objects behind them
	shader->blendSrc = disable; // Turn off blending
	shader->blendDst = disable;

	shader->fallback = NULL;
	_listInit(&shader->properties);
}

static inline pShader newShader_gl() {
	pShader shader = mmalloc(sizeof(Shader));
	initShader_gl(shader);
	return shader;
}

static inline void delShader(pShader *p) {
	if (p) {
		pShaderProperty prop = NULL;
		List_for_each_entry(ShaderProperty, prop, &(*p)->properties)
			mfree(prop);
		mfree(p);
		p = NULL;
	}
}
static inline bool loadShader_gl(pShader shader, const string vert_src, const string frag_src) {
	// vertex
	char log[512];
	int succ;
	int vtx = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vtx, 1, &vert_src, NULL);
	glCompileShader(vtx);
	glGetShaderiv(vtx, GL_COMPILE_STATUS, &succ);
	if (!succ) {
		glGetShaderInfoLog(vtx, sizeof(log), NULL, log);
		puts(log);
		return false;
	}
	// fragment
	int frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &frag_src, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &succ);
	if (!succ) {
		glGetShaderInfoLog(frag, sizeof(log), NULL, log);
		puts(log);
		return false;
	}
	// program
	int prog = glCreateProgram();
	glAttachShader(prog, vtx);
	glAttachShader(prog, frag);
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &succ);
	if (!succ) {
		glGetProgramInfoLog(prog, sizeof(log), NULL, log);
		puts(log);
		return false;
	}
	glDeleteShader(vtx);
	glDeleteShader(frag);
	shader->id = prog;

	// uniform
	pShaderProperty p = NULL;
	List_for_each_entry(ShaderProperty, p, &shader->properties) {
		p->id = glGetUniformLocation(prog, p->name);
		if (p->id == -1) {
			loge("find unifrom id failed : %s.\n", p->name);
			return false;
		}
	}
	return true;
}
#if !NO_SHADER_UNITY
static inline bool loadShader_unity_pass(pShader shader, string data, int data_len) {
	int i = 0, l = 0, c = 0, s = 0, e = 0;
	assert(parseEmpty(data + s, data_len, &i, &l, &c) != EOF);
	assert(parseWord(data + s, data_len, &i, &l, &c, &s, &e) != EOF);
	assert(strncmp(data + s, "GLSLPROGRAM", sizeof("GLSLPROGRAM") - 1) == 0);
	char *ptr = strrnstr(data + s, data_len - s + 1, "ENDGLSL", sizeof("ENDGLSL") - 1);
	assert(ptr != NULL);
	// copy source
	int endglsl = ptr - data - i;
	char* mem = mmalloc(endglsl);
	assert(mem);
	strncpy(mem, data + i, endglsl);

	// vertex
	char* vert = strstr(mem, "#ifdef VERTEX");
	assert(vert);
	char* vert_end = strstr(vert, "#endif");
	assert(vert_end);
	char* frag = strstr(vert_end, "#ifdef FRAGMENT");
	assert(frag);
	char* frag_end = strstr(frag, "#endif");
	assert(frag_end);
	vert_end[0] = 0;
	frag_end[0] = 0;
	return loadShader_gl(shader, vert + sizeof("#ifdef VERTEX") - 1, frag + sizeof("#ifdef FRAGMENT") - 1);
}
static inline bool loadShader_unity_subshader(pShader shader, string data, int data_len) {
	int i = 0, l = 0, c = 0, s = 0, e = 0;
	// pass
	assert(parseEmpty(data, data_len, &i, &l, &c) != EOF);
	assert(parseWord(data, data_len, &i, &l, &c, &s, &e) != EOF);
	assert(strncmp(data + s, "Pass", sizeof("Pass") - 1) == 0);
	assert(parseSymbol(data, data_len, &i, &l, &c, &s, &e, '{', '}') != EOF);
	if (!loadShader_unity_pass(shader, data + s, e - s + 1))
		return false;
	return true;
}
static inline bool loadShader_unity_properties_type(pShaderProperty prop, string data, int data_len) {
	int i = 0, l = 0, c = 0, s = 0, e = 0;
	assert(parseEmpty(data, data_len, &i, &l, &c) != EOF);
	assert(parseCfgString(data, data_len, &i, &l, &c, &s, &e) != EOF);
	assert(parseWord(data, data_len, &i, &l, &c, &s, &e) != EOF);
	if(strncmp(data + s , "2D", sizeof("2D") -1 ) == 0)
		prop->type = ShaderProperty_2D;
	else if (strncmp(data + s, "vec4", sizeof("vec4") - 1) == 0)
		prop->type = ShaderProperty_vec4;
	else
		loge("Unkown shader properties type : %s.\n", prop->name);
	return true;
}
static inline bool loadShader_unity_properties(pShader shader, string data, int data_len) {
	int i = 0, l = 0, c = 0, s = 0, e = 0;
	while (i < data_len) {
		if(parseEmpty(data, data_len, &i, &l, &c) == EOF)
			break;
		assert(parseWord(data, data_len, &i, &l, &c, &s, &e) != EOF);
		assert(e - s <= Shader_name_max);
		pShaderProperty p = mmalloc(sizeof(pShaderProperty));
		assert(p);
		listInit(p);
		strncpy(p->name, data + s, e - s + 1);

		assert(parseEmpty(data, data_len, &i, &l, &c) != EOF);
		assert(parseSymbol(data, data_len, &i, &l, &c, &s, &e, '(', ')') != EOF);
		if (!loadShader_unity_properties_type(p, data + s, e - s + 1)) {
			mfree(p);
			return false;
		}
		listAdd(p, &shader->properties);
	}
	return true;
}
static inline bool loadShader_unity_shader(pShader shader, string data, int data_len) {
	int i = 0, l = 0, c = 0, s = 0, e = 0;
	// properties
	assert(parseEmpty(data, data_len, &i, &l, &c) != EOF);
	assert(parseWord(data, data_len, &i, &l, &c, &s, &e) != EOF);
	assert(strncmp(data + s, "Properties", sizeof("Properties") - 1) == 0);
	assert(parseSymbol(data, data_len, &i, &l, &c, &s, &e, '{', '}') != EOF);
	if(!loadShader_unity_properties(shader, data + s, e - s + 1))
		return false;

	// subshader
	assert(parseEmpty(data, data_len, &i, &l, &c) != EOF);
	assert(parseWord(data, data_len, &i, &l, &c, &s, &e) != EOF);
	assert(strncmp(data + s, "SubShader", sizeof("SubShader") - 1) == 0);
	assert(parseSymbol(data, data_len, &i, &l, &c, &s, &e, '{', '}') != EOF);
	return loadShader_unity_subshader(shader, data + s, e - s + 1);
}
static inline bool loadShader_unity(pShader shader, string data, int data_len) {
	// header
	int i = 0, l = 0, c = 0, s = 0, e = 0;
	assert(parseEmpty(data, data_len, &i, &l, &c) != EOF);
	assert(parseWord(data, data_len,&i,&l,&c,&s,&e) != EOF);
	assert(strncmp(data + s, "Shader", sizeof("Shader")-1) == 0);
	// name
	assert(parseEmpty(data, data_len, &i, &l, &c) != EOF);
	assert(parseCfgString(data, data_len, &i, &l, &c,&s,&e) != EOF);
	assert(e - s <= Shader_name_max);
	strncpy(shader->name, data + s, e - s + 1);
	// properties
	assert(parseEmpty(data, data_len, &i, &l, &c) != EOF);
	assert(parseSymbol(data, data_len, &i, &l, &c, &s, &e, '{', '}') != EOF);
	return loadShader_unity_shader(shader, data + s, e - s + 1);
}
#endif

static inline bool loadShader(pShader shader, string filename) {
	size_t size;
	string buff = map_file(filename, &size);
	if (!buff) {
		loge("read shader file failed! : %s\n", filename);
		return false;
	}
	bool succ = false;
#if !NO_SHADER_UNITY
	if (strstr(filename, ".shader"))
		succ = loadShader_unity(shader, buff, size);
	else
#endif
		loge("unsupport shader! : %s\n", filename);
	if(!succ) loge("loadShader failed %s\n", filename);
	return succ;
}

_Func(Shader) {
	IDispose(Shader);
	IParseFile(Shader);
}_(Shader) = {
	IDispose_Def(newShader_gl, delShader,initShader_gl),
	loadShader,
};

static inline int shaderLocation(pShader shader, string name, int name_len) {
	assert(shader);
	if (isListEmpty(&shader->properties)) return -1;
	pShaderProperty ptr;
	List_for_each_entry(ShaderProperty, ptr, &shader->properties)
		if (strncmp(ptr->name, name, name_len) == 0)
			return ptr->id;
	return -1;
}
#endif