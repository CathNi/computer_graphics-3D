#include <math.h>
#pragma once

class Vector3D
{
public: double x, y, z;
public:

	Vector3D()
	{
		x = y = z = 0;
	}

	Vector3D(double X, double Y, double Z)
	{
		x = X; y = Y; z = Z;
	}

	Vector3D(Vector3D&vec)
	{
		x = vec.x; y = vec.y; z = vec.z;
	}

	Vector3D& operator=(const Vector3D&v)
	{
		x = v.x; y = v.y; z = v.z;
		return *this;
	}

	double operator*(const Vector3D&v)
	{
		return(v.x * x + v.y * y + v.z * z);
	}


	Vector3D operator*(double k)
	{
		x *= k; y *= k; z *= k;
		return *this;
	}

	Vector3D Normalize()
	{
		double norm = sqrt(x*x + y*y + z*z);
		x /= norm;
		y /= norm;
		z /= norm;
		return *this;
	}
	
	Vector3D VectorProduct(Vector3D a, Vector3D b)
	{
		//переопределить векторное произведение
		Vector3D res(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
		return res;
	}

	

	~Vector3D()
	{
	}
};

