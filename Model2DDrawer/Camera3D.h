#pragma once
#include "Vector3D.h"
#include "Camera2D.h"
#include "Matrix.h"
class Camera3D :
	public Camera2D
{
public:
	Vector3D Ov;
	Vector3D vN;
	Vector3D Top;
	double D;

	Matrix WorldtoView_matr, ViewtoProject_matr, WorldtoProject_matr, Coords;

public:

	Camera3D(double L, double R, double B, double T) :Camera2D(L, R, B, T)
	{
		Ov = Vector3D(0, 0, 0);
		vN = Vector3D(0, 0, 1);
		Top = Vector3D(0, 1, 0);
		D = 10;

		update_camera();
	}

	Matrix WorldtoView(Vector3D&v0)
	{
		v0 = Ov;

		Vector3D kv(vN);
		kv = kv.Normalize();

		Vector3D iv;
		iv = iv.VectorProduct(Top, vN);
		iv = iv.Normalize();

		Vector3D jv;
		jv = jv.VectorProduct(kv, iv);

		Matrix::Cell cells[16] = {
			iv.x, iv.y, iv.z, -(iv*v0),
			jv.x, jv.y, jv.z, -(jv*v0),
			kv.x, kv.y, kv.z, -(kv*v0),
			0, 0, 0, 1
		};

		Matrix::Cell cells_coords[16] = {
			Ov.x, Ov.x + iv.x, Ov.x + jv.x, Ov.x + kv.x,
			Ov.y, Ov.y + iv.y, Ov.y + jv.y, Ov.y + kv.y,
			Ov.z, Ov.z + iv.z, Ov.z + jv.z, Ov.z + kv.z,
			1, 1, 1, 1
		};

		WorldtoView_matr = Matrix(4, 4, cells);
		Coords = Matrix(4, 4, cells_coords);

		fstream out("D:\\WV.txt");
		out << "\tWV\n";
		WorldtoView_matr.print_to_file(out);
				
		out.close();
		return WorldtoView_matr;
	}

	Matrix ViewtoProject()
	{
		Matrix::Cell koeff;

		if (D == NAN)
			koeff = 0;
		else
			koeff = -1 / D;

		Matrix::Cell cells[12] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, koeff, 1
		};
		Matrix matr(3, 4, cells);

		ViewtoProject_matr = matr;


		fstream out("D:\\VP.txt");
		out << "\tVP\n";
		ViewtoProject_matr.print_to_file(out);

		out.close();

		return matr;
	}

	Matrix WorldtoProject(Vector3D&v0)
	{

		WorldtoProject_matr = ViewtoProject() * WorldtoView(v0);
		return WorldtoProject_matr;
	}

	void SetOv(double x, double y, double z)
	{
		Ov =  Vector3D(x, y, z);
		update_camera();
	}

	void SetN(double x, double y, double z)
	{
		vN = Vector3D(x, y, z);
		update_camera();
	}

	void SetT(double x, double y, double z)
	{
		Top = Vector3D(x, y, z);
		update_camera();
	}


	void Setd(double d)
	{
		D = d;
		update_camera();
	}

public:
	bool Deltad(int flag)
	{
		if (D < W/H*5 && flag < 0)
			return false;
		D = (flag > 0) ? D*1.1 : D*0.9;
		update_camera();
		return true;
	}

	void DeltaNorm(int flag)
	{
		double x, y, z;
		x = vN.x; y = vN.y; z = vN.z;
		switch (flag)
		{
		case 1: x-=0.1; break;
		case 2: x+=0.1; break;
		case 3: y+=0.1; break;
		case 4: y-=0.1; break;		
		}
		SetN(x, y, z);
	}

	void update_camera()
	{
		WorldtoProject_matr = WorldtoProject(Ov);

	}

	Matrix WorldToProject_(Matrix matr)
	{

		fstream out("D:\\WP.txt");
		out << "\t Coords \n";
		WorldtoProject_matr.print_to_file(out);

		out.close();

		return WorldtoProject_matr*matr;
	}

	~Camera3D()
	{

	}
};

