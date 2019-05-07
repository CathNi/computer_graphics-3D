#include "Matrix.h"
#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

inline Matrix Translation(Matrix::Cell X, Matrix::Cell Y, Matrix::Cell Z)
{
	Matrix::Cell matr[16] =
	{ 1.0, 0.0, 0.0, X, 
	  0.0, 1.0, 0.0, Y, 
	  0.0, 0.0, 1.0, Z,
	  0.0, 0.0, 0.0, 1.0};
	Matrix res(4, 4, matr);

	return res;
} 

inline Matrix Rotation(double phi, int flag)
{
	switch (flag)
	{
	//oz
	case 1: {
		Matrix::Cell matr[16] = { 
			cos(phi), (-1.0)*sin(phi), 0, 0,
			sin(phi),       cos(phi) , 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };
		Matrix res(4, 4, matr);
		return res;
	}
	//oy
	case 2:
	{
		Matrix::Cell matr[16] = { 
			cos(phi), 0, sin(phi), 0,
			0, 1, 0, 0,
			-sin(phi), 0, cos(phi), 0,
			0, 0, 0, 1 };
		Matrix res(4, 4, matr);
		return res;
	}
	//ox
	case 3:
	{
		
		Matrix::Cell matr[16] = { 
			1, 0, 0, 0,
			0, cos(phi), -sin(phi), 0,
			0, sin(phi), cos(phi), 0,
			0, 0, 0, 1 };
		Matrix res(4, 4, matr);
		return res;
	}
	}
	
}


inline Matrix Scaling(double kx, double ky, double kz)
{
	Matrix::Cell matr[16] = { 
		kx, 0, 0, 0,
		0, ky, 0, 0, 
		0, 0, kz, 0,
		0, 0, 0, 1};
	Matrix res(4, 4, matr);

	return res;
}

inline Matrix Reflection(
	int flag//0 - OO, 1 - OX, 2 - OY
	)
{
	switch (flag)
	{
	case 0: return Scaling(-1.0, -1.0, -1.0); break;//0
	case 1: return Scaling(1.0, -1.0, -1.0); break;// OX
	case 2:	return Scaling(-1.0, 1.0, -1.0); break;// OY
	case 3:	return Scaling(-1.0, -1.0, 1.0); break;// OZ
	case 4: return Scaling(1, 1, -1); // xy
	case 5: return Scaling(-1, 1, 1); //yz
	case 6: return Scaling(1, -1, 1); //xz
	default: return Scaling(1, 1, 1);
	}
	
}

void My_Transform(Matrix &matr_transform, double x, double y, double z)
{

	double p, dx, dz;
	
	dx = matr_transform.angle * cos(matr_transform.angle);
	dz = matr_transform.angle * sin(matr_transform.angle);

	matr_transform.angle += 0.01;

	dx = matr_transform.angle * cos(matr_transform.angle) - dx;
	dz = matr_transform.angle * sin(matr_transform.angle) - dz;

	matr_transform =
		Translation(x, y, z)*
		Rotation(0.05, 2)*Translation(dx, 0.01, dz)
		*Translation(-x, -y, -z);
}


inline Matrix Rotation(double A, double B)
{
	double c = A / sqrt(A*A + B*B);
	double s = B / sqrt(A*A + B*B);


	Matrix::Cell matr[16] = {
		c, 0, s, 0,
		0, 1, 0, 0,
		-s, 0, c, 0,
		0, 0, 0, 1 };
	Matrix res(4, 4, matr);
	return res;
}

#endif AFFINE_TRANSFORM_H
