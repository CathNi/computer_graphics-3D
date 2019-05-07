#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <fstream>
using namespace std;
#include <string>
#include "Matrix.h"
#include "AffineTransform.h"
#include "Model2D.h"
#include "Camera2D.h"
#include "Camera3D.h"
#include "Scene2D.h"
#include "Vector3D.h"
#include "Data.h"

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// прототип оконной процедуры
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// основная процедура
{
	// Первая составляющая часть основной процедуры - создание окна: сначала описывается оконный класс wc, затем создается окно hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// имя оконной процедуры, закрепленной за данным классом
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// идентификатор приложения, содержащий адрес начала выделенной ему области памяти
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// меню в оконном классе отсутствует
	wc.lpszClassName = (LPCSTR)"class";				// имя оконного класса, используемое при создании экземпляров окна
	RegisterClass(&wc);								// регистрация класса wc

	HWND hWnd = CreateWindow(						// hWnd - дескриптор, идентифицирующий окно; функция создания окна заполняет дескриптор hWnd ненулевым значением
		(LPCSTR)"class",							// имя оконного класса
		(LPCSTR)"3D Model Drawer Nikulina EYU",					// заголовок окна
		WS_OVERLAPPEDWINDOW,						// стиль окна
		200,200,400,400,							// координаты на экране левого верхнего угла окна, его ширина и высота
		nullptr,nullptr,hInstance,nullptr);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	// Вторая составляющая часть основной процедуры - основной цикл обработки системных сообщений, который ожидает сообщения и рассылает их соответствующим окнам
	MSG msg;
	while(GetMessage(&msg,nullptr,0,0))				// функция GetMessage выбирает из очереди сообщение и заносит его в структуру msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// функция DispatchMessage оповещает систему о необходимости вызова оконной процедуры
	}

	return 0;
}

// В основном модуле объявляется только одна глобальная переменная - создаётся объект класса Scene2D
// Все дальнейшие действия осуществляются посредством обращения к методам, реализованным в этом классе
Scene2D scene(L,R,B,T);

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// оконная процедура принимает и обрабатывает все сообщения, отправленные окну
{
	switch(msg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

	case WM_CREATE:
		{
			scene.model.LoadVerges("C:\\Users\\user\\Documents\\University\\kg\\3d\\verges.txt");
			scene.model.LoadVertices("C:\\Users\\user\\Documents\\University\\kg\\3d\\ver.txt");

			InvalidateRect(hWnd,nullptr,false);
			return 0;
		}

	case WM_PAINT:
		{
			HDC dc = GetDC(hWnd);
			scene.Clear(dc);
			scene.Render(dc);
			ReleaseDC(hWnd,dc);
			return DefWindowProc(hWnd,msg,wParam,lParam);
		}

	case WM_SIZE:
		{
			HDC dc = GetDC(hWnd);
			scene.SetResolution(dc);
			ReleaseDC(hWnd,dc);
			InvalidateRect(hWnd,nullptr,false);
			return 0;
		}
	case WM_KEYDOWN:				
	{									// Подробнее - https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms646280.aspx
		switch (wParam)					// Параметр wParam определяет, какая конкретно клавиша была нажата
		{								// Виртуальные коды всех клавиш можно найти на странице https://msdn.microsoft.com/ru-ru/library/windows/desktop/dd375731.aspx
		case VK_ESCAPE:				// Для примера повесим на клавишу <ESCAPE> очистку рабочей области окна hWnd
		{
			HDC dc = GetDC(hWnd);
			scene.Clear(dc);
			ReleaseDC(hWnd, dc);
			break;
		}

		case VK_UP:
		{
			scene.DeltaNorm(3);
		}break;

		case VK_DOWN:
		{
			scene.DeltaNorm(4);
		}break;

		case VK_LEFT:
		{
			scene.DeltaNorm(1);
		}break;

		case VK_RIGHT:
		{
			scene.DeltaNorm(2);
		}break;



		case VK_NUMPAD1:
		{
			scene.model.Apply(Translation(-1, 0, 0));
			
		}break;
		case VK_NUMPAD2:
		{
			scene.model.Apply(Translation(1, 0, 0));
		}break;
		case VK_NUMPAD4:
		{
			scene.model.Apply(Translation(0, -1, 0));
		}break;
		case VK_NUMPAD5:
		{
			scene.model.Apply(Translation(0, 1, 0));
		}break;
		case VK_NUMPAD7:
		{
			scene.model.Apply(Translation(0, 0, -1));
		}break;
		case VK_NUMPAD8:
		{
			scene.model.Apply(Translation(0, 0, 1));
		}break;

		case VK_ADD:
		{
			if(scene.Deltad(1))
				scene.model.Apply(Scaling(1.1, 1.1, 1.1));

		}break;
		case VK_SUBTRACT:
		{
			if(scene.Deltad(-1))
				scene.model.Apply(Scaling(0.9, 0.9, 0.9));
		}break;

		case VK_NUMPAD3://отражение относительно XY
		{
			scene.model.Apply(Reflection(4));

		}break;

		case VK_NUMPAD6://отражение относительно YZ: 
		{
			scene.model.Apply(Reflection(5));

		}break;

		case VK_NUMPAD9://отражение относительно XZ: 
		{
			scene.model.Apply(Reflection(3));
		}break;


		case 0x58://отражение относительно OX: X
		{
			scene.model.Apply(Reflection(1));

		}break;
		case 0x59://отражение относительно OY: Y
		{
			scene.model.Apply(Reflection(2));

		}break;

		case 0x5A://отражение относительно OZ: Z
		{
			scene.model.Apply(Reflection(3));
		}break;

		case 0x4F:// отражение относительно начала: O
		{
			scene.model.Apply(Reflection(0));

		}break;

		case 0x31:// поворот на угол в радианах: R
		{
			scene.model.Apply(Rotation(0.05, 3));

		}break;


		case 0x32:// поворот на угол в радианах: R
		{
			scene.model.Apply(Rotation(0.05, 2));

		}break;


		case 0x33:// поворот на угол в радианах: R
		{
			scene.model.Apply(Rotation(0.05, 1));

		}break;

		case VK_F1:
		{
			Matrix mt;
			double x, y, z;
			for (int i = 0; i < 10; i++)
			{
				scene.model.Get_Edge(0, x, y, z);
				My_Transform(mt, x, y, z);
				scene.model.Apply(mt);
			}
			
		}break;
		case VK_F2:
		{
			Matrix mt;
			
			double x1, y1, z1, x2, y2, z2;
			scene.model.Get_Edge(0, x1, y1, z1);
			scene.model.Get_Edge(1, x2, y2, z2);

			mt = Translation(x1, y1, z1)*
				Rotation(z2 - z1, x2 - x1)
				*Rotation(0.05, 1)
				*Rotation(z2 - z1, -x2 + x1)
				*Translation(-x1, -y1, -z1);

			scene.model.Apply(mt);
		
		}break;

		}
		InvalidateRect(hWnd, NULL, FALSE);
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	
	case WM_LBUTTONDOWN:
	{	POINT P;
		P.x = GET_X_LPARAM(lParam);		
		P.y = GET_Y_LPARAM(lParam);
		scene.StartDragging(P.x, P.y);
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		if (scene.isDragging())
		{
			scene.Drag(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			InvalidateRect(hWnd, NULL, FALSE);
		}
		return 0;
	}
	case WM_LBUTTONUP:
	{
		scene.StopDragging();
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		double wheelDelta = 0, x;
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);			
		pt.y = GET_Y_LPARAM(lParam);
		
		ScreenToClient(hWnd, &pt);
		pt.x -= 1;
		pt.y -= 1;
		double k = (GET_WHEEL_DELTA_WPARAM(wParam) > 0) ? 1.1 : 0.9;
		scene.scaling(pt.x, pt.y, k);
		InvalidateRect(hWnd, NULL, FALSE);
	}break;


	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
	return 0;
}
