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
        if (abs(col1[i] - col2[i]) > 0.0001f)
            return false;

    cout << "invalid " << endl;
    return true;
}

inline void floodFill(GLint x, GLint y, int toolsHeight, int screenHeight, int screenWidth, GLfloat *borderCol,
                      GLfloat *fill) {
    stack<Point> stack;
    stack.push(Point(x, y));

    bool filled[screenWidth][screenHeight];

    for (int i = 0; i < screenWidth; i++)
        for (int j = 0; j < screenHeight; j++)
            filled[i][j] = false;

    while (!stack.empty()) {
        Point p = stack.top();
        x = p.x;
        y = p.y;
        stack.pop();

        if (!filled[x][y] && (x >= 0) && (x < screenWidth && (y >= toolsHeight) && (y < screenHeight))) {

            GLfloat *color = getPixelColor(x, y);

            cout << color[0] << ' ' << color[1] << ' ' << color[2] << endl;

            if (sameCol(color, borderCol))
                continue;

            cout << x << ' ' << y << ' ' << filled[x][y] << endl;

            glactivate();
            glColor3f(fill[0], fill[1], fill[2]);

            glVertex2d(x, y);

            gldeactivate();

            filled[x][y] = true;
            stack.push(Point(x, y - 1));
            stack.push(Point(x, y + 1));
            stack.push(Point(x - 1, y));
            stack.push(Point(x + 1, y));

        }
    }

}

#endif //GRAPHICS_PROJECT_FILLING_UTILITIES_H
