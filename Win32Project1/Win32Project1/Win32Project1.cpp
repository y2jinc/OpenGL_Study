// Win32Project1.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Win32Project1.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// opengl ���� ���� ����
float	angle = 0.0f;							// ȸ���ϴ� �ﰢ���� ���� ������ ��´�.
HDC		g_HDC = NULL;							// ���� ��ġ ����
HWND	g_hwnd = NULL;
bool	g_done = false;
bool	g_fullscreen = FALSE;

INT		DEFAULT_WIDTH = 1024;
INT		DEFAULT_HEIGHT = 768;

void SetupPixelFormat(HDC hDC)
{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,	// ���� ��Ʈ���� ����.
		0,					// ���Ĺ��� ����.
		0,					// ����Ʈ ��Ʈ ����.
		0,					// ���� ���� ����.
		0, 0, 0, 0,			// ���� ��Ʈ�鵵 ����
		16,					// Z ���۴� 16 ��Ʈ
		0,					// ���ٽ� ���� ����.
		0,					// ���� ���� ����.
		PFD_MAIN_PLANE,		// ���� ����� ���.
		0,					// �����.
		0, 0, 0 };			// ���̾� ����ũ���� ����.	

	// ���� ������ �ȼ� ���� ����. ���� ��ȯ
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// �ȼ� ������ ��ġ ���Ű� ����
	SetPixelFormat(hDC, nPixelFormat, &pfd);	
}


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	DEVMODE	devModeScreen;
	ZeroMemory(&devModeScreen, sizeof(devModeScreen));
	devModeScreen.dmSize = sizeof(devModeScreen);
	devModeScreen.dmPelsWidth = DEFAULT_WIDTH;
	devModeScreen.dmPelsHeight = DEFAULT_HEIGHT;
	devModeScreen.dmBitsPerPel = 32;
	devModeScreen.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

	if (g_fullscreen && ChangeDisplaySettings(&devModeScreen, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		g_fullscreen = false;
	}

	int extendedWindowStyle = 0;
	int windowStyle = 0;
	if (g_fullscreen)	
	{
		extendedWindowStyle = WS_EX_APPWINDOW;
		windowStyle = WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		extendedWindowStyle = 0;
		windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	}

	RECT windowRect;
	windowRect.top = 0;
	windowRect.left = 0;
	windowRect.bottom = DEFAULT_WIDTH;
	windowRect.right = DEFAULT_HEIGHT;

	AdjustWindowRectEx(&windowRect, windowStyle, FALSE, extendedWindowStyle);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));
    MSG msg;

    // �⺻ �޽��� �����Դϴ�.
    while (!g_done)
    {
		PeekMessage(&msg, g_hwnd, 0, 0, PM_REMOVE);

		if (msg.message == WM_QUIT)
		{
			g_done = TRUE;
		}
		else
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();


			angle = angle + 0.1f;					// increase our rotation angle counter
			if (angle >= 360.0f)					// if we've gone in a circle, reset counter
				angle = 0.0f;
			glTranslatef(0.0f, 0.0f, -5.0f);		// move back 5 units
			glRotatef(angle, 0.0f, 0.0f, 1.0f);		// rotate along z-axis

			glColor3f(1.0f, 0.0f, 0.0f);			// set color to red
			glBegin(GL_TRIANGLES);					// draw the triangle
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);
			glEnd();

			SwapBuffers(g_HDC);

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

    }

    return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
      CW_USEDEFAULT, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hwnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC		hDC;
	static HGLRC	hRC;
	int	height, width;

    switch (message)
    {
	case WM_CREATE:
		hDC = GetDC(hWnd);
		g_HDC = hDC;
		SetupPixelFormat(hDC);

		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

		return 0;
		break;

	case WM_CLOSE:
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		
		PostQuitMessage(0);
		g_done = true;
		return 0;
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
            EndPaint(hWnd, &ps);
        }
        break;

	case WM_SIZE:
		height = HIWORD(lParam);
		width = LOWORD(lParam);

		if (height == 0)
		{
			height = 1;
		}

		// ����Ʈ ũ�⸦ �ٽ� ����
		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);	// ���� ��ķ� ����
		glLoadIdentity();

		// ��Ⱦ�� ����
		gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);

		glMatrixMode(GL_MODELVIEW);		// motel view  ��ķ� ����
		glLoadIdentity();				// �� �� ��� �ʱ�ȭ
		return 0;
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
