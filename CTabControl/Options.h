#pragma once
#include "stdafx.h"

struct DirectoryOpt {
	CString keywordFile;
	CString logDir;
};

struct CutPlaneOpt {
	float X = 0;
	float Y = 0;
	float Z = 0;
	BOOL bCutByX = true;
	BOOL bCutByY = true;
	BOOL bCutByZ = true;
};

template <class Type>
struct Vector3 {
	Type vec[3];
	Vector3<Type>(const Type& v0 = Type(0.), const Type& v1 = Type(0.), const Type& v2 = Type(0.))
	{
		vec[0] = v0;
		vec[1] = v1;
		vec[2] = v2;
	}
	Vector3<Type>& SetValue(Type a_x, Type a_y, Type a_z)
	{
		vec[0] = a_x;
		vec[1] = a_y;
		vec[2] = a_z;
		return *this;
	}
	Vector3<Type>& operator=(const Vector3<Type>& src)
	{
		vec[0] = src.vec[0];
		vec[1] = src.vec[1];
		vec[2] = src.vec[2];
		return *this;
	}
	Vector3<Type>& operator+=(const Vector3<Type>& src)
	{
		vec[0] += src.vec[0];
		vec[1] += src.vec[1];
		vec[2] += src.vec[2];
		return *this;
	}
	Vector3<Type>& operator-=(const Vector3<Type>& src)
	{
		vec[0] -= src.vec[0];
		vec[1] -= src.vec[1];
		vec[2] -= src.vec[2];
		return *this;
	}
	Vector3<Type>& operator*=(Type src)
	{
		vec[0] *= src;
		vec[1] *= src;
		vec[2] *= src;
		return *this;
	}
	Vector3<Type> operator+(const Vector3<Type>& src)
	{
		Vector3<Type> tmp = *this;
		tmp += src;
		return tmp;
	}

	Vector3<Type> operator-(const Vector3<Type>& src)
	{
		Vector3<Type> tmp = *this;
		tmp -= src;
		return tmp;
	}
	Vector3<Type> operator*(Type src)
	{
		Vector3<Type> tmp = *this;
		tmp *= src;
		return tmp;
	}
	Vector3<Type> cross(const Vector3<Type>& src)
	{
		return Vector3<Type>(vec[1] * src.vec[2] - src.vec[1] * vec[2],
			vec[2] * src.vec[0] - src.vec[2] * vec[0],
			vec[0] * src.vec[1] - src.vec[0] * vec[1]);
	}

	Type SqrLength() const
	{
		return (vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]);
	}

	Type Length() const
	{
		return (Type)sqrt(SqrLength());
	}

	Type Normalize()
	{
		Type magnitude = Length();

		if (magnitude != 0.0)
			(*this) *= (Type)(1.0 / magnitude);
		else SetValue(0.0, 0.0, 0.0);

		return magnitude;
	}

};

typedef Vector3<long>			Vector3l;
typedef Vector3<float>			Vector3f;
typedef Vector3<double>			Vector3d;