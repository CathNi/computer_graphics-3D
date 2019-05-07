#ifndef MATRIX_H
#define MATRIX_H
#include <fstream>
class Matrix
{
public:
	typedef double Cell;
private:
	int nstr, nstb;
	Cell **cells;
	void AllocateCells(int, int);
	void FreeCells();
	
	
public:
	Matrix() : nstr(0), nstb(0),angle(0), cells(nullptr) {}	// Конструктор по умолчанию
	Matrix(const Matrix &);					// Конструктор копирования
	Matrix(int, int, Cell*);						// Конструктор матрицы из списка Cell
	~Matrix();								// Деструктор
	double angle;
	Cell &operator()(int i, int j) { return cells[i][j]; }

	Matrix&operator=(const Matrix &);		// Перегрузка оператора присваивания
	Matrix operator+(const Matrix &);		// Сложение матриц
	Matrix operator-(const Matrix &);		// Вычитание матриц
	Matrix operator*(const Matrix &);		// Умножение матриц

	friend istream &operator>>(istream &, Matrix &);		// Перегрузка оператора >> для ввода матрицы
	friend ostream &operator<<(ostream &, const Matrix &);

	void print_to_file(fstream&out)
	{
		if (!out.is_open())
			return;

		out << '\n';

		for (int i = 0; i < nstr; i++)
		{
			for (int j = 0; j < nstb; j++)
				out << cells[i][j] << "  ";

			out << '\n \n';
		}

		out << '\n';
	}
};

Matrix::Matrix(const Matrix &m)
{
	AllocateCells(m.nstr, m.nstb);
	for (int i = 0; i < nstr; i++)
		for (int j = 0; j < nstb; j++)
			cells[i][j] = m.cells[i][j];
}

Matrix::Matrix(int N, int M, Cell* list = nullptr)
{
	AllocateCells(N, M);
	
	if (list == nullptr)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				cells[i][j] = 0;
	else
	{
		int k = 0;

		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				cells[i][j] = list[k++];
	}
}

Matrix::~Matrix()
{
	FreeCells();
}

Matrix& Matrix::operator=(const Matrix &m)
{
	if (nstr != m.nstr && nstb != m.nstb)
	{
		FreeCells();
		AllocateCells(m.nstr, m.nstb);
	}

	for (int i = 0; i < nstr; i++)
		for (int j = 0; j < nstb; j++)
			cells[i][j] = m.cells[i][j];

	return *this;
}

Matrix Matrix::operator+(const Matrix &m)
{
	Matrix res(*this);
	if (nstr == m.nstr && nstb == m.nstb)
	{
		for (int i = 0; i < nstr; i++)
			for (int j = 0; j < nstb; j++)
				res.cells[i][j] += m.cells[i][j];
	}
	return res;
}

Matrix Matrix::operator-(const Matrix &m)
{
	// вычитание матриц

	Matrix res(*this);
	if (nstr == m.nstr && nstb == m.nstb)
	{
		for (int i = 0; i < nstr; i++)
			for (int j = 0; j < nstb; j++)
				res.cells[i][j] -= m.cells[i][j];
	}
	return res;
}

Matrix Matrix::operator*(const Matrix &m)
{
	Matrix res(nstr, m.nstb);
	if (nstb = m.nstr)
	{
		for (int i = 0; i < nstr; i++)
			for (int j = 0; j < m.nstb; j++)
			{
				double summ = 0.0;

				for (int k = 0; k < nstb; k++)
				{
					double a = cells[i][k], b = m.cells[k][j];
					summ += a*b;
				}
				res.cells[i][j] = summ;
			}
	}
	return res;
}

istream &operator>>(istream &fi, Matrix &m)
{
	for (int i = 0; i<m.nstr; i++)
		for (int j = 0; j<m.nstb; j++)
			fi >> m.cells[i][j];
	return fi;
}

ostream &operator<<(ostream &fo, const Matrix &m)
{
	for (int i = 0; i<m.nstr; i++)
	{
		fo << "  ";
		for (int j = 0; j<m.nstb; j++)
			fo << m.cells[i][j] << " \t";
		fo << "\n";
	}
	return fo;
}

void Matrix::AllocateCells(int n, int m)
{
	nstr = n;
	nstb = m;
	cells = new Cell*[nstr];
	for (int i = 0; i < nstr; i++)
		cells[i] = new Cell[nstb];
}

void Matrix::FreeCells()
{

	for (int i = 0; i < nstr; i++)
		delete cells[i];
	delete cells;
	nstr = nstb = 0;
}



#endif MATRIX_H

