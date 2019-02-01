#pragma once
#ifndef _vec_h_
#define _vec_h_
#include <math.h>
#include "config.h"

#define _V(n)		struct { mfloat _v [n]; }		// define a n vector struct
#define _Vi(n)		struct { mint	_v [n]; }		// define a n vector struct

typedef struct {
	union {
		mfloat v [4];
		struct { mfloat x, y, z, w; };
		struct { mfloat r, g, b, a; };
		struct { mfloat lb, lg, lr, la; };
		_V (4) xyzw;
		_V (4) rgba;
		_V (3) xyz;
		_V (3) rgb;
		struct { _V (2) xy, zw; };
		struct { _V (2) rg, ba; };
	};
} vec4f, vec4, vec;

typedef struct {
	union {
		mfloat v [3];
		struct { mfloat x, y, z; };
		struct { mfloat r, g, b; };
		struct { mfloat lb, lg, lr; };
		_V (3) xyz;
		_V (3) rgb;
		_V (2) xy;
		_V (2) rg;
	};
} vec3f, vec3;

typedef struct {
	union {
		mfloat v [2];
		mfloat x, y;
		_V (2) xy;
	};
} vec2f, vec2;

typedef struct {
	union {
		mint v [4];
		struct { mint x, y, z, w; };
		struct { mint r, g, b, a; };
		struct { mint lb, lg, lr, la; };
		_Vi (4) xyzw;
		_Vi (4) rgba;
		_Vi (3) xyz;
		_Vi (3) rgb;
		struct { _Vi (2) xy, zw; };
		struct { _Vi (2) rg, ba; };
	};
} vec4i;

typedef struct {
	union {
		mint v [3];
		struct { mint x, y, z; };
		struct { mint r, g, b; };
		struct { mint lb, lg, lr; };
		_Vi (3) xyz;
		_Vi (3) rgb;
		_Vi (2) xy;
		_Vi (2) rg;
	};
} vec3i;

typedef struct {
	union {
		mint v [2];
		mint x, y;
		_Vi (2) xy;
	};
} vec2i;

#define onev(ptr)		((ptr)->x = 1.0,	(ptr)->y = 1.0,		(ptr)->z = 1.0)
#define one2v(ptr)		((ptr)->x = 1.0,	(ptr)->y = 1.0)
#define zerov(ptr)		((ptr)->x = 0,		(ptr)->y = 0,		(ptr)->z = 0)
#define zero2v(ptr)		((ptr)->x = 0,		(ptr)->y = 0)

#define mulv(ptr, n)	((ptr)->x *= n,		(ptr)->y *= n,		(ptr)->z *= n)	// a = n*a 
#define mul2v(ptr,n)	((ptr)->x *= n,		(ptr)->y *= n)						// a = n*a
#define divv(ptr, n)	((ptr)->x /= n,		(ptr)->y /= n,		(ptr)->z /= n)	// a = a/n
#define div2v(ptr,n)	((ptr)->x /= n,		(ptr)->y /= n)						// a = a/n

#define addv(a, n, b)	((a)->x += n*((b)->x),	(a)->y += n*((b)->y),	(a)->z += n*((b)->z))	// a = a + n*b
#define add2v(a,n, b)	((a)->x += n*((b)->x),	(a)->y += n*((b)->y))							// a = a + n*b
#define subv(a, n, b)	((a)->x += -n*((b)->x),	(a)->y += -n*((b)->y),	(a)->z += -n*((b)->z))	// a = a - n*b
#define sub2v(a,n, b)	((a)->x += -n*((b)->x),	(a)->y += -n*((b)->y))							// a = a - n*b
#endif