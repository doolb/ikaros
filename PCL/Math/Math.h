#pragma once
#include <math.h>
#include "../config.h"

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
	};
} mat3;