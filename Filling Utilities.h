//
// Created by El belkasy on 5/31/2022.
//

#ifndef GRAPHICS_PROJECT_FILLING_UTILITIES_H
#define GRAPHICS_PROJECT_FILLING_UTILITIES_H

#include "DrawSimpleShapes.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

#include <gl\GLu.h>


inline void floodFill(HDC hdc, int x, int y, int toolsHeight, int screenHeight, int screenWidth, COLORREF borderCol,
                      GLfloat *fill) {

    stack<Point> stack;
    stack.push(Point(x, y));

    bool filled[screenWidth][screenHeight];
    for (int i = 0; i < screenWidth; i++)
        for (int j = 0; j < screenHeight; j++)
            filled[i][j] = false;

    while (!stack.empty()) {
        Point p = stack.top();
        stack.pop();

        if (!filled[p.x][p.y] && (p.x >= 0) && (p.x < screenWidth && (p.y >= toolsHeight) && (p.y < screenHeight))) {
            COLORREF col = GetPixel(hdc, p.x, p.y);

            cout << col << ' ' << borderCol << endl;
            if (col == borderCol)
                continue;
            glBegin(GL_POINTS);
            glColor3f(fill[0], fill[1], fill[2]);

            glVertex2d(p.x, p.y);

            glEnd();
            glFlush();

            filled[p.x][p.y] = true;

            stack.push(Point(p.x, p.y - 1));
            stack.push(Point(p.x, p.y + 1));
            stack.push(Point(p.x - 1, p.y));
            stack.push(Point(p.x + 1, p.y));

        }
    }

}

inline void floodFill(HDC hdc, int x, int y, int toolsHeight, int screenHeight, int screenWidth, COLORREF borderCol,
                      COLORREF fillCol) {
    stack<Point> stack;
    stack.push(Point(x, y));
    while (!stack.empty()) {
        Point p = stack.top();
        stack.pop();

        bool filled[screenWidth][screenHeight];
        for (int i = 0; i < screenWidth; i++)
            for (int j = 0; j < screenHeight; j++)
                filled[x][y] = false;

        if (!filled[p.x][p.y] && (p.x >= 0) && (p.x < screenWidth && (p.y >= toolsHeight) && (p.y < screenHeight))) {
            COLORREF col = GetPixel(hdc, p.x, p.y);

            if (col == borderCol || col == fillCol)
                continue;

            SetPixel(hdc, p.x, p.y, fillCol);
            filled[p.x][p.y] = true;
            cout << col << ' ' << fillCol << ' ' << p.x << " " << p.y << endl;

            stack.push(Point(p.x, p.y - 1));
            stack.push(Point(p.x, p.y + 1));
            stack.push(Point(p.x - 1, p.y));
            stack.push(Point(p.x + 1, p.y));
        }
    }
}

#endif //GRAPHICS_PROJECT_FILLING_UTILITIES_H
