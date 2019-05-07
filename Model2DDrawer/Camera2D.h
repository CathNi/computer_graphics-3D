#ifndef CAMERA_2D_H
#define CAMERA_2D_H

class Camera2D
{
public:
	double L, R, B, T;			// Мировые координаты границ окна
	int W, H;					// Разрешение рабочей области
	double posX, posY;			// Позиция графического курсора в мировых координатах (для функций MoveTo и LineTo)
	int WorldToScreenX(double X)
	{
		return (int)((X - L) / (R - L)*W);
	}
	int WorldToScreenY(double Y)		
	{
		return (int)((T - Y) / (T - B)*H);
	}
	double ScreenToWorldX(int X)		
	{
		return double(L + ((R - L)*((double)X + 0.5) / W));
	}
	double ScreenToWorldY(int Y)		
	{
		return double(T - ((T - B)*((double)Y + 0.5) / H));
	}



public:
	Camera2D(double L, double R, double B, double T) : L(L), R(R), B(B), T(T) {};

	void Clear(HDC dc)
	{
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		Rectangle(dc, 0, 0, r.right, r.bottom);
	}
	void SetResolution(HDC dc)
	{
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);

		W = r.right + 1;
		H = r.bottom + 1;

		double delta = (T - B)*W / double(2 * H), middle = (L + R) / 2;
		L = middle - delta;
		R = middle + delta;

		//DX = ((R - L) / W);
		//DY = ((T - B) / H);
	}
	void MoveTo(double X, double Y)
	{
		posX = X;
		posY = Y;
	}
	void LineTo_(HDC dc, double X, double Y)
	{
		MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), NULL); // точка перемещена
		LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));//линия нарисована

		MoveTo(X, Y);//новые координаты
	}
	void Axes(HDC dc)
	{
		HPEN pen = CreatePen(PS_DOT, 0, RGB(230, 230, 230));
		SelectObject(dc, pen);

		MoveTo(L, 0);
		LineTo_(dc, R, 0);
		MoveTo(0, B);
		LineTo_(dc, 0, T);
	}

	typedef double(*Func)(double);
	double f(double x)
	{
		double A, B, C;
		A = -1; B = 1;
		C = -2;
		return (-1.0)*(A*x + C);
	}

	void Plot(HDC dc)
	{
		double x_curr = ScreenToWorldX(0);
		MoveTo(x_curr, f(x_curr));
		double dx = (R - L) / W;
		for (; x_curr < R; x_curr += dx)
			LineTo_(dc, x_curr, f(x_curr));
	}

private:
	bool is_dragg;
	int x_prev, y_prev;
	double Lp, Rp, Tp, Bp;
	// Скрытые поля, необходимые для реализации перетаскивания графика функции
	// Например, поля, в которых хранятся экранные координаты курсора в момент начала перетаскивания,
	// или мировые координаты границ окна в момент начала перетаскивания и т.д.
public:
	// Методы, используемые для реализации перетаскивания графика функции
	void StartDragging(int X, int Y)
	{
		is_dragg = true;
		x_prev = X;
		y_prev = Y;
	}
	void Drag(int X, int Y)
	{
		move((X - x_prev), (Y - y_prev));

		x_prev = X;
		y_prev = Y;

	}
	void StopDragging()
	{
		is_dragg = false;
	}
	bool isDragging()
	{
		return is_dragg;
	}
	void move(int x, int y)
	{
		double
			delta_X_world = (R - L)*x / W,
			delta_Y_world = (T - B)*y / H;
		L -= delta_X_world;
		R -= delta_X_world;
		B += delta_Y_world;
		T += delta_Y_world;

		x_prev = x; y_prev = y;
	}
	void scaling(int x_curr, int y_curr, double k)
	{
		double X_world = ScreenToWorldX(x_curr), Y_world = ScreenToWorldY(y_curr);
		L = X_world - (X_world - L) / k;
		R = (R - X_world) / k + X_world;
		T = (T - Y_world) / k + Y_world;
		B = Y_world - (Y_world - B) / k;

	}
	
};

#endif CAMERA_2D_H
