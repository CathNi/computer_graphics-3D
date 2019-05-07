#ifndef MODEL_2D_H
#define MODEL_2D_H

class Model2D
{
public:
	typedef int** MatrixInt; // тип данных для хранения целочисленной матрицы ребер
	Matrix Vertices;
	MatrixInt Edges;
	Matrix Verges;
	int N_Vertices, N_Edges, N_Verges;

	Model2D()
	{
		Vertices = Matrix();
		Verges = Matrix();
		N_Verges = N_Vertices = N_Edges = 0;
		Edges = nullptr;
	}
	/*
	void DefineTree()
	{
		fstream out("D:\\1.txt");
		double r = 5, phi = 0.f, dphi = 0.f;
		int n = 10;
		dphi = 2 * 3.14 / n;
		Matrix::Cell *cells = new Matrix::Cell[4 * (n + 1)];

		Edges = new int*[n + n];

		for (int i = 0; i < n + n; i++)
			Edges[i] = new int[2];

		cells[0] = cells[1] = cells[2] = 0;
		cells[3] = 1;

		for (int i = 1; i < n + 1; i++)
		{
			for (int j = 0; j < 4; j++)
				switch (j)
				{
				case 0: cells[i + j] = r*cos(phi); break;
				case 1: cells[i + j] = 0; break;
				case 2: cells[i + j] = r*sin(phi); break;
				case 3: cells[i + j] = 1; break;
				}

			phi += dphi;
		}

		for (int i = 0; i < n + n - 1; i+=2)
		{
			Edges[i][0] = 0;
			Edges[i][1] = i;
			Edges[i + 1][0] = i;
			Edges[i + 1][1] = i + 1;
		}

		N_Vertices = n + 1;
		N_Edges = n + n;


		for (int i = 1; i < n + 1; i++)
		{
			out << cells[i] << " " << cells[i + 1] << " " << cells[i + 2] << " " << cells[i + 3] << '\n';

		}

		Vertices = Matrix(4, N_Vertices, cells);
		out << "\n Vertices:\n";
		Vertices.print_to_file(out);
		out << "\n \tEdges:\n";
		for (int i = 0; i < n + n; i++)
			out << Edges[i][0] << "  " << Edges[i][1] << "\n";
		out.close();
	}
	*/
	void Get_Edge(int number, double &x, double&y, double&z)
	{
		if (number >= 0 && number <= N_Vertices)
		{

			double zn = Vertices(3, number);
			x = Vertices(0, number);
			y = Vertices(1, number);
			z = Vertices(2, number);

			x /= zn; y /= zn; z /= zn;
		}
		else
			x = y = z = 0;
	}



	void FindEdges()
	{
		N_Edges = N_Verges * 3;
		int k = 0;
		Matrix::Cell *cells = new Matrix::Cell[N_Edges * 2];
		

		for (int i = 0; i < N_Verges; i++)
			for (int j = 0; j < 2; j++)
				cells[k++] = Verges(j, i);

		for (int i = 0; i < N_Verges; i++)
			for (int j = 1; j < 3; j++)
				cells[k++] = Verges(j, i);

		for (int i = 0; i < N_Verges; i++)
			for (int j = 0; j < 3; j += 2)
				cells[k++] = Verges(j, i);

		Matrix matr (N_Edges, 2, cells);

		Edges = new int*[N_Edges];
		for (int i = 0; i < N_Edges; i++)
			Edges[i] = new int[2];

		for (int i = 0; i < N_Edges; i++)
			for (int j = 0; j < 2; j++)
				Edges[i][j] = matr(i, j);

	}

	void LoadVerges(string FileName) // вершины
	{
		ifstream fin(FileName);
		if (!fin.is_open())
			return;

		fin >> N_Verges;

		Matrix::Cell *cells, buff;
		cells = new Matrix::Cell[3 * N_Verges];

		for (int i = 0; i < 3 * N_Verges; i++)
		{
			fin >> buff;
			cells[i] = buff;
		}
			
		Verges = Matrix(3, N_Verges, cells);

		fstream out("D:\\out_v.txt");
		out.clear();
		Verges.print_to_file(out);

		fin.close();

		FindEdges();

	}

	void LoadVertices(string FileName) // ребра
	{
		ifstream fin(FileName);
		if (!fin.is_open())
			return;

		fin >> N_Vertices;
		
		Matrix::Cell *cells, buff;
		cells = new Matrix::Cell[4 * N_Vertices];

		for (int i = 0; i < 4 * N_Vertices; i++)
			fin >> cells[i];

		Vertices = Matrix(4, N_Vertices, cells);

	}

	void Apply(Matrix A)
	{
		Vertices = A*Vertices;		
	}

};

#endif MODEL_2D_H