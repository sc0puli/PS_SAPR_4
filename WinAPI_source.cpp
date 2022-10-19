#include <windows.h>
#include <string>

#include "resource.h"
#include "plot.h"
#include "colorref.h"

using namespace std;

const wchar_t windowClass[] = L"win32app";
const wchar_t windowTitle[] = L"Win32API - PS SAPR #4";

void DRAWGRID(HDC hdc, RECT& rect)
{
    HBRUSH brush, old_brush;

    brush = CreateSolidBrush(LTGR);
    old_brush = (HBRUSH)SelectObject(hdc, brush);

    Rectangle(hdc, rect.left + 20, rect.top + 20, rect.right - 20, rect.bottom - 20);

    (HBRUSH)SelectObject(hdc, old_brush);
    (HBRUSH)DeleteObject(brush);

    HPEN pen, old_pen;

    pen = CreatePen(PS_SOLID, 1, GRIDCOLOR);
    old_pen = (HPEN)SelectObject(hdc, pen);

    for (long int i = rect.left + 20; i < rect.right - 20; i += 15)
    {
        for (long int j = rect.top + 20; j < rect.bottom - 20; j += 15)
        {
            MoveToEx(hdc, i, j, nullptr);
            LineTo(hdc, i, rect.bottom - 20);

            MoveToEx(hdc, j, i, nullptr);
            LineTo(hdc, rect.right - 20, i);
        }
    }

    (HPEN)SelectObject(hdc, old_pen);
    (HPEN)DeleteObject(pen);
}

Plot plt(0);

long __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
   
    PAINTSTRUCT ps;
    HDC hdc;
    RECT r;
    static OPENFILENAME ofn;

    GetClientRect(hWnd, &r);

    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);

        DRAWGRID(hdc, r);
        plt.DrawPlot(hdc);
        
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_KEYDOWN:
    {
        if (wParam == VK_ADD)
        {
            wchar_t szFileName[MAX_PATH] = L"";

            ZeroMemory(&ofn, sizeof(ofn));

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
            ofn.lpstrFile = szFileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            ofn.lpstrDefExt = L"txt";

            if (GetOpenFileName(&ofn))
                MessageBox(hWnd, ofn.lpstrFile, L"Имя файла", MB_OK);

            plt.ReadPlot(ofn);
           
            InvalidateRect(hWnd, nullptr, 1);
        }
        break;
    }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = (WNDPROC)WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = windowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, L"Can�t register window class!", L"Win32 API Test", NULL);
        return 1;
    }

    HWND hWnd = CreateWindow(windowClass, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 900, 500, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        MessageBox(NULL, L"Can�t create window!", L"Win32 API Test", NULL);
        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
