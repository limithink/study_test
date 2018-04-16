// winapiopengl.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "main.h"
#include "glProc.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
//
//HDC hdc;
//HGLRC hglrc;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//mydef
namespace mydef
{
void glEnvEnable(HWND hWnd, HDC *phdc, HGLRC *phglrc);
void glEnvDisable(HWND hWnd, HDC hdc, HGLRC hglrc);
void OpenFile(HWND hWnd, mydef::Map **retMapData);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	AllocConsole();

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIOPENGL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIOPENGL));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	FreeConsole();
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIOPENGL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPIOPENGL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 1280, 720, nullptr, nullptr, hInstance, nullptr);

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
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;
	static HGLRC hglrc;
	//static FILE *fp = NULL;
	static mydef::Map *MapData = NULL;
	switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
				//DestroyWindow(hWnd);
				PostMessage(hWnd, WM_CLOSE, NULL, NULL);
				break;
			case ID_FILE_OPEN:
				{
					mydef::OpenFile(hWnd, &MapData);
					mydef::DispScene(MapData);
					SwapBuffers(hdc);
				}
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CREATE:
		{
			mydef::glEnvEnable(hWnd, &hdc, &hglrc);
			//SetTimer(hWnd, 33, 1, NULL);
			mydef::Init();
		}
		break;
	case WM_SIZE:
		{
			mydef::ReSize(LOWORD(lParam), HIWORD(lParam));
		}
		break;
	case WM_TIMER:
		{
			//InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	case WM_PAINT:
        {
            PAINTSTRUCT ps;
			HDC hdc0 = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
			//mydef::DispScene();
			SwapBuffers(hdc);
			
			//ValidateRect(hWnd, NULL);
        }
        break;
	case WM_CLOSE:
		{
			int ret = MessageBox(hWnd, TEXT("Are you sure to exit!?"), TEXT("Caution"), MB_YESNO);
			if (ret == IDYES) PostMessage(hWnd, WM_DESTROY, NULL, NULL);
		}
		break;
    case WM_DESTROY:
		delete MapData;
		mydef::glEnvDisable(hWnd, hdc, hglrc);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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


namespace mydef
{

void glEnvEnable(HWND hWnd, HDC *phdc, HGLRC *phglrc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	*phdc = GetDC(hWnd);
	//*phdc = GetDC(hWnd);
	//pfd registration
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	//
	int iFormat = ChoosePixelFormat(*phdc, &pfd);
	SetPixelFormat(*phdc, iFormat, &pfd);
	//
	*phglrc = wglCreateContext(*phdc);
	//*phglrc = wglCreateContext(*phdc);
	wglMakeCurrent(*phdc, *phglrc);
}

void glEnvDisable(HWND hWnd, HDC hdc, HGLRC hglrc)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
	ReleaseDC(hWnd, hdc);
}

void OpenFile(HWND hWnd, mydef::Map **retMapData)
{
	using namespace mydef;
	//
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	OPENFILENAME ofn;
	wchar_t FileName[MAX_PATH] = TEXT("");
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = TEXT("All Files (*.*)\0*.*\0");
	ofn.lpstrFile = FileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = TEXT("txt");
	if (!GetOpenFileName(&ofn))
		return;
	std::wifstream FileObj(FileName);
	WriteConsole(hstdout, TEXT("File Opened! Dir: "), 18, NULL, NULL);
	WriteConsole(hstdout, FileName, lstrlen(FileName), NULL, NULL);
	WriteConsole(hstdout, TEXT("\n\n"), 2, NULL, NULL);
	//Read File
	//WriteConsole(hstdout, TEXT("MapPoint:\n"), 10, NULL, NULL);
	*retMapData = new Map;
	(*retMapData)->head = NULL;
	(*retMapData)->TotalPolygon = 0;
	LinkList<Polygon*> **cur = &((*retMapData)->head);
	while (!FileObj.eof())
	{
		int TotalPoint;
		FileObj >> TotalPoint;
		//char strTemp[100];
		//sprintf(strTemp, "%d\n", TotalPoint);
		//WriteConsoleA(hstdout, strTemp, lstrlenA(strTemp), NULL, NULL);
		*cur = new LinkList<Polygon*>;
		(*cur)->obj = new Polygon(TotalPoint);
		(*cur)->next = NULL;
		LinkList<pMP> **cursor;
		cursor = &((*cur)->obj->head);
		cur = &((*cur)->next);
		//
		for (int ctr = 0; ctr < TotalPoint; ctr++)
		{
			MP mp;
			FileObj >> mp.lon;
			FileObj >> mp.lat;
			//sprintf(strTemp, "%f  %f\n", mp.lon, mp.lat);
			//WriteConsoleA(hstdout, strTemp, lstrlenA(strTemp), NULL, NULL);
			(*cursor)->obj->lon = mp.lon;
			(*cursor)->obj->lat = mp.lat;
			cursor = &((*cursor)->next);
		}
		(*retMapData)->TotalPolygon++;
	}
	WriteConsole(hstdout, TEXT("\nFile Read Complete!\n\n"), 22, NULL, NULL);
}

}