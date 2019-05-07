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

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// �������� ������� ���������
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// �������� ���������
{
	// ������ ������������ ����� �������� ��������� - �������� ����: ������� ����������� ������� ����� wc, ����� ��������� ���� hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// ��� ������� ���������, ������������ �� ������ �������
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// ������������� ����������, ���������� ����� ������ ���������� ��� ������� ������
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// ���� � ������� ������ �����������
	wc.lpszClassName = (LPCSTR)"class";				// ��� �������� ������, ������������ ��� �������� ����������� ����
	RegisterClass(&wc);								// ����������� ������ wc

	HWND hWnd = CreateWindow(						// hWnd - ����������, ���������������� ����; ������� �������� ���� ��������� ���������� hWnd ��������� ���������
		(LPCSTR)"class",							// ��� �������� ������
		(LPCSTR)"3D Model Drawer Nikulina EYU",					// ��������� ����
		WS_OVERLAPPEDWINDOW,						// ����� ����
		200,200,400,400,							// ���������� �� ������ ������ �������� ���� ����, ��� ������ � ������
		nullptr,nullptr,hInstance,nullptr);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	// ������ ������������ ����� �������� ��������� - �������� ���� ��������� ��������� ���������, ������� ������� ��������� � ��������� �� ��������������� �����
	MSG msg;
	while(GetMessage(&msg,nullptr,0,0))				// ������� GetMessage �������� �� ������� ��������� � ������� ��� � ��������� msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// ������� DispatchMessage ��������� ������� � ������������� ������ ������� ���������
	}

	return 0;
}

// � �������� ������ ����������� ������ ���� ���������� ���������� - �������� ������ ������ Scene2D
// ��� ���������� �������� �������������� ����������� ��������� � �������, ������������� � ���� ������
Scene2D scene(L,R,B,T);

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// ������� ��������� ��������� � ������������ ��� ���������, ������������ ����
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
	{									// ��������� - https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms646280.aspx
		switch (wParam)					// �������� wParam ����������, ����� ��������� ������� ���� ������
		{								// ����������� ���� ���� ������ ����� ����� �� �������� https://msdn.microsoft.com/ru-ru/library/windows/desktop/dd375731.aspx
		case VK_ESCAPE:				// ��� ������� ������� �� ������� <ESCAPE> ������� ������� ������� ���� hWnd
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

		case VK_NUMPAD3://��������� ������������ XY
		{
			scene.model.Apply(Reflection(4));

		}break;

		case VK_NUMPAD6://��������� ������������ YZ: 
		{
			scene.model.Apply(Reflection(5));

		}break;

		case VK_NUMPAD9://��������� ������������ XZ: 
		{
			scene.model.Apply(Reflection(3));
		}break;


		case 0x58://��������� ������������ OX: X
		{
			scene.model.Apply(Reflection(1));

		}break;
		case 0x59://��������� ������������ OY: Y
		{
			scene.model.Apply(Reflection(2));

		}break;

		case 0x5A://��������� ������������ OZ: Z
		{
			scene.model.Apply(Reflection(3));
		}break;

		case 0x4F:// ��������� ������������ ������: O
		{
			scene.model.Apply(Reflection(0));

		}break;

		case 0x31:// ������� �� ���� � ��������: R
		{
			scene.model.Apply(Rotation(0.05, 3));

		}break;


		case 0x32:// ������� �� ���� � ��������: R
		{
			scene.model.Apply(Rotation(0.05, 2));

		}break;


		case 0x33:// ������� �� ���� � ��������: R
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
