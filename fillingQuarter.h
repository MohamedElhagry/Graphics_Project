//
// Created by Mohamed on 5/31/2022.
//

#ifndef GRAPHICS_PROJECT_FILLINGQUARTER_H
#define GRAPHICS_PROJECT_FILLINGQUARTER_H

#include <Windows.h>
#include <bits/stdc++.h>
using namespace std;

#include "DrawSimpleShapes.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#include <gl\GLu.h>

inline void Draw2Lines( int xc, int yc, int x, int y, GLfloat *drawingColor, int quarter)
{
    cout << quarter << endl;
    switch (quarter)
    {
        case 1:
            drawLine( xc, yc, xc + x, yc + y, drawingColor);
            drawLine( xc, yc, xc + y, yc + x, drawingColor);
            break;
        case 2:
            drawLine( xc, yc, xc + x, yc - y, drawingColor);
            drawLine( xc, yc, xc + y, yc - x, drawingColor);
            break;
        case 3:
            drawLine( xc, yc, xc - y, yc - x, drawingColor);
            drawLine( xc, yc, xc - x, yc - y, drawingColor);
            break;
        case 4:
            drawLine( xc, yc, xc - y, yc + x, drawingColor);
            drawLine( xc, yc, xc - x, yc + y, drawingColor);
            break;
    }
}

inline void FillQuarter(int xc, int yc, int R, GLfloat *drawingColor, int quarter)
{
    int x = 0, y = R;
    int d = 1 - R, d1 = 3, d2 = 5 - 2 * R;
    do {
        Draw2Lines(xc, yc, x, y, drawingColor, quarter);
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

/*
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
*/
#endif //GRAPHICS_PROJECT_FILLINGQUARTER_H
