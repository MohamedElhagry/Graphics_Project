//
// Created by Mohamed on 5/31/2022.
//

#ifndef GRAPHICS_PROJECT_NONCONVEX_H
#define GRAPHICS_PROJECT_NONCONVEX_H
#include "DrawSimpleShapes.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#include <gl\GLu.h>

using namespace std;

struct EdgeRec {
    double x, slopeInv;
    int yMax;

    EdgeRec(double x = 0, int yMax = 0, double slopeInv = 0) : x(x), yMax(yMax), slopeInv(slopeInv) {}
};


typedef vector<EdgeRec> EdgeTable[800];

inline void EdgeToTable(Point p1, Point p2, EdgeTable table)
{
    if (p1.y == p2.y)
        return;
    if (p1.y > p2.y)
        swap(p1, p2);
    double slopeInv = (p2.x - p1.x) / (double)(p2.y - p1.y);
    table[p1.y].emplace_back(EdgeRec(p1.x, p2.y, slopeInv));
}

inline void PolygonToTable(vector<Point> points, EdgeTable table)
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

inline void TableToScreen(EdgeTable table, GLfloat *drawingColor)
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
            drawLine(a.x, y, b.x, y, drawingColor);
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


inline void FillPolygon(vector <Point> points, GLfloat *drawingColor)
{
    EdgeTable table;
    PolygonToTable(points, table);
    TableToScreen(table, drawingColor);
}


inline void FillPolygon(Point* points, int n, GLfloat *drawingColor)
{
    vector<Point> pointVec;
    for(int i=0; i<n; i++)
        pointVec.push_back(Point(points[i].x, points[i].y));
    FillPolygon(pointVec, drawingColor);
}

#endif //GRAPHICS_PROJECT_NONCONVEX_H
