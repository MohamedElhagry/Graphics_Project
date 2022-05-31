#include <Windows.h>
#include <bits/stdc++.h>
using namespace std;

const int N = 1600;

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

typedef struct {
    int xLeft, xRight;
} EdgeTable[N];

void InitEdgeTable(EdgeTable table)
{
    for (int i = 0; i < N; i++)
    {
        table[i].xLeft = INT_MAX;
        table[i].xRight = INT_MIN;
    }
}

void EdgeToTable(Point p1, Point p2, EdgeTable table)
{
    if (p1.y == p2.y)
        return;
    if (p1.y > p2.y)
        swap(p1, p2);
    double slopeInv = (p2.x - p1.x) / (double)(p2.y - p1.y);
    double x = p1.x;
    for (int y = p1.y; y < p2.y; y++, x += slopeInv)
    {
        table[y].xLeft = min(table[y].xLeft, (int)ceil(x));
        table[y].xRight = max(table[y].xRight, (int)floor(x));
    }
}

void PolygonToTable(vector<Point> points, EdgeTable table)
{
    int n = points.size();
    Point p1 = points[n - 1];
    for (int i = 0; i < n; i++)
    {
        Point p2 = points[i];
        EdgeToTable(p1, p2, table);
        p1 = p2;
    }
}

void TableToScreen(HDC hdc, EdgeTable table, COLORREF c)
{
    for (int y = 0; y < N; y++)
        if (table[y].xLeft < table[y].xRight)
            DrawLine(hdc, table[y].xLeft, y, table[y].xRight, y, c);
}

void FillPolygon(HDC hdc, vector<Point> points, COLORREF c)
{
    EdgeTable table;
    InitEdgeTable(table);
    PolygonToTable(points, table);
    TableToScreen(hdc, table, c);
}

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    static int n = 6;
    static vector<Point> p(n);
    static int index = 0;
    switch (mcode)
    {
        case WM_LBUTTONDOWN:
            p[index] = Point(LOWORD(lp), HIWORD(lp));
            if (index == n - 1)
            {
                hdc = GetDC(hwnd);
                FillPolygon(hdc, p, RGB(255, 0, 0));
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