#include <windows.h>
#include <string>

#include "resource.h"
#include "FuncCollection.h"
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

FuncCollection funcs(0, 0, 1, 1);

long __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
   
    PAINTSTRUCT ps;
    HDC hdc;

    RECT r;

    GetClientRect(hWnd, &r);

    switch (message) {
    case WM_KEYDOWN:
        if (wParam == VK_DOWN)
        {
            OPENFILENAME ofn;
            wchar_t szFileName[MAX_PATH] = L"";
            ZeroMemory(&ofn, sizeof(ofn));

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
            ofn.lpstrFile = szFileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            ofn.lpstrDefExt = L"txt";

            GetOpenFileName(&ofn);
                
           if(funcs.ReadExternalGraph(ofn))
                InvalidateRect(hWnd, nullptr, 1);
        }
        break;

    case WM_DESTROY:
    {
        int msgboxID = MessageBox(hWnd, L"Сохранить график в файл?", L"Сохранение файла", MB_YESNO | MB_ICONQUESTION);
        switch (msgboxID)
        {
        case IDYES: 
        {
            OPENFILENAME ofn;
            wchar_t szFileName[MAX_PATH] = L"";
            ZeroMemory(&ofn, sizeof(ofn));

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
            ofn.lpstrFile = szFileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            ofn.lpstrDefExt = L"txt";

            GetSaveFileName(&ofn);

            funcs.WriteGraphsToExternal(ofn);
            break;
        }
        default: 
            break;
        }

        PostQuitMessage(0);
        break;
    }
    case WM_RBUTTONDOWN:
        if (funcs.FuncCount != 0)
        {
            wchar_t buf[256];
            swprintf(buf, 256, L"Количество графиков: %d \n Количество точек по оси Х: %d \n Минимальный X: %f \n Максимальный Х: %f \n Минимальный Y: %f \n Максимальный Y: %f \n",
                funcs.FuncCount, funcs.func[0].xList.size(), funcs.MinX(), funcs.MaxX(), funcs.MinY(), funcs.MaxY());
            MessageBox(hWnd, buf, L"Сводная информация", MB_OK);
        }
        
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        DRAWGRID(hdc, r);

        funcs.DrawFuncs(hdc, r);
        

        EndPaint(hWnd, &ps);
        break;
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
