// Lab_3(Dasha).cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Lab_3(Dasha).h"

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "Resource.h"
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#define MAX_LOADSTRING 100

// Глобальные переменные:
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;

int step = 0;
float distance = 0.f;
float angle = 0.f;
float t = 0.f;
float scale1 = 0.f;
float scale2 = 5.f;
UINT frameIndex;
UINT frameCount;

Image* people, * logo, *image;

HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
VOID Display(HDC);
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB3DASHA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB3DASHA));

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
    GdiplusShutdown(gdiplusToken);
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB3DASHA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB3DASHA);
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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   people = Bitmap(L"people.png").GetThumbnailImage(66, 70);
   logo = Bitmap(L"2.jpg").GetThumbnailImage(60, 24);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
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
    case WM_CREATE:
    {
        image = Image::FromFile(L"background.gif");
        if (image == NULL) return -1;

        frameIndex = 0;
        frameCount = image->GetFrameCount(&FrameDimensionTime);

        SetTimer(hWnd, 1, 10, NULL);
        return 0;
    }
    case WM_TIMER:
    {
        frameIndex = (frameIndex + 1) % frameCount;
        image->SelectActiveFrame(&FrameDimensionTime, frameIndex);
        step++;
        t += 0.01f;
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
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
    case WM_ERASEBKGND: return 1;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            Display(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        if (people != NULL)
        {
            delete people;
        }
        if (logo != NULL)
        {
            delete logo;
        }
        if (image != NULL)
        {
            delete image;
        }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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

PointF Tween(const PointF& A, const PointF& B, float t)
{
    return PointF(A.X * (1.f - t) + B.X * t, A.Y * (1.f - t) + B.Y * t);
}

void Tween(const PointF* A, const PointF* B, PointF* P, int count, float t)
{
    for (int i = 0; i < count; i++)
    {
        P[i] = Tween(A[i], B[i], t);
    }
}

VOID DisplayTween(Graphics& g)
{
    Rect rect;
    g.GetVisibleClipBounds(&rect);
    Bitmap backbuffer(rect.Width, rect.Height, &g);

    Graphics temp(&backbuffer);
    temp.SetSmoothingMode(SmoothingModeHighQuality);
    SolidBrush brush(Color::Tan);

    PointF start_points[] = {
        PointF(1300.f, 50.f),
        PointF(1400.f, 50.f),
        PointF(1400.f, 150.f),
        PointF(1300.f, 150.f)
    };
    PointF end_points[] = {
        PointF(1300.f, 50.f),
        PointF(1350.f, 100.f),
        PointF(1300.f, 150.f),
        PointF(1250.f, 100.f)
    };
    PointF int_points[4];
    Tween(start_points, end_points, int_points, 4, t);
    g.FillPolygon(&brush, int_points, 4);
}

VOID DisplayPicture(Graphics& g) 
{
    Rect rect;
    g.GetVisibleClipBounds(&rect);

    Bitmap backBuffer(rect.Width, rect.Height, &g);

    Graphics temp(&backBuffer);

    temp.SetSmoothingMode(SmoothingModeHighQuality);

    //основа
    SolidBrush solidBrush(Color::LightGreen);
    g.FillRectangle(&solidBrush, 100, 200, 600, 200);
    TextureBrush tBrush2(logo, WrapModeTile);
    Pen pen1(&tBrush2, 24);
    g.DrawLine(&pen1, 100, 324, 500, 324);
    //окна
    TextureBrush tBrush(people, WrapModeTile);
    Color color = Color::Blue;
    INT n = 1;
    solidBrush.SetColor(Color::LightBlue);
    g.FillRectangle(&tBrush, 120, 230, 100, 70);
    g.FillRectangle(&solidBrush, 600, 230, 100, 70);
    g.FillRectangle(&tBrush, 240, 230, 100, 70);
    g.FillRectangle(&tBrush, 360, 230, 100, 70);


    //дверь
    SolidBrush solidBrush1(Color::LightBlue);
    g.FillRectangle(&solidBrush1, 500, 230, 70, 80);
    SolidBrush solidBrush2(Color::DarkGreen);
    g.FillRectangle(&solidBrush2, 500, 310, 70, 80);
    Pen penBlack(Color::Black, 2);
    g.DrawLine(&penBlack, 535, 230, 535, 390);

    //большие колеса
    solidBrush.SetColor(Color::Black);
    HatchBrush hatchBrush(HatchStyleDashedUpwardDiagonal, Color::Black, Color::Gray);
    g.FillEllipse(&hatchBrush, 570, 370, 80, 80);
    g.FillEllipse(&hatchBrush, 165, 370, 80, 80);
    g.FillEllipse(&solidBrush, 585, 385, 50, 50);
    g.FillEllipse(&solidBrush, 180, 385, 50, 50);

    Pen pen(Color::SaddleBrown, 4);

    ////крыша
    solidBrush.SetColor(Color::Green);
    g.FillRectangle(&solidBrush, 255, 180, 200, 20);


    solidBrush.SetColor(Color::Honeydew);
    /*Region region(Rect(630, 200, 80, 65));
    g.SetClip(&region, CombineModeReplace);
    g.FillEllipse(&solidBrush, 550, 200, 160, 130);
    Region infiniteRegion;
    g.SetClip(&infiniteRegion, CombineModeReplace);*/

    GraphicsPath path;
    path.AddEllipse(480, 200, 220, 160);
    Region rgn2(&path);
    g.SetClip(&rgn2, CombineModeExclude);
    g.FillRectangle(&solidBrush, 580, 200, 130, 75);
    Region infiniteRegion;
    g.SetClip(&infiniteRegion);

    GraphicsPath path1;
    path1.AddEllipse(100, 200, 160, 150);
    Region rgn(&path1);
    g.SetClip(&rgn, CombineModeExclude);
    g.FillRectangle(&solidBrush, 100, 200, 80, 75);
    g.SetClip(&infiniteRegion);


    //текст на корпусе
    Font font(TEXT("Arial"), 15, FontStyleBoldItalic);
    solidBrush.SetColor(Color::White);
    g.DrawString(TEXT("№145"), -1, &font, PointF(590, 310), &solidBrush);
    //фонарь
    solidBrush.SetColor(Color::Yellow);
    g.FillEllipse(&solidBrush, 700, 340, 10, 20);
    pen.SetColor(Color::Black);
    pen.SetWidth(2);
    g.DrawEllipse(&pen, 700, 340, 10, 20);
    //лучи
    color = Color::Yellow;
    g.FillPie(&solidBrush, 605, 250, 200, 200, 330, 60);
}

VOID Display(HDC hdc)
{
    Graphics g(hdc);

    Rect rect;
    g.GetVisibleClipBounds(&rect);

    Bitmap backBuffer(rect.Width, rect.Height, &g);

    Graphics temp(&backBuffer);
    temp.DrawImage(image, 0, 0, rect.Width, rect.Height);

    distance = 20.f * (step - 50);
    temp.ScaleTransform(0.4f, 0.5f);
    scale1 = scale1 + 0.01f;

    scale2 = scale2 - 0.05f;

    if (t<1)
    {
        DisplayTween(temp);
    }
    else
    {
        t = 0;
    }

    if (step < 180)
    {
        temp.TranslateTransform(0.f + distance, 600.f);
        temp.ScaleTransform(scale1, 0.7f);
        DisplayPicture(temp);
    }
    else
    {
        step = -1;
        scale1 = 0;
        temp.ResetTransform();
    }


    g.DrawImage(&backBuffer, rect);
}