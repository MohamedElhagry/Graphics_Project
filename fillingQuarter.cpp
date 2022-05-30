#include <Windows.h>
#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;

    Point(int x = 0, int y = 0)
    {
        this->x = x;
        this->y = y;
    }
};

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

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
    SetPixel(hdc, xc + x, yc + y, c);
    SetPixel(hdc, xc + x, yc - y, c);
    SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc - x, yc - y, c);
    SetPixel(hdc, xc + y, yc + x, c);
    SetPixel(hdc, xc + y, yc - x, c);
    SetPixel(hdc, xc - y, yc + x, c);
    SetPixel(hdc, xc - y, yc - x, c);
}

void Draw2Lines(HDC hdc, int xc, int yc, int x, int y, COLORREF c, int quarter)
{
    cout << quarter << endl;
    switch (quarter)
    {
        case 1:
            DrawLine(hdc, xc, yc, xc + x, yc + y, c);
            DrawLine(hdc, xc, yc, xc + y, yc + x, c);
            break;
        case 2:
            DrawLine(hdc, xc, yc, xc + x, yc - y, c);
            DrawLine(hdc, xc, yc, xc + y, yc - x, c);
            break;
        case 3:
            DrawLine(hdc, xc, yc, xc - y, yc - x, c);
            DrawLine(hdc, xc, yc, xc - x, yc - y, c);
            break;
        case 4:
            DrawLine(hdc, xc, yc, xc - y, yc + x, c);
            DrawLine(hdc, xc, yc, xc - x, yc + y, c);
            break;
    }
}

void DrawCircle(HDC hdc, int xc, int yc, int R, COLORREF c)
{
    int x = 0, y = R;
    int d = 1 - R, d1 = 3, d2 = 5 - 2 * R;
    do {
        Draw8Points(hdc, xc, yc, x, y, c);
        if (d < 0)
        {
            d += d1;
            d2 += 2;
        }
        else
        {
            d += d2;
            d2 += 4;
            y--;
        }
        d1 += 2;
        x++;

    } while (x < y);
}

void FillQuarter(HDC hdc, int xc, int yc, int R, COLORREF c, int quarter)
{
    int x = 0, y = R;
    int d = 1 - R, d1 = 3, d2 = 5 - 2 * R;
    do {
        Draw2Lines(hdc, xc, yc, x, y, c, quarter);
        if (d < 0)
        {
            d += d1;
            d2 += 2;
        }
        else
        {
            d += d2;
            d2 += 4;
            y--;
        }
        d1 += 2;
        x++;

    } while (x < y);
}

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
    static HDC hdc;
    static Point p[3];
    static int idx = 0, R;
    switch (mcode)
    {
        case WM_LBUTTONDOWN:
            p[idx] = Point(LOWORD(lp), HIWORD(lp));
            if (idx == 1)
            {
                hdc = GetDC(hwnd);

                R = hypot(p[0].x - p[1].x, p[0].y - p[1].y);
                DrawCircle(hdc, p[0].x, p[0].y, R, RGB(255, 0, 0));
                ReleaseDC(hwnd, hdc);
            }
            else if (idx > 1)
            {
                idx--;
                if (hypot(p[2].x - p[0].x, p[2].y - p[0].y) > R)  // point is outside the circle
                    break;
                hdc = GetDC(hwnd);

                if ((p[2].x > p[0].x) && (p[2].y > p[0].y))
                    FillQuarter(hdc, p[0].x, p[0].y, R, RGB(255, 0, 0), 1);
                else if ((p[2].x > p[0].x) && (p[2].y < p[0].y))
                    FillQuarter(hdc, p[0].x, p[0].y, R, RGB(255, 0, 0), 2);
                else if ((p[2].x < p[0].x) && (p[2].y < p[0].y))
                    FillQuarter(hdc, p[0].x, p[0].y, R, RGB(255, 0, 0), 3);
                else
                    FillQuarter(hdc, p[0].x, p[0].y, R, RGB(255, 0, 0), 4);

                ReleaseDC(hwnd, hdc);
            }
            idx++;
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default: return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}
