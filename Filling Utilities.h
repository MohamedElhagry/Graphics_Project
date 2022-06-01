//
// Created by El belkasy on 5/31/2022.
//

#ifndef GRAPHICS_PROJECT_FILLING_UTILITIES_H
#define GRAPHICS_PROJECT_FILLING_UTILITIES_H
#include "DrawSimpleShapes.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

#include <gl\GLu.h>

inline void floodFill(HDC hdc, int x, int y, int ytop, COLORREF borderCol, COLORREF fillCol)
{
    stack<Point> stack;
    stack.push(Point(x,y));
    while(!stack.empty())
    {
        Point p = stack.top();
        stack.pop();
        GLfloat col = GetPixel(hdc, p.x, p.y);
        if(col == borderCol || col == fillCol || y == ytop)
            continue;

        SetPixel(hdc, x, y, fillCol);

        stack.push(Point(p.x, p.y-1));
        stack.push(Point(p.x, p.y+1));
        stack.push(Point(p.x-1, p.y));
        stack.push(Point(p.x+1, p.y));

    }


}
#endif //GRAPHICS_PROJECT_FILLING_UTILITIES_H
