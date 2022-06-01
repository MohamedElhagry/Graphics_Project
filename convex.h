//
// Created by Mohamed on 5/31/2022.
//

#ifndef GRAPHICS_PROJECT_CONVEX_H
#define GRAPHICS_PROJECT_CONVEX_H
#include "DrawSimpleShapes.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#include <gl\GLu.h>
const int N = 1600;

typedef struct {
    int xLeft, xRight;
} EdgeTableC[N];

inline void InitEdgeTableC(EdgeTableC table)
{
    for (int i = 0; i < N; i++)
    {
        table[i].xLeft = INT_MAX;
        table[i].xRight = INT_MIN;
    }
}

inline void EdgeToTableC(Point p1, Point p2, EdgeTableC table)
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

inline void PolygonToTableC(vector<Point> points, EdgeTableC table)
{
    int n = points.size();
    Point p1 = points[n - 1];
    for (int i = 0; i < n; i++)
    {
        Point p2 = points[i];
        EdgeToTableC(p1, p2, table);
        p1 = p2;
    }
}

inline void TableToScreenC(EdgeTableC table, GLfloat *drawingColor)
{
    for (int y = 0; y < N; y++)
        if (table[y].xLeft < table[y].xRight)
            drawLine(table[y].xLeft, y, table[y].xRight, y, drawingColor);
}

inline void FillPolygonC(vector<Point> points, GLfloat *drawingColor)
{
    EdgeTableC table;
    InitEdgeTableC(table);
    PolygonToTableC(points, table);
    TableToScreenC(table, drawingColor);
}

inline void FillPolygonC(Point* points, int n, GLfloat *drawingColor)
{
    vector<Point> pointVec;
    for(int i=0; i<n; i++)
        pointVec.push_back(Point(points[i].x, points[i].y));
    FillPolygonC(pointVec, drawingColor);
}

#endif //GRAPHICS_PROJECT_CONVEX_H
