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

struct EdgeRec {
    double x, slopeInv;
    int yMax;

    EdgeRec(double x = 0, int yMax = 0, double slopeInv = 0) : x(x), yMax(yMax), slopeInv(slopeInv) {}
};

typedef vector<EdgeRec> EdgeTable[800];

void EdgeToTable(Point p1, Point p2, EdgeTable table)
{
    if (p1.y == p2.y)
        return;
    if (p1.y > p2.y)
        swap(p1, p2);
    double slopeInv = (p2.x - p1.x) / (double)(p2.y - p1.y);
    table[p1.y].emplace_back(EdgeRec(p1.x, p2.y, slopeInv));
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
    int y = 0;
    while (table[y].empty())
        y++;
    auto active = table[y];
    while (!active.empty())
    {
        sort(active.begin(), active.end(), [](EdgeRec a, EdgeRec b){
            return a.x < b.x;
        });

        for (int i = 0; i < active.size(); i += 2)
        {
            auto a = active[i], b = active[i + 1];
            DrawLine(hdc, a.x, y, b.x, y, c);
        }

        y++;

        for (int i = 0; i < active.size(); i++)
            if (active[i].yMax == y)
            {
                active.erase(active.begin() + i);
                i--;
            }

        for (auto& node : active)
            node.x += node.slopeInv;

        for (auto& node : table[y])
            active.emplace_back(node);
    }
}

void FillPolygon(HDC hdc, vector<Point>& points, COLORREF c)
{
    EdgeTable table;
    PolygonToTable(points, table);
    TableToScreen(hdc, table, c);
}

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    static int n = 5;
//    cout << "Enter number of points: ";
//    cin >> n;
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