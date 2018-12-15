#ifndef _Math_h_
#define _Math_h_
//#include <math.h>
#include "config.h"

#define M3(l,c) (l * 3 + c)
#define M4(l,c) (l * 4 + c)

typedef struct {
	union
	{
		mfloat M [16];
		mfloat m [4] [4];
		struct
		{
			mfloat m_11, m_12, m_13, m_14;
			mfloat m_21, m_22, m_23, m_24;
			mfloat m_31, m_32, m_33, m_34;
			mfloat m_41, m_42, m_43, m_44;
		};
		struct {
			struct { mfloat v [4]; } M_1, M_2, M_3, M_4;
		};
	};
} mat4, mat;

typedef struct {
	union
	{
		mfloat M [9];
		mfloat m [3] [3];
		struct
		{
			mfloat m_11, m_12, m_13;
			mfloat m_21, m_22, m_23;
			mfloat m_31, m_32, m_33;
		};
		struct {
			struct { mfloat v [4]; } M_1, M_2, M_3;
		};
	};
} mat3;

#define matOne(ptr)		((ptr)->m_11 = 1.0, (ptr)->m_22 = 1.0, (ptr)->m_33 = 1.0, (ptr)->m_44 = 1.0)
#define matOne3(ptr)	((ptr)->m_11 = 1.0, (ptr)->m_22 = 1.0, (ptr)->m_33 = 1.0)
#define matZero(ptr)	((ptr)->m_11 = 0, (ptr)->m_22 = 0, (ptr)->m_33 = 0, (ptr)->m_44 = 0)
#define matZero3(ptr)	((ptr)->m_11 = 0, (ptr)->m_22 = 0, (ptr)->m_33 = 0)
#endif