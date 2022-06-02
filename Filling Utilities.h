//
// Created by El belkasy on 5/31/2022.
//

#ifndef GRAPHICS_PROJECT_FILLING_UTILITIES_H
#define GRAPHICS_PROJECT_FILLING_UTILITIES_H

#include "DrawSimpleShapes.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

#include <gl\GLu.h>

inline bool sameCol(GLfloat *col1, GLfloat *col2) {
    for (int i = 0; i < 3; i++)
        if (col1[i] != col2[i])
            return false;

    cout << "invalid \n";
    return true;
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

inline void floodFill(int x, int y, int ytop, GLfloat *borderCol, GLfloat *fillCol) {
    glBegin(GL_POINTS);
    glColor3f(fillCol[0], fillCol[1], fillCol[2]);

    stack<Point> stack;
    stack.push(Point(x, y));
    while (!stack.empty()) {
        Point p = stack.top();
        stack.pop();
        GLfloat col[] = {0, 0, 0};
        getColor(x, y, col);
        cout << col[0] << " " << col[1] << " " << col[2] << endl;


        if (sameCol(col, borderCol) || sameCol(col, fillCol) || y == ytop)
            continue;

        glVertex2d(p.x, p.y);

        stack.push(Point(p.x, p.y - 1));
        stack.push(Point(p.x, p.y + 1));
        stack.push(Point(p.x - 1, p.y));
        stack.push(Point(p.x + 1, p.y));
    }

    glEnd();
    glFlush();
}

#endif //GRAPHICS_PROJECT_FILLING_UTILITIES_H
