// Gorodezki_20.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Gorodezki_20.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                    // текущий экземпляр
WCHAR   szTitle[MAX_LOADSTRING];                    // Текст строки заголовка
WCHAR   szWindowClass[MAX_LOADSTRING];              // имя класса главного окна
int     xView, yView;
float   A, B;
int     M_SLOT2[15][2] = { {0,0}, {1,1}, {3,2}, {4,4}, {5,5}, {7,6}, {8,8}, {9,10}, {11,11}, {12,13}, {13,10}, {14,13}, {15,15}, {16,19}, {17,17} };
int     M_SLOT3[15][2] = { {0,0}, {-1,1}, {-3,2}, {-4,4}, {-5,5}, {-7,6}, {-8,8}, {-9,10}, {-11,11}, {-12,13}, {-13,10}, {-14,13}, {-15,15}, {-16,19}, {-17,17} };
int     M_SLOT1[15][2] = { {0,0}, {1,-1}, {3,-2}, {4,-4}, {5,-5}, {7,-6}, {8,-8}, {9,-10}, {11,-11}, {12,-13}, {13,-10}, {14,-13}, {15,-15}, {16,-19}, {17,-17} };

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                MainWndAddWidgets(HWND);
BOOL                Line(HDC, int, int, int, int, HPEN);
BOOL                Dot(HDC, int, int, HBRUSH);
void                Approximation();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GORODEZKI20, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GORODEZKI20));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GORODEZKI20));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GORODEZKI20);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 400, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   MainWndAddWidgets(hWnd);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
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
            HPEN hSeparatorPen, hAxisPen;
            HBRUSH  hDotBrush;

            hDotBrush = CreateSolidBrush(RGB(10, 200, 100));
            hSeparatorPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
            hAxisPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

            Line(hdc, 300, 0, 300, yView, hSeparatorPen);

            Line(hdc, 300 + (xView - 300)/2, 20, 300 + (xView - 300) / 2, yView - 20, hAxisPen);
            Line(hdc, 300 + 20, yView/2, xView - 20, yView / 2, hAxisPen);

            SetMapMode(hdc, MM_ISOTROPIC); //логические единицы отображаем, как физические
            SetWindowExtEx(hdc, 500, 500, NULL); //Длина осей
            SetViewportExtEx(hdc, xView, -yView, NULL); //Определяем облась вывода
            SetViewportOrgEx(hdc, 300 + (xView - 300) / 2, yView / 2, NULL); //Начало координат
            MoveToEx(hdc, 0, 0, NULL); //сделать текущими координаты x1, y1
            for (int i = 0; i < 15; i++)
            {
                Dot(hdc, M_SLOT1[i][0] * 10, M_SLOT1[i][1] * 10, hDotBrush);
            }
            Approximation();
            MoveToEx(hdc, 0, 0, NULL); //сделать текущими координаты x1, y1
            for (int i = 0; i < 500; i++)
            {
                float y = A + B * i;
                LineTo(hdc, i, (int)y);
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        xView = LOWORD(lParam);
        yView = HIWORD(lParam);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void MainWndAddWidgets(HWND hWnd)
{
    CreateWindow(L"button", L"Slot 1", WS_VISIBLE | WS_CHILD, 5, 5, 100, 30, hWnd, NULL, NULL, NULL);
    CreateWindow(L"button", L"Slot 2", WS_VISIBLE | WS_CHILD, 5, 45, 100, 30, hWnd, NULL, NULL, NULL);
    CreateWindow(L"button", L"Slot 3", WS_VISIBLE | WS_CHILD, 5, 85, 100, 30, hWnd, NULL, NULL, NULL);

    CreateWindow(L"static", L"X = ", WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, 130, 30, 30, 30, hWnd, NULL, NULL, NULL);
    CreateWindow(L"edit", NULL, WS_VISIBLE | WS_CHILD | ES_NUMBER, 155, 30, 40, 20, hWnd, NULL, NULL, NULL);

    CreateWindow(L"static", L"Y = ", WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, 200, 30, 30, 30, hWnd, NULL, NULL, NULL);
    CreateWindow(L"edit", NULL, WS_VISIBLE | WS_CHILD | ES_NUMBER, 225, 30, 40, 20, hWnd, NULL, NULL, NULL);

    CreateWindow(L"button", L"Clear slot", WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, 120, 65, 100, 30, hWnd, NULL, NULL, NULL);
    CreateWindow(L"button", L"Add", WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, 230, 65, 45, 30, hWnd, NULL, NULL, NULL);
}

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2, HPEN hPen)
{
    SelectObject(hdc, hPen);
    MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
    return LineTo(hdc, x2, y2); //нарисовать линию
}

BOOL Dot(HDC hdc, int x, int y, HBRUSH hBrush)
{
    SelectObject(hdc, hBrush);
    return Ellipse(hdc, x - 6, y - 6, x + 6, y + 6); //нарисовать линию
}

void Approximation()
{
    int sumX = 0;
    int sumY = 0;
    int sumPowX = 0;
    int sumXY = 0;
    int n = 15;

    for (int i = 0; i < n; i++)
    {
        sumX += M_SLOT1[i][0];
        sumY += M_SLOT1[i][1];
        sumPowX += M_SLOT1[i][0] * M_SLOT1[i][0];
        sumXY += M_SLOT1[i][0] * M_SLOT1[i][1];
    }

    int D = (n * sumPowX) - (sumX * sumX);

    int Da = (sumY * sumPowX) - (sumXY * sumX);
    int Db = (n * sumXY) - (sumX * sumY);

    A = (float)Da / (float)D;
    B = (float)Db / (float)D;
}

// Обработчик сообщений для окна "О программе".
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
