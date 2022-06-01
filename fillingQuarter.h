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

inline void Draw2Lines(int xc, int yc, int x, int y, GLfloat *drawingColor, int quarter) {
    cout << quarter << endl;
    switch (quarter) {
        case 1:
            drawLine(xc, yc, xc + x, yc + y, drawingColor);
            drawLine(xc, yc, xc + y, yc + x, drawingColor);
            break;
        case 2:
            drawLine(xc, yc, xc + x, yc - y, drawingColor);
            drawLine(xc, yc, xc + y, yc - x, drawingColor);
            break;
        case 3:
            drawLine(xc, yc, xc - y, yc - x, drawingColor);
            drawLine(xc, yc, xc - x, yc - y, drawingColor);
            break;
        case 4:
            drawLine(xc, yc, xc - y, yc + x, drawingColor);
            drawLine(xc, yc, xc - x, yc + y, drawingColor);
            break;
    }
}

inline void FillQuarter(int xc, int yc, int R, GLfloat *drawingColor, int quarter) {
    int x = 0, y = R;
    int d = 1 - R, d1 = 3, d2 = 5 - 2 * R;
    do {
        Draw2Lines(xc, yc, x, y, drawingColor, quarter);
        if (d < 0) {
            d += d1;
            d2 += 2;
        } else {
            d += d2;
            d2 += 4;
            y--;
        }
        d1 += 2;
        x++;

    } while (x < y);
}

#endif //GRAPHICS_PROJECT_FILLINGQUARTER_H
