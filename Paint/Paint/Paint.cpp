// Paint.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Paint.h"
#include "vector"
#include "Point.h"
#include <stdio.h>
#include "Line.h"
#include "stdef.h"
#include "Commdlg.h"

#define MAX_LOADSTRING 100

using namespace std;

int x = - 10, y = -10;
vector<Line> lines;
int tool = 0;

//my metods
void cleareRect(HDC hdc);
void drawLines(HDC hdc);
void createMenu();

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PAINT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINT));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PAINT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//

HWND hWnd;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	createMenu();

   // add button
   /*HWND button = CreateWindow(
        L"BUTTON", // this makes a "button" 
		L"Class Options", // this is the text which will appear in the button 
        WS_VISIBLE | WS_CHILD,
        5, // these four lines are the position and dimensions of the button 
        60,
        105,
        20,
        hWnd, // this is the buttons parent window 
        (HMENU)100, // these next two lines pretty much tell windows what to do when the button is pressed 
        (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
        NULL);/**/
   // end add button
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//


Point start_point;
Line now_line;
bool is_down;
HDC hdcLast;
void CaptureScreen(HWND window);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	Point p;

	switch (message)
	{
	case WM_MOUSEMOVE:
		if (!is_down){
			break;
		}
			x=LOWORD(lParam); //узнаём координаты
			y=HIWORD(lParam);
			p.x = x;
			p.y = y;
			switch (tool){
				case WM_LINE:
					now_line.setFinishPoint(p);
					break;
				case WM_PENCEL:
					now_line.setFinishPoint(p);
					lines.push_back(now_line);
					now_line.setStartPoint(p);
					break;
			}
			InvalidateRect(hWnd, NULL, false);
		break;
	case WM_LBUTTONDOWN:		
		{	
			is_down = true;
			x=LOWORD(lParam); //узнаём координаты
			y=HIWORD(lParam);
			p.x = x;
			p.y = y;
			switch (tool){
				case WM_PENCEL:
				case WM_LINE:
					now_line.setStartPoint(p);
					now_line.setFinishPoint(p);
					break;				

			}
			InvalidateRect(hWnd, NULL, false);
		}
		break;
	case WM_LBUTTONUP:	
			is_down = false;
			x=LOWORD(lParam); //узнаём координаты
			y=HIWORD(lParam);
			p.x = x;
			p.y = y;
			switch (tool){
				case WM_PENCEL:
				case WM_LINE:
					now_line.setFinishPoint(p);					
					lines.push_back(now_line);
					now_line.update();
					break;
			}
			InvalidateRect(hWnd, NULL, false);
		break;		
	case WM_COMMAND:             		
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);		

		// Parse the menu selections:
		switch (wmId)
		{		
				case 1401:
				case 1403:
				case 1405:
				case 1407:
					MessageBox(NULL, L"Change", L"  ", NULL);
		now_line.setWidth(wmId%1400);
		break;
		case WM_LINE:
		case WM_PENCEL:
			tool = wmId;
			break;
		case 1100:
		 CaptureScreen(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
			//cleare	
			if (tool != WM_PENCEL){
				cleareRect(hdc);
			}			
			//draw line
			drawLines(hdc);	
			ValidateRect(hWnd, NULL);
			EndPaint(hWnd, &ps);		
   	    }
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void drawPixel(HDC hdc, int i, int j){
	SetPixel(hdc, i, j, RGB( 0 , 0 , 0 )); 
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void createMenu(){
	   //create menu
   HMENU hMenuDraw = CreateMenu();
   HMENU hPopMenuDraw = CreatePopupMenu();
   HMENU hPopMenuWidth = CreatePopupMenu();

   AppendMenu(hMenuDraw, MF_STRING | MF_POPUP, (UINT)hPopMenuDraw, L"Draw"); 
   AppendMenu(hMenuDraw, MF_STRING, 1100, L"Print");
   AppendMenu(hMenuDraw, MF_STRING, 1200, L"Save");
   AppendMenu(hMenuDraw, MF_STRING, 1300, L"Load");
   AppendMenu(hMenuDraw, MF_STRING | MF_POPUP, (UINT)hPopMenuWidth, L"Width");
   
   AppendMenu(hPopMenuDraw, MF_STRING, WM_PENCEL, L"Pencel");
   AppendMenu(hPopMenuDraw, MF_STRING, WM_LINE, L"Line");

   AppendMenu(hPopMenuWidth, MF_STRING, 1401, L"1");
   AppendMenu(hPopMenuWidth, MF_STRING, 1403, L"3");
   AppendMenu(hPopMenuWidth, MF_STRING, 1405, L"5");
   AppendMenu(hPopMenuWidth, MF_STRING, 1407, L"7");
   AppendMenu(hPopMenuWidth, MF_STRING, 1109, L"9");

   //end create menu

   // add menu
   SetMenu(hWnd, hMenuDraw);
   SetMenu(hWnd, hPopMenuDraw);
   SetMenu(hWnd, hPopMenuWidth);

   //end add menu
}



void cleareRect(HDC hdc){
			RECT rect;
			GetClientRect (hWnd, &rect);
			FillRect (hdc, &rect, (HBRUSH)(COLOR_WINDOW+1));
}
void drawBitmap(HDC hdc);

void drawLines(HDC hdc){	
    //drawBitmap(hdc);		    	
	now_line.draw(hdc);	
    for (int i = 0; i < lines.size(); i++){
		lines[i].draw(hdc);
	}			
}

void drawBitmap(HDC hdc, HBITMAP hBitmap){
    	BITMAP 			bitmap;
    	HDC 			hdcMem;
        HGDIOBJ 		oldBitmap;
		hdcMem = CreateCompatibleDC(hdc);
        oldBitmap = SelectObject(hdcMem, hBitmap);

        GetObject(hBitmap, sizeof(bitmap), &bitmap);
        BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);
}



void CaptureScreen(HWND window)
{
    //Получаем прямоугольную область экрана
    RECT windowRect, clientRect;
    GetWindowRect(window, &windowRect);
    GetClientRect(window, &clientRect); // Это изменяет только коордианаты правого угла =(
 
 
    //Размеры битмэпа
    int bitmap_dx = clientRect.right-clientRect.left;
    int bitmap_dy = clientRect.bottom-clientRect.top;
	int x = (windowRect.right - windowRect.left - clientRect.right) / 2;
	int y = windowRect.bottom - windowRect.top - clientRect.bottom - x;
 
    BITMAPINFOHEADER bmpInfoHeader;
    BITMAPFILEHEADER bmpFileHeader;
    BITMAP* pBitmap;
 
    bmpFileHeader.bfType = 0x4d42;
    bmpFileHeader.bfSize = 0;
    bmpFileHeader.bfReserved1 = 0;
    bmpFileHeader.bfReserved2 = 0;
    bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 
    bmpInfoHeader.biSize = sizeof(bmpInfoHeader);
    bmpInfoHeader.biWidth = bitmap_dx;
    bmpInfoHeader.biHeight = bitmap_dy;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 24;
    bmpInfoHeader.biCompression = BI_RGB;
    bmpInfoHeader.biSizeImage = bitmap_dx*bitmap_dy*(24/8);
    bmpInfoHeader.biXPelsPerMeter = 0;
    bmpInfoHeader.biYPelsPerMeter = 0;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;
 
    BITMAPINFO info;
    info.bmiHeader = bmpInfoHeader;
 
    BYTE* memory;
    HDC winDC = GetWindowDC(window);
    HDC bmpDC = CreateCompatibleDC(winDC);
    //Создаем битмэп
    HBITMAP bitmap = CreateDIBSection(winDC, &info, DIB_RGB_COLORS, (void**)&memory, NULL, 0);
    SelectObject(bmpDC, bitmap);//Выбираем в контекст битмэп
    BitBlt(bmpDC, 0, 0, bitmap_dx, bitmap_dy, winDC, x, y, SRCCOPY);
    ReleaseDC(window, winDC);
/*
OPENFILENAME ofn;//Указатель на структуру с данными инициализации диалогового окна
    LPWSTR strFilter = L"Файлы данных *.bmp\0";
    LPWSTR strFileName = L"";
    memset(&ofn, 0, sizeof(OPENFILENAME));//Обнуление ofn
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = window;
    ofn.lpstrFilter = strFilter;
    ofn.lpstrFile = strFileName;//Буфер для имени файла
    ofn.nMaxFile = MAX_PATH;//Размер файла
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
    ofn.lpstrFile = strFileName;
    GetSaveFileName(&ofn); 
*/
//	LPWSTR buf;
//	GetDlgItemText(hWnd,1,buf,sizeof(buf));	
 
    HANDLE hFile = CreateFile(
		L"D:/picture.bmp", GENERIC_WRITE, 
        0, NULL, OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,NULL);
 
    if (hFile == INVALID_HANDLE_VALUE) 
        return;
 
    DWORD dwWritten = 0;
    WriteFile(hFile, &bmpFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    WriteFile(hFile, &bmpInfoHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
    WriteFile(hFile, memory, bmpInfoHeader.biSizeImage, &dwWritten, NULL);
    CloseHandle(hFile);
}