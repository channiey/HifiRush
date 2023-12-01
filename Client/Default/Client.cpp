#include "../Default/stdafx.h"

#ifdef _DEBUG
#include "imgui_impl_win32.h"
#endif // _DEBUG

#include "Client.h"
#include "../Public/Client_Defines.h"

#ifdef _DEBUG
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);// IMGUI
#endif // _DEBUG


#include "MainApp.h"
#include "EngineInstance.h"

#define MAX_LOADSTRING 100

/* Extern */
HINSTANCE   g_hInst;                               
HWND	    g_hWnd;
_uint       g_iFPS;
_bool		g_bLoadPrototype = FALSE;

WCHAR szTitle[MAX_LOADSTRING];                 
WCHAR szWindowClass[MAX_LOADSTRING];           

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
   
#if CONSOLE_LOG
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#endif // CONSOLE_LOG

#endif // _DEBUG

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    srand(time(NULL));

	CMainApp*		pMainApp = nullptr;

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	pMainApp = CMainApp::Create();
	if (nullptr == pMainApp)
		return FALSE;

    /* Cursor Option*/
    {
        if (CURSOL_HIDE)
            ShowCursor(FALSE);
        if (CURSOL_LOCK)
        {
            RECT rc2;
            POINT lt, rb;
            GetClientRect(g_hWnd, &rc2);
            lt.x = rc2.left;
            lt.y = rc2.top;
            rb.x = rc2.right;
            rb.y = rc2.bottom;
            ClientToScreen(g_hWnd, &lt);
            ClientToScreen(g_hWnd, &rb);
            rc2.left = lt.x;
            rc2.top = lt.y;
            rc2.right = rb.x;
            rc2.bottom = rb.y;
            ClipCursor(&rc2);
        }
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

	CEngineInstance*		pGameInstance = CEngineInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Default"))))
		return FALSE;

	if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_FPS"))))
		return FALSE;

    _double		fTimeAcc = 0.f;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message) break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		fTimeAcc += pGameInstance->Compute_TimeDelta(TEXT("Timer_Default"));

		if (fTimeAcc >= 1.f / FPS_LIMIT)
		{
            const _double fTimeDelta = pGameInstance->Compute_TimeDelta(TEXT("Timer_FPS"));

            //if (0.016 <= fTimeDelta)
            //{
            //    cout << "\n\n\n\n! ! % # % # %  # % ! % @ # #\n";
            //    cout << fTimeDelta << "! ! % # % # %  # % ! % @ # #\n";
            //    cout << "! ! % # % # %  # % ! % @ # #\n\n\n\n";
            //}
            //else
            //    cout << fTimeDelta << endl;

            pMainApp->Tick(fTimeDelta);
            pMainApp->Render();
            pMainApp->FinishTick();
            /*if (MAX_DELTA >= fTimeDelta)
            {
			    pMainApp->Tick(fTimeDelta);
			    pMainApp->Render();
                pMainApp->FinishTick();
            }*/
			fTimeAcc = 0.f;
		}		
	}

	Safe_Release(pGameInstance);

	Safe_Release(pMainApp);

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    g_hInst = hInstance;

   RECT		rcWindow = { 0, 0, g_iWinSizeX, g_iWinSizeY };
   AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, TRUE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   g_hWnd = hWnd;

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    /* ImGui */
#ifdef _DEBUG
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) 
        return true;
#endif // _DEBUG

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
   /* case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(g_hWnd);
            break;
        }
        break;*/
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
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
