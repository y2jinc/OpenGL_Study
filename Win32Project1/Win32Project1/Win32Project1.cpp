// Win32Project1.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Win32Project1.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// opengl 관련 전역 변수
float	angle = 0.0f;							// 회전하는 삼각형의 현재 각도를 담는다.
HDC		g_HDC = NULL;							// 전역 장치 문맥
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
		0, 0, 0, 0, 0, 0,	// 색상 비트들은 무시.
		0,					// 알파버퍼 없음.
		0,					// 시프트 비트 무시.
		0,					// 누적 버퍼 없음.
		0, 0, 0, 0,			// 누적 비트들도 무시
		16,					// Z 버퍼는 16 비트
		0,					// 스텐실 버퍼 없음.
		0,					// 보조 버퍼 없음.
		PFD_MAIN_PLANE,		// 메인 드로잉 평면.
		0,					// 예약됨.
		0, 0, 0 };			// 레이어 마스크들을 무시.	

	// 가장 근접한 픽셀 형식 선택. 색인 반환
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// 픽셀 형식을 장치 문매겡 설정
	SetPixelFormat(hDC, nPixelFormat, &pfd);	
}


// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
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

    // 기본 메시지 루프입니다.
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
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
            // 메뉴 선택을 구문 분석합니다.
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
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

		// 뷰포트 크기를 다시 설정
		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);	// 투영 행렬로 설정
		glLoadIdentity();

		// 종횡비 설정
		gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);

		glMatrixMode(GL_MODELVIEW);		// motel view  행렬로 설정
		glLoadIdentity();				// 모델 뷰 행렬 초기화
		return 0;
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
