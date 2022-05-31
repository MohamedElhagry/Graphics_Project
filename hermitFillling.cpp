#include <Windows.h>
#include "Draw Simple Shapes.cpp"

void DrawHermiteCurve(HDC hdc, Point p1, Point T1, Point p2, Point T2, COLORREF color)
{
    double a0 = p1.x,
            a1 = T1.x,
            a2 = -3 * p1.x - 2 * T1.x + 3 * p2.x - T2.x,
            a3 = 2 * p1.x + T1.x - 2 * p2.x + T2.x;
    double b0 = p1.y,
            b1 = T1.y,
            b2 = -3 * p1.y - 2 * T1.y + 3 * p2.y - T2.y,
            b3 = 2 * p1.y + T1.y - 2 * p2.y + T2.y;
    for (double t = 0; t <= 1; t += 0.001)
    {
        double t2 = t * t, t3 = t2 * t;
        double x = a0 + a1 * t + a2 * t2 + a3 * t3;
        double y = b0 + b1 * t + b2 * t2 + b3 * t3;
        SetPixel(hdc, round(x), round(y), color);
    }
}

void DrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dy) <= abs(dx))
    {
        if (x1 > x2)
            swap(x1, x2), swap(y1, y2);
        SetPixel(hdc, x1, y1, c);
        int x = x1;
        while (x < x2)
        {
            x++;
            double y = y1 + (double)(x - x1) * dy / dx;
            SetPixel(hdc, x, round(y), c);
        }
    }
    else
    {
        if (y1 > y2)
            swap(x1, x2), swap(y1, y2);
        SetPixel(hdc, x1, y1, c);
        int y = y1;
        while (y < y2)
        {
            y++;
            double x = x1 + (double)(y - y1) * dx / dy;
            SetPixel(hdc, round(x), y, c);
        }
    }
}

void DrawRectangle(HDC hdc, int x1, int y1, int x3, int y3, COLORREF c)
{
    int x2 = x1, y2 = y3;
    int x4 = x3, y4 = y1;
    DrawLine(hdc, x1, y1, x2, y2, c);
    DrawLine(hdc, x2, y2, x3, y3, c);
    DrawLine(hdc, x3, y3, x4, y4, c);
    DrawLine(hdc, x4, y4, x1, y1, c);
}

void FillRectangleWithHermite(HDC hdc, int x1, int y1, int x3, int y3, COLORREF c)
{
    Point T1(0, 0);
    Point T2(0, 0);
    for (int x = x1; x <= x3; x++)
        DrawHermiteCurve(hdc, Point(x, y1), T1, Point(x, y3), T2, c);
}

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    static Point p[2];
    static int index = 0;
    static int side;
    switch (mcode)
    {
        case WM_LBUTTONDOWN:
            p[index] = Point(LOWORD(lp), HIWORD(lp));
            if (index == 1)
            {
                hdc = GetDC(hwnd);
                side = hypot(p[0].x - p[1].x, p[0].y - p[1].y);
                DrawRectangle(hdc, p[0].x, p[0].y, p[0].x + side, p[0].y + side, RGB(255, 0, 0));
                FillRectangleWithHermite(hdc, p[0].x, p[0].y, p[0].x + side, p[0].y + side, RGB(255, 0, 0));
                ReleaseDC(hwnd, hdc);
            }
            index++;
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}