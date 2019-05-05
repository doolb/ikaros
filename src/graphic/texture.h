#ifndef TEXTURE_H
#define TEXTURE_H
#include "config.h"
#include "System.h"
#include "hashtable.h"

typedef struct _Texture {
	char name[MAX_PATH];
	uint id;
	Def_HashList;
}Texture, *pTexture;

static Def_HashTable(textureDic, 4);

static inline int loadTexture_gl(pTexture tex, string filename) {
	uint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// texture parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image
	int width, height, channel;
	stbi_set_flip_vertically_on_load(true);
	byte *data = stbi_load(filename, &width, &height, &channel, 0);

	// setup texture data
	glActiveTexture(GL_TEXTURE0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	GLenum err = glGetError(); 
	if (err) {
		loge("load image to texture : %s %x.\n", filename, err);
	}
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	strncpy(tex->name, filename, MAX_PATH);
	tex->id = id;
	return id;
}

static inline pTexture loadTexture(string filename) {
	assert(filename);
	int max_len = strnlen(filename, MAX_PATH);
	ulong hash = hash_str_bit(filename, max_len, Hash_Bits(textureDic));
	pTexture p = NULL;
	Hlist_for_each_entry(Texture, p, &textureDic[hash]) {
		if (strncmp(p->name, filename, max_len) == 0) {
			return p;
		}
 	}
	
	p = mmalloc(sizeof(Texture));
	assert(p);
	hlistInit(p);
	loadTexture_gl(p, filename);
	return p;
}

_Func(Texture) {
	ILoad(Texture);
}_(Texture) = {
	loadTexture
};
#endif