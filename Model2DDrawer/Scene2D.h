#ifndef SCENE_2D_H
#define SCENE_2D_H
#include "Matrix.h"
class Scene2D : public Camera3D
{
public:
	Model2D model;
	Model2D Axes;
	
	Scene2D(double L, double R, double B, double T): Camera3D(L, R, B, T)
	{
		double s = sqrt(R*R + T*T);
		Matrix::Cell cells[16] = {
			0, R, 0, 0,
			0, 0, T, 0,
			0, 0, 0, 10,
			1, 1, 1, 1
		};
		Axes.Vertices = Matrix(4, 4, cells);
		Axes.N_Vertices = 4;

		Axes.Edges = new int*[3];
		for (int i = 0; i < 3; i++)
			Axes.Edges[i] = new int[2];

		Axes.Edges[0][0] = 0;
		Axes.Edges[0][1] = 1;

		Axes.Edges[1][0] = 0;
		Axes.Edges[1][1] = 2;

		Axes.Edges[2][0] = 0;
		Axes.Edges[2][1] = 3;

		Axes.N_Edges = 3;
	}

	double HomogeneousToCartesian(double u1, double u3)
	{
		return (double(u1) / double(u3));
	}

	void DrawAxes(HDC dc)
	{
		HPEN pen = CreatePen(PS_DOT, 0, RGB(230, 230, 230));
		SelectObject(dc, pen);

		Matrix c = WorldToProject_(Axes.Vertices);
		
		fstream out("D:\\out_V.txt");
		out << "\t Coords \n";
		c.print_to_file(out);

		out.close();

		for (int i = 0; i < 4; i++)
		{
	
			MoveTo(0,0);

			double x = c(0, i) / c(2, i);
			double y = c(1, i) / c(2, i);

			LineTo_(dc, x, y);

		}
		
		


	}

	void Render(HDC dc)
	{
		// отрисовка модели
		Clear(dc);
		DrawAxes(dc);

		HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		SelectObject(dc, pen);
		
		int ver1, ver2, ver3;
		double u_curr1, u_curr2, u_curr3;
		double x_curr, y_curr;

		Matrix matr_coords = WorldToProject_(model.Vertices);		
		
		for (int i = 0; i < model.N_Edges; i++)
		{
			ver1 = model.Edges[i][0];
			ver2 = model.Edges[i][1];

			u_curr1 = matr_coords(0, ver1);
			u_curr2 = matr_coords(1, ver1);
			u_curr3 = matr_coords(2, ver1);

			x_curr = HomogeneousToCartesian(u_curr1, u_curr3);
			y_curr = HomogeneousToCartesian(u_curr2, u_curr3);

			MoveTo(x_curr, y_curr);

			u_curr1 = matr_coords(0, ver2);
			u_curr2 = matr_coords(1, ver2);
			u_curr3 = matr_coords(2, ver2);

			x_curr = HomogeneousToCartesian(u_curr1, u_curr3);
			y_curr = HomogeneousToCartesian(u_curr2, u_curr3);

			LineTo_(dc, x_curr, y_curr);
		}
	}

};

#endif SCENE_2D_H
