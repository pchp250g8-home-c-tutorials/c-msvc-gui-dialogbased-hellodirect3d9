// HelloDirect3D.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "HelloDirect3D.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
byte nRedColor;
HWND m_hWnd;
BOOL bActive;
D3DPRESENT_PARAMETERS d3dpp;
IDirect3D9* m_d3d9;
IDirect3DDevice9* m_d3d_Device;

// Forward declarations of functions included in this code module:
BOOL                InitInstance(HINSTANCE, int);
INT_PTR CALLBACK    HelloDirect3DDlg(HWND, UINT, WPARAM, LPARAM);
//------------------------------------------------------------------------------------------------------
BOOL InitializeDirect3D();
void DestroyDirect3D();
void RenderScene();
//------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HELLODIRECT3D, szWindowClass, MAX_LOADSTRING);
    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    return 0;
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
   INT_PTR nResponse;
   INITCOMMONCONTROLSEX InitCtrls;
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной
   InitCtrls.dwSize = sizeof(InitCtrls);
   // Выберите этот параметр для включения всех общих классов управления, которые необходимо использовать
   // в вашем приложении.
   InitCtrls.dwICC = ICC_WIN95_CLASSES;
   InitCommonControlsEx(&InitCtrls);
   nResponse = DialogBox(hInst, MAKEINTRESOURCE(IDD_HELLEO_DIRECT3D_DIALOG), NULL, HelloDirect3DDlg);
   if (nResponse == IDOK)
   {
       // TODO: Введите код для обработки закрытия диалогового окна
       //  с помощью кнопки "ОК"
   }
   else if (nResponse == IDCANCEL)
   {
       // TODO: Введите код для обработки закрытия диалогового окна
       //  с помощью кнопки "Отмена"
   }
   else if (nResponse == -1)
       return FALSE;
   return TRUE;

   return TRUE;
}



// Message handler for about box.
INT_PTR CALLBACK HelloDirect3DDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        m_hWnd = hDlg;
        if (!InitializeDirect3D())
        {
            MessageBox(NULL, _T("Error Loading DirectX"), _T("Error"), MB_OK | MB_ICONERROR);
            PostQuitMessage(0);
            return FALSE;
        }
        SetTimer(hDlg, IDR_TIMER1, 1, NULL);
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    case WM_ACTIVATE:
    {
        bActive = LOWORD(wParam);
        break;
    }
    case WM_TIMER:
    {
        RenderScene();
        break;
    }
   
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);
        // TODO: Add any drawing code that uses hdc here...
        RenderScene();
        EndPaint(hDlg, &ps);
        break;
    }
    case WM_DESTROY:
        KillTimer(hDlg, IDR_TIMER1);
        DestroyDirect3D();
        PostQuitMessage(0);
        break;
    }
    return (INT_PTR)FALSE;
}


BOOL InitializeDirect3D()
{
    D3DDISPLAYMODE d3ddm;
    HRESULT hr;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    ZeroMemory(&d3ddm, sizeof(d3ddm));
    m_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
    if (m_d3d9 == NULL) return FALSE;
    hr = m_d3d9->lpVtbl->GetAdapterDisplayMode(m_d3d9, 0, &d3ddm);
    if (FAILED(hr)) return FALSE;
    d3dpp.Windowed = TRUE;
    d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    hr = m_d3d9->lpVtbl->CreateDevice
    (
        m_d3d9, 
        0, 
        D3DDEVTYPE_HAL, 
        m_hWnd, 
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
        &d3dpp, 
        &m_d3d_Device
    );
    if (FAILED(hr)) return FALSE;
    return TRUE;
}

void DestroyDirect3D()
{
    if (m_d3d_Device != NULL)
    {
        m_d3d_Device->lpVtbl->Release(m_d3d_Device);
        m_d3d_Device = NULL;
    }
        
    if (m_d3d9 != NULL)
    {
        m_d3d9->lpVtbl->Release(m_d3d9);
        m_d3d9 = NULL;
    }
        
}

void RenderScene()
{
    if (m_d3d9 == NULL) return;
    if (m_d3d_Device == NULL) return;
    m_d3d_Device->lpVtbl->Clear
    (
        m_d3d_Device,
        0, NULL, 
        D3DCLEAR_TARGET, 
        D3DCOLOR_XRGB(nRedColor++, 0, 0), 
        0, 0
    );
    m_d3d_Device->lpVtbl->BeginScene(m_d3d_Device);
    m_d3d_Device->lpVtbl->EndScene(m_d3d_Device);
    m_d3d_Device->lpVtbl->Present(m_d3d_Device, NULL, NULL, 0, NULL);
}